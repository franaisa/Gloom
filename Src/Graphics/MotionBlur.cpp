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
	
	CMotionBlur::CMotionBlur(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera) : _previousCameraOrientation(Quaternion::IDENTITY),
																									  _sceneCamera( camera->getOgreCamera() ) {
		_sceneCamera = camera->getOgreCamera();
		_cameraNode = camera->getSceneNode();

		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _sceneCamera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "MotionBlurFranCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "MotionBlurFranCompositor", true);
		
		_compositor->addListener(this);
	}

	//________________________________________________________________________

	CMotionBlur::~CMotionBlur() {
		// Nada que hacer
		_compositor->removeListener(this);
	}

	//________________________________________________________________________

	void CMotionBlur::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat) {
		// Obtenemos el puntero al gestor de parametros del fragment shader
		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		// Construimos una matriz de vista-proyeccion que no tenga en cuenta la posicion del frame
		// anterior, solo la rotacion. De esta forma el blur solo afecta a la rotacion de camara.
		Matrix4 motionCorrectedMatrix = _sceneCamera->getProjectionMatrix() * Ogre::Math::makeViewMatrix(_sceneCamera->getDerivedPosition(), _previousCameraOrientation);

		// Pasamos la matriz de vista-proyeccion del frame anterior corregida para tener en
		// cuenta solo las rotaciones de camara
		fpParams->setNamedConstant("previousViewProjMatrix", motionCorrectedMatrix);
		_previousCameraOrientation	= _sceneCamera->getDerivedOrientation();

		// Calculamos la matriz vista-proyeccion para el siguiente frame y pasamos al 
		// shader la inversa de la matriz actual de vista-proyeccion
		Matrix4 projectionMatrix	= _sceneCamera->getProjectionMatrix();
		Matrix4 viewMatrix			= _sceneCamera->getViewMatrix();
		Matrix4 viewProjMatrix		= projectionMatrix * viewMatrix;

		// Pasamos la inversa de la matriz vista-proyeccion
		fpParams->setNamedConstant( "inverseViewProjMatrix", viewProjMatrix.inverse() );

		// Calculamos el tiempo transcurrido para renderizar la escena en este frame
		float deltaTime = 1.0f / BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getStatistics().lastFPS;
		fpParams->setNamedConstant("deltaTime", deltaTime);
	}
}
