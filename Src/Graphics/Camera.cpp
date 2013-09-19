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
		_camera->setNearClipDistance(1);
		_camera->setFarClipDistance(1500);

		_defaultFov = (Math::PI/2)*0.89;

		_camera->setFOVy(Ogre::Radian(_defaultFov));
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
		//_camera->setOrientation(orientation);
		
		_cameraNode->setOrientation(orientation);
	}
	//--------------------------------------------------------

	void CCamera::rollCamera(float fRadian){
		//_camera->roll(Ogre::Radian(fRadian));
		_cameraNode->roll(Ogre::Radian(fRadian));
	}

	//--------------------------------------------------------
	
	void CCamera::lookAt(const Vector3& position){
		_camera->lookAt(position);
	}

	//--------------------------------------------------------

	void CCamera::restartCamera(){
		_camera->setOrientation(Quaternion::IDENTITY);
	}

	//--------------------------------------------------------

	Graphics::CEntity* CCamera::addEntityChild(const std::string &nameEntity, const std::string &nameMesh, Vector3 position){
	
		// creo escena para la entidad nueva
		Ogre::SceneNode* sceneNode;
		sceneNode = new Ogre::SceneNode(_scene->getSceneMgr(), "_SceneCamera_"+nameEntity);

		// creo la entidad nueva
		Ogre::Entity *entity;
		entity = _scene->getSceneMgr()->createEntity(nameMesh);

		
		// atacho la entidad a la escena recien creada
		sceneNode->attachObject(entity);
		
		entity->setRenderQueueGroupAndPriority(Ogre::RenderQueueGroupID::RENDER_QUEUE_6, 200);

		//entity->getSubEntity(0)->getMaterial()->setDepthCheckEnabled(false); 
		//entity->getSubEntity(0)->getMaterial()->setDepthWriteEnabled(false);

		// especifico su posicion
		sceneNode->setPosition(position);

		// añado la escena como hija de la camara
		_cameraNode->addChild(sceneNode);

		// Esto es una pequeña ñapa, me creo un entidad grafica pero sin inicializar, y le añado una escena ahierro
		// Hago esto para que se pueda desplazar desde la logica sin ningun problema.
		Graphics::CEntity *entityTemp = new CEntity(nameEntity, nameMesh);
		entityTemp->setOgreEntity(entity);
		entityTemp->setSceneNode(sceneNode);
		entityTemp->setScene(_scene);

		return entityTemp;
	}
	
	//--------------------------------------------------------

	void CCamera::setFov(float radianAngle){
		_camera->setFOVy(Ogre::Radian(radianAngle));
	}

	void CCamera::resetFov(){
		_camera->setFOVy(Ogre::Radian(_defaultFov));
	}

} // namespace Graphics
