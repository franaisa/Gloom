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
	
	CMotionBlur::CMotionBlur(Ogre::CompositorManager* compositorManager, Ogre::Camera* camera) : _previousViewProjMatrix(Matrix4::IDENTITY), 
																								 _camera(camera) {
		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _camera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "MotionBlurFranCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "MotionBlurFranCompositor", true);
		
		_compositor->addListener(this);
		_camera->addListener(this);
	}

	CMotionBlur::~CMotionBlur() {
		// Nada que hacer
		_compositor->removeListener(this);
		_camera->removeListener(this);
	}

	void CMotionBlur::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat) {
		mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("previousViewProjMatrix", _previousViewProjMatrix);
	}

	void CMotionBlur::cameraPreRenderScene(Ogre::Camera* camera) {
		// Actualizamos la matriz de vista y proyeccion del frame anterior
		// para el shader de motion blur
		Matrix4 previousViewMatrix = _camera->getViewMatrix();
		Matrix4 previousProjectionMatrix = _camera->getProjectionMatrix();
		_previousViewProjMatrix = previousViewMatrix * previousProjectionMatrix;
	}
}
