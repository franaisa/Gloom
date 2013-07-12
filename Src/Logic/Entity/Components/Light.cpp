/**
@file Light.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#include "Light.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Light.h"
#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include <OgreSceneManager.h>
#include "Logic/Server.h"

#include <OgreMeshManager.h>
#include <OgreMaterial.h>
#include <OgreMesh.h>
#include <OgreEntity.h>

using namespace Ogre;

namespace Logic {

	IMP_FACTORY(CLight);

	CLight::CLight() {
		// Nada que hacer
	}

	//---------------------------------------------------------

	CLight::~CLight() {

	}
	
	//---------------------------------------------------------
	
	bool CLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Nos aseguramos de que haya una posicion que asignar a la luz
		assert( entityInfo->hasAttribute("position") );

		_light.createPointLight(Graphics::CServer::getSingletonPtr()->getActiveScene(), "luzTest", entityInfo->getVector3Attribute("position") );

		//_light.setposition( entityInfo->getVector3Attribute("position") );

		if( entityInfo->hasAttribute("color") ) {
			Ogre::Vector3 color = entityInfo->getVector3Attribute("color");
			_light.setColour(color.x, color.y, color.z);
		}

		if( entityInfo->hasAttribute("attenuation") ) {
			Ogre::Vector3 attenuation = entityInfo->getVector3Attribute("attenuation");
			// Como de momento no contemplamos el rango de alcance en las luces
			// lo fijamos a un valor arbitrario
			_light.setAttenuation(50.0f, attenuation.x, attenuation.y, attenuation.z);
		}

		/*Vector3 direction;
		
		if(entityInfo->hasAttribute("direction"))
			direction = entityInfo->getVector3Attribute("direction");

		std::string type = entityInfo->getStringAttribute("type");
		if(type == "SpotLight"){
			_light.createSpotlLight(_entity->getMap()->getScene(), _entity->getName(), position, direction);
			if(entityInfo->hasAttribute("innerAngle") && entityInfo->hasAttribute("outerAngle"))
				_light->setRange(entityInfo->getFloatAttribute("innerAngle"), entityInfo->getFloatAttribute("outerAngle") );
		}
		if(type == "DirectionalLight")
			_light->createDirectionalLight(_entity->getMap()->getScene(), _entity->getName(), position, direction);
		if(type == "PointLight")
			_light->createPointLight(_entity->getMap()->getScene(),_entity->getName(), position);

		if(entityInfo->hasAttribute("castShadows"))
			_light->setCastShadows(entityInfo->getBoolAttribute("castShadows"));

		if(entityInfo->hasAttribute("colour")){
			Vector3 colour = entityInfo->getVector3Attribute("colour");
			_light->setColour(colour.x, colour.y, colour.z);
		}
		
		if(entityInfo->hasAttribute("specularColour")){
			Vector3 specularColour = entityInfo->getVector3Attribute("specularColour");
			_light->setSpecularColour(specularColour.x, specularColour.y, specularColour.z);
		}

		if(entityInfo->hasAttribute("intensity"))
			_light->setIntensity(entityInfo->getFloatAttribute("intensity"));
		*/

		//_light->attachToScene();

		return true;

	} // spawn

	void CLight::onStart() {
		/*MeshPtr pMesh = MeshManager::getSingleton().load("agent47.mesh",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    
        HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY,
        HardwareBuffer::HBU_STATIC_WRITE_ONLY,
        true, true);
		// so we can still read it
 
		// build tangent vectors, all our meshes use only one texture coordset 
		// Note: we can build into VES_TANGENT now (SM2+)
 
		unsigned short src, dest;   
		if (!pMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)) {
			pMesh->buildTangentVectors(VES_TANGENT, src, dest);
		}

		// Creamos un objeto 3d
		Entity* hitman = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createEntity(pMesh);
		//Entity* hitman = mSceneMgr->createEntity("hitman", "agent47.mesh");
		hitman->setMaterialName("pointLightTest"); // asignamos el material

		SceneNode* hitmanNode = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
		hitmanNode->attachObject(hitman);
		hitmanNode->setPosition(0, 0, 0);*/
	}

} // namespace Logic


