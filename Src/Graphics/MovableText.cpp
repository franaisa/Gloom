//---------------------------------------------------------------------------
// MovableText.cpp
//---------------------------------------------------------------------------

/**
@file MovableText.cpp

Contiene la implementación de la clase que permite renderizar texto 2d movible.

@see Graphics::CMovableText

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "MovableText.h"
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
	CMovableText::CMovableText(const std::string& bbSetName, const Ogre::Vector3& position) : _bbSetName(bbSetName),
																					_position(position) {
		// Crea un billboard set con el nombre dado
		_billboardSet = _scene->getSceneMgr()->createBillboardSet(_bbSetName);
	} // CEntity

	//________________________________________________________________________

	CMovableText::~CMovableText() {
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");

		// Supuestamente no tenemos que eliminar nada ya que lo único que tenemos son
		// los punteros a los billboards que en teoria Ogre se encarga de eliminar
		// en su debido momento. Por lo tanto lo único que hacemos es setear nuestros
		// punteros como nulos.
		_billboardSet = NULL;
		_attachedBillboards.clear();
	} // ~CEntity
	
	//________________________________________________________________________
		
	bool CMovableText::attachToScene(CScene* scene) {
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
		
	bool CMovableText::deattachFromScene() {
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
		
	bool CMovableText::load() {
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
		
	void CMovableText::unload() {
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
		
	void CMovableText::tick(float secs) {
	} // tick
	
	//________________________________________________________________________
		
	void CMovableText::setVisible(bool visible) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setVisible(visible);

	} // setVisible
	
	//________________________________________________________________________
		
	const bool CMovableText::getVisible() {
		if(_billboardNode)
			return _billboardSet->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//________________________________________________________________________
		
	void CMovableText::setPosition(const Ogre::Vector3 &position) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setPosition(position);

	} // setPosition
	
	//________________________________________________________________________
		
	void CMovableText::setScale(const Ogre::Vector3 &scale) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode)
			_billboardNode->setScale(scale);

	} // setScale
	
	//________________________________________________________________________
		
	void CMovableText::setScale(const float scale) {
		assert(_billboardNode && "La entidad no ha sido cargada");
		if(_billboardNode) {
			Ogre::Vector3 scaleVector(scale, scale, scale);
			_billboardNode->setScale(scaleVector);
		}

	} // setScale

	//________________________________________________________________________

	void CMovableText::setText(const std::string& text, const Ogre::Vector3& position, const ColourValue& color, const std::string& fontName, float width, float height) {
	}

} // namespace Graphics
