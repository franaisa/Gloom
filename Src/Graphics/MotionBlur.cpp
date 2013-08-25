//---------------------------------------------------------------------------
// MotionBlur.cpp
//---------------------------------------------------------------------------

/**
@file MotionBlur.cpp

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "MotionBlur.h"

#include <OgreCompositorManager.h>

#include "Server.h"
#include "Scene.h"
#include "Camera.h"

namespace Graphics {
	
	CMotionBlur::CMotionBlur(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera) : _previousViewProjMatrix(Matrix4::IDENTITY), 
																									  _previousFPS(0.0f),
																									  _sceneCamera( camera->getOgreCamera() ) {
		_sceneCamera = camera->getOgreCamera();
		_cameraNode = camera->getSceneNode();

		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _sceneCamera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "MotionBlurFranCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "MotionBlurFranCompositor", true);
		
		_compositor->addListener(this);
		_sceneCamera->addListener(this);
	}

	//________________________________________________________________________

	CMotionBlur::~CMotionBlur() {
		// Nada que hacer
		_compositor->removeListener(this);

		if(_sceneCamera != NULL)
			_sceneCamera->removeListener(this);
	}

	//________________________________________________________________________

	void CMotionBlur::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat) {
		// Actualizamos los parametros del shader antes de que sea renderizado
		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		fpParams->setNamedConstant("inverseViewProjMatrix", _inverseViewProjMatrix);
		fpParams->setNamedConstant("previousViewProjMatrix", _previousViewProjMatrix);
	}

	//________________________________________________________________________

	void CMotionBlur::cameraPreRenderScene(Ogre::Camera* camera) {
		// Antes de renderizar la escena, almacenamos la inversa de la matriz de vista
		// y proyeccion con la que se va a realizar el render
		Matrix4 projectionMatrix  = _sceneCamera->getProjectionMatrix();
		_inverseViewProjMatrix	  = (projectionMatrix * _sceneCamera->getViewMatrix()).inverse();
	}

	//________________________________________________________________________

	// No sirve porque al compositor se le llama despues de esto
	void CMotionBlur::cameraPostRenderScene(Ogre::Camera* camera) {
		// Despues de renderizar la escena nos quedamos con la matriz que de vista
		// y proyeccion con la que se renderizo la escena
		Matrix4 previousProjectionMatrix = _sceneCamera->getProjectionMatrix();
		Matrix4 previousViewMatrix = _sceneCamera->getViewMatrix();
		_previousViewProjMatrix = previousProjectionMatrix * previousViewMatrix;
	}

	//________________________________________________________________________

	void CMotionBlur::tick(unsigned int msecs) {
		// compute view projection inverse matrix
		/*Ogre::Matrix4 projectionMatrix   = _sceneCamera->getProjectionMatrix();
		_inverseViewProjMatrix = (projectionMatrix * _sceneCamera->getViewMatrix()).inverse();

		float interpolationFactor				= _previousFPS * 0.03f; // m_timeScale is a multiplier to control motion blur interactively
		Ogre::Quaternion current_orientation	= _sceneCamera->getDerivedOrientation();
		Ogre::Vector3 current_position			= _sceneCamera->getDerivedPosition();
		Ogre::Quaternion estimatedOrientation	= Ogre::Quaternion::Slerp(interpolationFactor, current_orientation, (_previousOrientation));
		Ogre::Vector3 estimatedPosition			= (1-interpolationFactor) * current_position + interpolationFactor * (_previousPosition);
		Ogre::Matrix4 prev_viewMatrix			= Ogre::Math::makeViewMatrix(estimatedPosition, estimatedOrientation);
		// compute final matrix
		_previousViewProjMatrix = projectionMatrix * prev_viewMatrix;

		// update position and orientation for next update time
		_previousOrientation = current_orientation;
		_previousPosition = current_position;
		_previousFPS = 1.0f / msecs * 0.001f;*/
	}

	//________________________________________________________________________
		
	void CMotionBlur::cameraDestroyed(Ogre::Camera *camera) {
		_sceneCamera = NULL;
	}
}
