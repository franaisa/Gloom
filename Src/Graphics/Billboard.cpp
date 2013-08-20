#include "Billboard.h"
#include "Scene.h"
#include "Server.h"
#include "Entity.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

using namespace Ogre;

namespace Graphics {

	//________________________________________________________________________

	CBillboard::CBillboard() {
		// Nada que hacer
	}

	//________________________________________________________________________

	CBillboard::~CBillboard() {
		// Nada que hacer
	}

	//________________________________________________________________________

	void CBillboard::load(CScene* scene, CEntity* parent, const std::string& materialName, const Ogre::Vector3& position, const Ogre::Vector2& defaultDimensions) {
		// Obtenemos la escena a la que vamos a atachar el billboard
		SceneManager* sceneMgr = scene->getSceneMgr();

		// Creamos el set y le asignamos un material
		_billboardSet = sceneMgr->createBillboardSet();
		_billboardSet->setMaterialName(materialName);
		
		// Si existe una entidad grafica atachamos el billboard a su nodo de escena,
		// en caso contrario creamos un nodo nuevo y tomamos la posicion dada como
		// global.
		if(parent != NULL) {
			_sceneNode = parent->getSceneNode();
			_billboard = _billboardSet->createBillboard(position);
		}
		else {
			_sceneNode = static_cast<SceneNode*>( sceneMgr->getRootSceneNode()->createChild() );
			_billboard = _billboardSet->createBillboard(Ogre::Vector3::ZERO);
			_sceneNode->setPosition(position);
		}

		if(defaultDimensions != Ogre::Vector2::ZERO)
			_billboardSet->setDefaultDimensions(defaultDimensions.x, defaultDimensions.y);

		// Atachamos el billboard al nodo de escena
		_sceneNode->attachObject(_billboardSet);
	}

	//________________________________________________________________________

	void CBillboard::setVisible(bool isVisible) {
		_billboardSet->setVisible(isVisible);
	}

	//________________________________________________________________________

	bool CBillboard::isVisible() {
		return _billboardSet->isVisible();
	}

	//________________________________________________________________________

	void CBillboard::setPosition(const Ogre::Vector3& position) {
		_sceneNode->setPosition(position);
	}
}