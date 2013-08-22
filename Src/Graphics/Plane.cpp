#include "Plane.h"
#include "Scene.h"
#include "Server.h"
#include "Entity.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics {

	//________________________________________________________________________

	CPlane::CPlane(CScene* scene, const std::string& name, const Vector3& position, const Vector3& orientation, 
				   float width, float height, unsigned int xSegments, unsigned int ySegments, float uTiling, 
				   float vTiling, const Vector3& upVector, const std::string& materialName, unsigned int lightMask, 
				   bool castShadows) {

		// Sacamos la escena de Ogre
		Ogre::SceneManager* sceneMgr = scene->getSceneMgr();

		// Construimos un plano con la orientacion dada
		Ogre::Plane plane(orientation, 0);
		// Una vez tenemos el plano, creamos el mesh
		Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
													  plane, width, height, xSegments, ySegments, true, 1, uTiling, vTiling, upVector);

		// Le pedimos a Ogre que nos calcule las tangentes con mucho cariño si puede ser
		unsigned short src, dest;
		if (!planeMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest)) {
			planeMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		// Creamos el plano a partir del mesh generado
		_entity = sceneMgr->createEntity(planeMesh);
		_sceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		_sceneNode->attachObject(_entity);
		_sceneNode->setPosition(position);
		
		// Asignamos la mascara de luces
		_entity->setLightMask(lightMask);
		// Asignamos el material dado
		_entity->setMaterialName(materialName);
		// Indicamos si el plano va a producir sombras
		_entity->setCastShadows(castShadows);
	}

	//________________________________________________________________________

	CPlane::~CPlane() {
		// Nada que hacer
	}

}