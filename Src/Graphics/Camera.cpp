//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
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
		
		// Create the camera's top node (which will only handle position).
		_cameraNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
 
		// Create the camera's yaw node as a child of camera's top node.
		_cameraYawNode = _cameraNode->createChildSceneNode();
 
		// Create the camera's pitch node as a child of camera's yaw node.
		_cameraPitchNode = _cameraYawNode->createChildSceneNode();
 
		// Create the camera's roll node as a child of camera's pitch node
		// and attach the camera to it.
		_cameraRollNode = _cameraPitchNode->createChildSceneNode();
		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		_camera->setNearClipDistance(2);
		_camera->setFarClipDistance(500);
		_camera->setFOVy(Ogre::Radian(70));
		_cameraRollNode->attachObject(this->_camera);



	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_cameraYawNode->detachAllObjects();
		_cameraPitchNode->detachAllObjects();
		_cameraRollNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraRollNode);
		_scene->getSceneMgr()->destroySceneNode(_cameraPitchNode);
		_scene->getSceneMgr()->destroySceneNode(_cameraYawNode);
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

	Ogre::Vector3 CCamera::getCameraDirection() {
		return _camera->getRealDirection(); 

	}
	//--------------------------------------------------------

	Ogre::Quaternion CCamera::getRealOrientation(){
		return _camera->getRealOrientation();
	}
	//--------------------------------------------------------

	void CCamera::setOrientation(Ogre::Quaternion orientation){
		_camera->setOrientation(orientation);
	}
	//--------------------------------------------------------

	void CCamera::rollCamera(float fRadian){
		Ogre::Radian rad(fRadian);
		_camera->roll(rad);
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

	void CCamera::moveCamera(Ogre::Radian mRotX, Ogre::Radian mRotY){
		 Ogre::Real pitchAngle;
		 Ogre::Real pitchAngleSign;
 
		 // Yaws the camera according to the mouse relative movement.
		 _cameraYawNode->yaw(mRotX);
 
		 // Pitches the camera according to the mouse relative movement.
		 _cameraPitchNode->pitch(mRotY);
		/*
		 // Translates the camera according to the translate vector which is
		 // controlled by the keyboard arrows.
		 //
		 // NOTE: We multiply the mTranslateVector by the cameraPitchNode's
		 // orientation quaternion and the cameraYawNode's orientation
		 // quaternion to translate the camera accoding to the camera's
		 // orientation around the Y-axis and the X-axis.
		 this->cameraNode->translate(this->cameraYawNode->getOrientation() *
									 this->cameraPitchNode->getOrientation() *
									 this->mTranslateVector,
									 Ogre::SceneNode::TS_LOCAL);*/
 
		 // Angle of rotation around the X-axis.
		 pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(_cameraPitchNode->getOrientation().w)).valueDegrees());
 
		 // Just to determine the sign of the angle we pick up above, the
		 // value itself does not interest us.
		 pitchAngleSign = _cameraPitchNode->getOrientation().x;
 
		 // Limit the pitch between -90 degress and +90 degrees, Quake3-style.
		 if (pitchAngle > 90.0f)
		 {
			 if (pitchAngleSign > 0)
				 // Set orientation to 90 degrees on X-axis.
				 _cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
																		Ogre::Math::Sqrt(0.5f), 0, 0));
			 else if (pitchAngleSign < 0)
				 // Sets orientation to -90 degrees on X-axis.
				 _cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
																		-Ogre::Math::Sqrt(0.5f), 0, 0));
		 }
		 
	}

	//--------------------------------------------------------

	void CCamera::setYaw(Ogre::Quaternion yaw){
		_cameraYawNode->setOrientation(yaw);
	}

} // namespace Graphics
