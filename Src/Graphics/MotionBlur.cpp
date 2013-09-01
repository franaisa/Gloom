//---------------------------------------------------------------------------
// MotionBlur.cpp
//---------------------------------------------------------------------------

/**
@file MotionBlur.cpp

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "MotionBlur.h"
#include "BaseSubsystems/Server.h"

#include <OgreCompositorManager.h>

#include "Server.h"
#include "Scene.h"
#include "Camera.h"

namespace Graphics {
	
	CMotionBlur::CMotionBlur(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera) : _previousViewProjMatrix(Matrix4::IDENTITY),
																									  _previousCameraPosition(Vector3::ZERO),
																									  _sceneCamera( camera->getOgreCamera() ) {
		_sceneCamera = camera->getOgreCamera();
		_cameraNode = camera->getSceneNode();

		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _sceneCamera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "MotionBlurFranCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "MotionBlurFranCompositor", true);

		/*_compositor = compositorManager->addCompositor(cameraViewport, "volumetricLightCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "volumetricLightCompositor", true);*/
		
		_compositor->addListener(this);
	}

	//________________________________________________________________________

	CMotionBlur::~CMotionBlur() {
		// Nada que hacer
		_compositor->removeListener(this);
	}

	//________________________________________________________________________

	void CMotionBlur::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat) {
		/*Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		//fpParams->setNamedConstant("previousViewProjMatrix", _previousViewProjMatrix);

		// Pasamos la matriz de vista y proyeccion
		Matrix4 projectionMatrix = _sceneCamera->getProjectionMatrix();
		Matrix4 viewMatrix = _sceneCamera->getViewMatrix();
		Matrix4 viewProjMatrix = projectionMatrix * viewMatrix;
		_previousViewProjMatrix = viewProjMatrix;
		fpParams->setNamedConstant("viewProjMatrix", viewProjMatrix);
		
		//fpParams->setNamedConstant("inverseViewProjMatrix", viewProjMatrix.inverse());

		// Pasamos la direccion de la luz --> {-52.0654, 60.1029, -108.08}
		Vector3 lightPosition(-52.0654f, 60.1029f, -108.08f);
		//fpParams->setNamedConstant("lightPosition", lightPosition);

		fpParams->setNamedConstant("currentCameraPos", _sceneCamera->getDerivedPosition());
		fpParams->setNamedConstant("previousCameraPos", _previousCameraPosition);

		_previousCameraPosition = _sceneCamera->getDerivedPosition();*/

		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		// Pasamos la matriz de vista-proyeccion del frame anterior
		fpParams->setNamedConstant("previousViewProjMatrix", _previousViewProjMatrix);

		// Calculamos la matriz vista-proyeccion para el siguiente frame y pasamos al 
		// shader la inversa de la matriz actual de vista-proyeccion
		Matrix4 projectionMatrix	= _sceneCamera->getProjectionMatrix();
		Matrix4 viewMatrix			= _sceneCamera->getViewMatrix();
		Matrix4 viewProjMatrix		= projectionMatrix * viewMatrix;
		_previousViewProjMatrix		= viewProjMatrix;

		// Pasamos la matriz actual de vista-proyeccion
		//fpParams->setNamedConstant("viewProjMatrix", viewProjMatrix);
		
		// Pasamos la inversa de la matriz vista-proyeccion
		fpParams->setNamedConstant("inverseViewProjMatrix", viewProjMatrix.inverse());

		// Calculamos el tiempo transcurrido para renderizar la escena en este frame
		float deltaTime = 1.0f / BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getStatistics().lastFPS;
		fpParams->setNamedConstant("deltaTime", deltaTime);

		// Pasamos las posiciones de camara actual y anterior
		/*Vector3 currentCameraPosition = _sceneCamera->getDerivedPosition();
		fpParams->setNamedConstant("currentCameraPosition", currentCameraPosition);
		fpParams->setNamedConstant("previousCameraPosition", _previousCameraPosition);

		// Nos quedamos con la posicion de este frame de la camara para comparar
		// con el frame siguiente
		_previousCameraPosition = currentCameraPosition;*/
	}
}
