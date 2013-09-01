//---------------------------------------------------------------------------
// VolumetricLightScattering.cpp
//---------------------------------------------------------------------------

/**
@file VolumetricLightScattering.cpp

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "VolumetricLightScattering.h"
#include "BaseSubsystems/Server.h"

#include <OgreCompositorManager.h>

#include "Server.h"
#include "Scene.h"
#include "Camera.h"

namespace Graphics {
	
	CVolumetricLightScattering::CVolumetricLightScattering(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera) :							  
																_sceneCamera( camera->getOgreCamera() ) {
		_sceneCamera = camera->getOgreCamera();
		_cameraNode = camera->getSceneNode();

		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _sceneCamera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "volumetricLightCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "volumetricLightCompositor", true);
		
		_compositor->addListener(this);
	}

	//________________________________________________________________________

	CVolumetricLightScattering::~CVolumetricLightScattering() {
		// Nada que hacer
		_compositor->removeListener(this);
	}

	//________________________________________________________________________

	void CVolumetricLightScattering::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat) {
		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		// Pasamos la matriz de vista y proyeccion
		Matrix4 projectionMatrix = _sceneCamera->getProjectionMatrix();
		Matrix4 viewMatrix = _sceneCamera->getViewMatrix();
		Matrix4 viewProjMatrix = projectionMatrix * viewMatrix;
		fpParams->setNamedConstant("viewProjMatrix", viewProjMatrix);

		// Pasamos la direccion de la luz --> {-52.0654, 60.1029, -108.08}
		Vector3 lightPosition(-52.0654f, 60.1029f, -108.08f);
		fpParams->setNamedConstant("lightPosition", lightPosition);
	}
}
