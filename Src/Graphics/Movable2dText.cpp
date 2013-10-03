
#include "Movable2dText.h"
#include "Scene.h"
#include "Server.h"
#include "Entity.h"

#include <OgreSceneManager.h>
#include <OgreFontManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

using namespace Ogre;

namespace Graphics {

	//________________________________________________________________________

	CMovable2dText::CMovable2dText() {
		// Nada que hacer
	}

	//________________________________________________________________________

	CMovable2dText::~CMovable2dText() {
		// Nada que hacer
	}

	//________________________________________________________________________

	void CMovable2dText::load(CScene* scene, CEntity* parent, const Ogre::Vector3& position, const std::string& text, 
							  const std::string& textFont, const Ogre::Vector4& color, float textSize) {

		std::string textMaterial = text + "_textMaterial";
		std::string writeTexture = text + "_writeTexture";
		
		// Cargamos la textura de fondo (una imagen transparente)
		Image alphaImg;
		alphaImg.load("alphaImg.png", "General");
		TexturePtr background = TextureManager::getSingleton().createManual("alphaImg", "General", TEX_TYPE_2D, alphaImg.getWidth(), alphaImg.getHeight(), 5, PF_A8R8G8B8);
		background->getBuffer(0, 0)->blitFromMemory( alphaImg.getPixelBox(0, 0) );
		
		// Cargamos la fuente dada
		FontPtr font = FontManager::getSingleton().getByName(textFont);
		//font->setTrueTypeSize();
		// Escribimos sobre la textura en la que vamos a pegar la fuente
		// el fondo que hemos generado previamente
		TexturePtr texture = TextureManager::getSingleton().createManual(writeTexture, "General", TEX_TYPE_2D, 512, 512, MIP_UNLIMITED , PF_A8R8G8B8, Ogre::TU_STATIC | Ogre::TU_AUTOMIPMAP);
		texture->getBuffer()->blit( background->getBuffer() );
		// Escribimos sobre la textura el texto dado por parametro
		ColourValue ogreColor(color.x, color.y, color.z, color.w);
		writeToTexture(text, texture, Image::Box(25, 275, 370, 500), font, ogreColor, 'c');

		// Volvemos a renderizar sobre una textura el texto, pero esta vez para asegurarnos
		// que la fase de oclusion del light scatter no afecta al billboard
		TexturePtr blackTexture = TextureManager::getSingleton().createManual(writeTexture, "General", TEX_TYPE_2D, 512, 512, MIP_UNLIMITED , PF_A8R8G8B8, Ogre::TU_STATIC | Ogre::TU_AUTOMIPMAP);
		blackTexture->getBuffer()->blit( background->getBuffer() );
		// Escribimos sobre la textura el texto dado por parametro
		writeToTexture(text, blackTexture, Image::Box(25, 275, 370, 500), font, ColourValue::Black, 'c');

		// Creamos un material a partir de la textura generada
		// para el billboard que vamos a crear
		MaterialPtr m_QuadMaterial = Ogre::MaterialManager::getSingleton().create( ( textMaterial ), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false );

		Ogre::Pass *pass = m_QuadMaterial->getTechnique(0)->getPass(0);
		pass->setLightingEnabled( false );
		pass->setDepthCheckEnabled( true );
		pass->setDepthWriteEnabled( false );
		pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

		Ogre::TextureUnitState *tex = pass->createTextureUnitState( "MyCustomState", 0 );
		tex->setTexture(texture);
		tex->setTextureFiltering( Ogre::TFO_ANISOTROPIC );
		tex->setTextureAnisotropy( 8 );

		// Creamos una nueva tecnica para aplicar la oclusion del light scatter.
		// En este caso lo unico que hacemos es pintar el texto de negro.
		Ogre::Technique* technique = m_QuadMaterial->createTechnique();
		technique->setSchemeName("blackScheme");

		pass = technique->createPass();
		pass->setLightingEnabled(false);
		pass->setDepthCheckEnabled( true );
		pass->setDepthWriteEnabled( false );
		pass->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

		tex = pass->createTextureUnitState( "BlackText", 0 );
		tex->setTexture(blackTexture);
		tex->setTextureFiltering( Ogre::TFO_ANISOTROPIC );
		tex->setTextureAnisotropy( 8 );

		m_QuadMaterial->load();

		// Cargamos sobre el billboard el nuevo material generado con el texto
		CBillboard::load( scene, parent, textMaterial, position, Ogre::Vector2(textSize, textSize) );
	}

