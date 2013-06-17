//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#include "Camera.h"
#include "Scene.h"
#include "Entity.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>



namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;
		//Creacion del nodo que contiene a la camara
		_cameraNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
		//Creación de la camara
		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		//Seteo de atributos de la camara
		_camera->setNearClipDistance(2);
		_camera->setFarClipDistance(500);

		_camera->setFOVy(Ogre::Radian(70));
		//Atachamos la camara al nodo
		_cameraNode->attachObject(this->_camera);

	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);

	} // ~CCamera

	//--------------------------------------------------------
	
	const Vector3 &CCamera::getCameraPosition() 
	{
		return _cameraNode->getPosition();
	}

	//--------------------------------------------------------

	const Quaternion &CCamera::getCameraOrientation() 
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	void CCamera::setCameraPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setCameraDirection(const Vector3& direction) {
		_camera->setDirection(direction);
	}
	//--------------------------------------------------------

	const Vector3& CCamera::getCameraDirection() {
		return _camera->getRealDirection(); 

	}
	//--------------------------------------------------------

	const Quaternion& CCamera::getRealOrientation(){
		return _camera->getRealOrientation();
	}
	//--------------------------------------------------------

	void CCamera::setOrientation(const Quaternion& orientation){
		_camera->setOrientation(orientation);
	}
	//--------------------------------------------------------

	void CCamera::rollCamera(float fRadian){
		_camera->roll(Ogre::Radian(fRadian));
	}

	//--------------------------------------------------------
	
	void CCamera::lookAt(const Vector3& position){
		_camera->lookAt(position);
	}

	//--------------------------------------------------------


	Graphics::CEntity* CCamera::addEntityChild(const std::string &nameEntity, const std::string &nameMesh, Vector3 position){
	
		Ogre::SceneNode* sceneNode;
		sceneNode = new Ogre::SceneNode(_scene->getSceneMgr(), "_SceneCamera_"+nameEntity);

		Ogre::Entity *entity;
		entity = _scene->getSceneMgr()->createEntity(nameEntity, nameMesh);

		return 0;
	}
	
	//--------------------------------------------------------


} // namespace Graphics
