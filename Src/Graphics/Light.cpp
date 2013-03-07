#include "Light.h"
#include "Scene.h"
#include "Server.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>


namespace Graphics{

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createSpotlLight(std::string name, Vector3 position, Vector3 direction){
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_SPOTLIGHT);
		_light->setDirection(direction);

		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createDirectionalLight(std::string name, Vector3 position, Vector3 direction){
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_DIRECTIONAL);
		_light->setDirection(direction);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

		bool CLight::createPointLight(std::string name, Vector3 position){
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_POINT);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setCastShadows(bool enabled){
		_light->setCastShadows(enabled);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setColour(float r, float g, float b){
		_light->setDiffuseColour(r, g, b);
		int a = 0;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setSpecularColour(float r, float g, float b){
		_light->setSpecularColour(r, g, b);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	CLight::~CLight(){
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyLight(_light);
		_light = 0;
	}
}