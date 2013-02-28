#include "Light.h"
#include "Scene.h"
#include "Server.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

/*
namespace Graphics{


	bool CLight::createLight(std::string name, LightType type, Vector3 position){
		if(_node)
			return false;
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(name);
		_light->setPosition(position);
		_light->setType(type);
		return true;
	}

	bool CLight::destroyLight(){
		_node->detachObject(_light);
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroySceneNode(_node);
		_node = 0;
		CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyLight(_light);
		return true;
	}

	void CLight::attachToScene(CScene *scene){
		_node = static_cast<Ogre::SceneNode*>(CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->getRootSceneNode()->createChild());
		_node->attachObject(_light);
	}


}*/