#include "Light.h"
#include "Scene.h"
#include "Server.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>


namespace Graphics{

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createSpotlLight(std::string name, Vector3 position, Vector3 direction){
		if(_node)
			return false;
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_SPOTLIGHT);
		_light->setDirection(direction);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createDirectionalLight(std::string name, Vector3 position, Vector3 direction){
		if(_node)
			return false;
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_DIRECTIONAL);
		_light->setDirection(direction);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CLight::createPointLight(CScene* scene, std::string name, Vector3 position){
		if(_node)
			return false;
		_light = scene->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_POINT);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::destroyLight(){		
		_node->detachObject(_light);
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroySceneNode(_node);
		_node = 0;
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyLight(_light);
		_light = 0;
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::attachToScene(){
		_node = static_cast<Ogre::SceneNode*>(CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->getRootSceneNode()->createChild());
		//_node = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createSceneNode(_light->getName());
		_node->attachObject(_light);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setCastShadows(bool enabled){
		_light->setCastShadows(enabled);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setColour(float r, float g, float b){
		_light->setDiffuseColour(r, g, b);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setSpecularColour(float r, float g, float b){
		_light->setSpecularColour(r, g, b);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	CLight::~CLight(){
		_node->detachObject(_light);
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroySceneNode(_node);
		_node = 0;
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyLight(_light);
		_light = 0;
	}
}