	//________________________________________________________________________

	void CMovable2dText::setVisible(bool isVisible) {
		CBillboard::setVisible(isVisible);
	}

	//________________________________________________________________________

	bool CMovable2dText::isVisible() {
		return CBillboard::isVisible();
	}

	//________________________________________________________________________

	void CMovable2dText::setPosition(const Ogre::Vector3& position) {
		CBillboard::setPosition(position);
	}

	//________________________________________________________________________

	void CMovable2dText::writeToTexture(const String &str, TexturePtr destTexture, Image::Box destRectangle, FontPtr font, 
						const ColourValue &color, char justify,  bool wordwrap) {

		if (destTexture->getHeight() < destRectangle.bottom)
			destRectangle.bottom = destTexture->getHeight();
		if (destTexture->getWidth() < destRectangle.right)
			destRectangle.right = destTexture->getWidth();
 
		if (!font->isLoaded())
			font->load();
 
		TexturePtr fontTexture = (TexturePtr) TextureManager::getSingleton().getByName(
			font->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName());
 
		HardwarePixelBufferSharedPtr fontBuffer = fontTexture->getBuffer();
		HardwarePixelBufferSharedPtr destBuffer = destTexture->getBuffer();
 
		PixelBox destPb = destBuffer->lock(destRectangle,HardwareBuffer::HBL_NORMAL);
 
		// The font texture buffer was created write only...so we cannot read it back :o). 
		// One solution is to copy the buffer  instead of locking it. (Maybe there is a way 
		// to create a font texture which is not write_only ?)
 
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
		for (unsigned int strindex = 0; strindex < str.size(); strindex++) {
			switch(str[strindex]) {
			case ' ': cursorX += charwidth;  break;
			case '\t':cursorX += charwidth * 3; break;
			case '\n':cursorY += charheight; carriagreturn = true; break;
			default:
			{
					//wrapping
					if ((cursorX + GlyphTexCoords[strindex].getWidth()> lineend) && !carriagreturn )
					{
						cursorY += charheight;
						carriagreturn = true;
					}
 
					//justify
					if (carriagreturn)
					{
						size_t l = strindex;
						size_t textwidth = 0;    
						size_t wordwidth = 0;
 
						while( (l < str.size() ) && (str[l] != '\n)'))
						{        
							wordwidth = 0;
 
							switch (str[l])
							{
							case ' ': wordwidth = charwidth; ++l; break;
							case '\t': wordwidth = charwidth *3; ++l; break;
							case '\n': l = str.size();
							}
 
							if (wordwrap)
								while((l < str.size()) && (str[l] != ' ') && (str[l] != '\t') && (str[l] != '\n'))
								{
									wordwidth += GlyphTexCoords[l].getWidth();
									++l;
								}
							else
								{
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
 
						switch (justify)
						{
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
					for (size_t i = 0; i < GlyphTexCoords[strindex].getHeight(); i++ )
						for (size_t j = 0; j < GlyphTexCoords[strindex].getWidth(); j++)
						{
							 float alpha =  color.a * (fontData[(i + GlyphTexCoords[strindex].top) * fontRowPitchBytes + (j + GlyphTexCoords[strindex].left) * fontPixelSize +1 ] / 255.0);
							 float invalpha = 1.0 - alpha;
							 size_t offset = (i + cursorY) * destRowPitchBytes + (j + cursorX) * destPixelSize;
							  ColourValue pix;
							 PixelUtil::unpackColour(&pix,destPb.format,&destData[offset]);
							 pix = (pix * invalpha) + (color * alpha);
							 PixelUtil::packColour(pix,destPb.format,&destData[offset]);
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

}