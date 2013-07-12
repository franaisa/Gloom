#include "Light.h"
#include "Scene.h"
#include "Server.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics {

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createSpotlLight(CScene* scene, std::string name, Vector3 position, Vector3 direction){
		_light = scene->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_SPOTLIGHT);
		_light->setDirection(direction);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CLight::createDirectionalLight(CScene* scene, std::string name, Vector3 position, Vector3 direction){
		_light = scene->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(Ogre::Light::LT_DIRECTIONAL);
		_light->setDirection(direction);
		return true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////


	bool CLight::createPointLight(CScene* scene, std::string name, Vector3 position){
		_light = scene->getSceneMgr()->createLight(name);
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

	void CLight::setAttenuation(float Range, float Kc, float Kl, float Kq) {
		_light->setAttenuation(Range, Kc, Kl, Kq);
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