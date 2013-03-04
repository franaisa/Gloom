//---------------------------------------------------------------------------
// Billboard.cpp
//---------------------------------------------------------------------------

/**
@file Billboard.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Billboard.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <OgreFontManager.h>
#include <OgreFont.h>
#include <OgreTextureManager.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>

using namespace Ogre;

namespace Graphics {
	CBillboard::CBillboard(const std::string& bbSetName, const Ogre::Vector3& position) : _bbSetName(bbSetName),
																					_position(position) {
		// Crea un billboard set con el nombre dado
		_billboardSet = _scene->getSceneMgr()->createBillboardSet(_bbSetName);
	} // CEntity

	//________________________________________________________________________

	CBillboard::~CBillboard() {
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");

		// Supuestamente no tenemos que eliminar nada ya que lo único que tenemos son
		// los punteros a los billboards que en teoria Ogre se encarga de eliminar
		// en su debido momento. Por lo tanto lo único que hacemos es setear nuestros
		// punteros como nulos.
		_billboardSet = NULL;
		_attachedBillboards.clear();
	} // ~CEntity
	
	//________________________________________________________________________
		
	bool CBillboard::attachToScene(CScene* scene) {
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no está cargada forzamos su carga.
		if (!_loaded) {
			_scene = scene;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;
	} // attachToScene
	
	//________________________________________________________________________
		
	bool CBillboard::deattachFromScene() {
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad está cargada forzamos su descarga.
		else {
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = NULL;
		}

		return true;
	} // deattachFromScene
	
	//________________________________________________________________________
		
	bool CBillboard::load() {
		try {
			// Creamos un nuevo nodo de escena del que va a colgar el billboard set
			// Es necesario que hagamos esto porque necesitamos que los billboards sean
			// entidades móviles
			_billboardNode = static_cast<Ogre::SceneNode*>( _scene->getSceneMgr()->getRootSceneNode()->createChild(_position) );
		}
		catch(std::exception e) {
			return false;
		}

		// Atachamos el billboard set al nodo creado en la escena
		_billboardNode->attachObject(_billboardSet);
		
		// Indicamos que el billboard ha sido cargado
		_loaded = true;

		return true;

	} // load
	
	//________________________________________________________________________
		
	void CBillboard::unload() {
		if(_billboardNode) {
			// desacoplamos la entidad de su nodo
			_billboardNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_billboardNode);
			_billboardNode = NULL;
		}
		if(_billboardSet) {
			_scene->getSceneMgr()->destroyBillboardSet(_billboardSet);
			_billboardSet = NULL;
		}

	} // load

	//________________________________________________________________________
		
	void CBillboard::tick(float secs) {
	} // tick
	
	//________________________________________________________________________
		
	void CBillboard::setVisible(bool visible) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setVisible(visible);

	} // setVisible
	
	//________________________________________________________________________
		
	const bool CBillboard::getVisible() {
		if(_billboardNode)
			return _billboardSet->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//________________________________________________________________________
		
	void CBillboard::setPosition(const Ogre::Vector3 &position) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setPosition(position);

	} // setPosition
	
	//________________________________________________________________________
		
	void CBillboard::setScale(const Ogre::Vector3 &scale) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setScale(scale);

	} // setScale
	
	//________________________________________________________________________
		
	void CBillboard::setScale(const float scale) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode) {
			Ogre::Vector3 scaleVector(scale, scale, scale);
			_billboardNode->setScale(scaleVector);
		}

	} // setScale

	//________________________________________________________________________

	void CBillboard::addMaterial(const std::string& materialName, const Ogre::Vector3& position, float width, float height) {
	}

	//________________________________________________________________________

	void CBillboard::addText(const std::string& text, const Ogre::Vector3& position, const ColourValue& color, const std::string& fontName, float width, float height) {
		// Creamos un billboard en la posicion dada
		Ogre::Billboard* billboardText = _billboardSet->createBillboard( position );
		// Si nos han pasado una dimensiones validas, seteamos las dimensiones del texto
		if(width != 0 && height != 0) billboardText->setDimensions(width, height);




		////////////////////////////////////////////////////////////////////////
		// Escribimos el texto en una textura que luego usaremos en el billboard
		////////////////////////////////////////////////////////////////////////

		// Obtenemos la textura que usaremos de background, en nuestro caso solo necesitamos que sea transparente
		//Texture* background = static_cast<Ogre::Texture*>( TextureManager::getSingletonPtr()->getByName("Background.png").getPointer() );
		
		// Obtenemos la fuente que vamos a usar para escribir en la textura
		Font* font = static_cast<Ogre::Font*>( FontManager::getSingleton().getByName(fontName).getPointer() );
 
		// Make sure the texture is not WRITE_ONLY, we need to read the buffer to do the blending with the font (get the alpha for example)
		// Creamos la textura sobre la que vamos a escribir el texto
		Texture* texture = TextureManager::getSingleton().createManual("Write Texture", "General", TEX_TYPE_2D, width, height, MIP_UNLIMITED , 
																		PF_X8R8G8B8, Ogre::TU_STATIC|Ogre::TU_AUTOMIPMAP).getPointer();
		// Seteamos el background de la textura
		//texture->getBuffer()->blit(background->getBuffer());
 
		// Escribimos el texto en la textura que hemos creado con las dimensiones, el color y la fuente dadas.
		WriteToTexture(text, texture, Image::Box(25,275,370,500), font, color, 'c');
	}

	//________________________________________________________________________

	void CBillboard::WriteToTexture(const String& str, Texture* destTexture, Image::Box destRectangle, Font* font, const ColourValue& color, char justify,  bool wordwrap) {
 
		if (destTexture->getHeight() < destRectangle.bottom)
			destRectangle.bottom = destTexture->getHeight();
		if (destTexture->getWidth() < destRectangle.right)
			destRectangle.right = destTexture->getWidth();
 
		if (!font->isLoaded())
			font->load();
 
		TexturePtr fontTexture = (TexturePtr) TextureManager::getSingleton().getByName(font->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName());
 
		HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
		HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();
 
		PixelBox destPb = destBuffer->lock(destRectangle,HardwareBuffer::HBL_NORMAL);
 
		// The font texture buffer was created write only...so we cannot read it back :o). One solution is to copy the buffer  instead of locking it. (Maybe there is a way to create a font texture which is not write_only ?)
 
		// create a buffer
		size_t nBuffSize = fontBuffer->getSizeInBytes();
		uint8* buffer = (uint8*)calloc(nBuffSize, sizeof(uint8)); 
 
		// create pixel box using the copy of the buffer
		PixelBox fontPb(fontBuffer->getWidth(), fontBuffer->getHeight(),fontBuffer->getDepth(), fontBuffer->getFormat(), buffer);          
		fontBuffer->blitToMemory(fontPb);
 
		uint8* fontData = static_cast<uint8*>( fontPb.data );
		uint8* destData = static_cast<uint8*>( destPb.data );
 
		const size_t fontPixelSize = PixelUtil::getNumElemBytes(fontPb.format);
		const size_t destPixelSize = PixelUtil::getNumElemBytes(destPb.format);
 
		const size_t fontRowPitchBytes = fontPb.rowPitch * fontPixelSize;
		const size_t destRowPitchBytes = destPb.rowPitch * destPixelSize;
 
		Box *GlyphTexCoords;
		GlyphTexCoords = new Box[str.size()];
 
		Font::UVRect glypheTexRect;
		size_t charheight = 0;
		size_t charwidth = 0;
 
		for(unsigned int i = 0; i < str.size(); i++) {
			if ((str[i] != '\t') && (str[i] != '\n') && (str[i] != ' ')) {
				glypheTexRect = font->getGlyphTexCoords(str[i]);
				GlyphTexCoords[i].left = glypheTexRect.left * fontTexture->getSrcWidth();
				GlyphTexCoords[i].top = glypheTexRect.top * fontTexture->getSrcHeight();
				GlyphTexCoords[i].right = glypheTexRect.right * fontTexture->getSrcWidth();
				GlyphTexCoords[i].bottom = glypheTexRect.bottom * fontTexture->getSrcHeight();
 
				if (GlyphTexCoords[i].getHeight() > charheight)
					charheight = GlyphTexCoords[i].getHeight();
				if (GlyphTexCoords[i].getWidth() > charwidth)
					charwidth = GlyphTexCoords[i].getWidth();
			}
 
		}    
 
		size_t cursorX = 0;
		size_t cursorY = 0;
		size_t lineend = destRectangle.getWidth();
		bool carriagreturn = true;
		for (unsigned int strindex = 0; strindex < str.size(); ++strindex) {
			switch(str[strindex]) {
			case ' ': cursorX += charwidth;  break;
			case '\t':cursorX += charwidth * 3; break;
			case '\n':cursorY += charheight; carriagreturn = true; break;
			default:
			{
				//wrapping
				if ((cursorX + GlyphTexCoords[strindex].getWidth()> lineend) && !carriagreturn ) {
					cursorY += charheight;
					carriagreturn = true;
				}
 
				//justify
				if (carriagreturn) {
					size_t l = strindex;
					size_t textwidth = 0;    
					size_t wordwidth = 0;
 
					while( (l < str.size() ) && (str[l] != '\n)')) {        
						wordwidth = 0;
 
						switch (str[l]) {
						case ' ': wordwidth = charwidth; ++l; break;
						case '\t': wordwidth = charwidth *3; ++l; break;
						case '\n': l = str.size();
						}
 
						if (wordwrap) {
							while((l < str.size()) && (str[l] != ' ') && (str[l] != '\t') && (str[l] != '\n')) {
								wordwidth += GlyphTexCoords[l].getWidth();
								++l;
							}
						}
						else {
							wordwidth += GlyphTexCoords[l].getWidth();
							l++;
						}
 
						if ((textwidth + wordwidth) <= destRectangle.getWidth())
							textwidth += (wordwidth);
						else
							break;
					}
 
					if ((textwidth == 0) && (wordwidth > destRectangle.getWidth()))
						textwidth = destRectangle.getWidth();
 
					switch (justify) {
					case 'c':    cursorX = (destRectangle.getWidth() - textwidth)/2;
							lineend = destRectangle.getWidth() - cursorX;
							break;
 
					case 'r':    cursorX = (destRectangle.getWidth() - textwidth);
							lineend = destRectangle.getWidth();
							break;
 
					default:    cursorX = 0;
							lineend = textwidth;
							break;
					}
 
					carriagreturn = false;
				}
 
				//abort - net enough space to draw
				if ((cursorY + charheight) > destRectangle.getHeight())
					goto stop;
 
				//draw pixel by pixel
				for (size_t i = 0; i < GlyphTexCoords[strindex].getHeight(); ++i ) {
					for (size_t j = 0; j < GlyphTexCoords[strindex].getWidth(); ++j) {
						float alpha =  color.a * (fontData[(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize +1 ] / 255.0);
						float invalpha = 1.0 - alpha;
						size_t offset = (i + cursorY) * destRowPitchBytes + (j + cursorX) * destPixelSize;
						ColourValue pix;
						PixelUtil::unpackColour(&pix,destPb.format,&destData[offset]);
						pix = (pix * invalpha) + (color * alpha);
						PixelUtil::packColour(pix,destPb.format,&destData[offset]);
					}
				}
 
				cursorX += GlyphTexCoords[strindex].getWidth();
			}//default
		}//switch
	}//for
 
	stop:
		delete[] GlyphTexCoords;
 
		destBuffer->unlock();
 
		// Free the memory allocated for the buffer
		free(buffer); buffer = 0;
	}

} // namespace Graphics
