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

#include "Input/Server.h"
#include "Input/PlayerController.h"

#include "Logic/Entity/Entity.h"

#include <OgreCompositorManager.h>

#include "Server.h"
#include "Scene.h"
#include "Camera.h"

namespace Graphics {
	
	CVolumetricLightScattering::CVolumetricLightScattering(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera, const std::string& sceneName) :							  
																_sceneCamera( camera->getOgreCamera() ),
																_currentScene( _mapScatterParams.end() ) {
		_sceneCamera = camera->getOgreCamera();
		_cameraNode = camera->getSceneNode();

		// Añadimos el compositor de motion blur
		Ogre::Viewport* cameraViewport = _sceneCamera->getViewport();

		_compositor = compositorManager->addCompositor(cameraViewport, "volumetricLightCompositor");
		compositorManager->setCompositorEnabled(cameraViewport, "volumetricLightCompositor", true);
		
		_compositor->addListener(this);

		initLightSources(sceneName);

		_currentScene = _mapScatterParams.find(sceneName);
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

		// Seteamos los valores del foco de luz mas cercano
		if( _currentScene != _mapScatterParams.end() ) {
			std::pair<ScatteringParams, bool> closestLightSrc = getClosestLightSource(_currentScene->second);

			if(closestLightSrc.second) {
				fpParams->setNamedConstant("lightPosition", closestLightSrc.first.lightPosition);
				fpParams->setNamedConstant("Density", closestLightSrc.first.density);
				fpParams->setNamedConstant("Decay", closestLightSrc.first.decay);
				fpParams->setNamedConstant("Weight", closestLightSrc.first.weight);
				fpParams->setNamedConstant("Exposure", closestLightSrc.first.exposure);
			}
		}
	}

	//________________________________________________________________________

	std::pair<CVolumetricLightScattering::ScatteringParams, bool> CVolumetricLightScattering::getClosestLightSource(const std::list<ScatteringParams>& lightList) {
		std::pair<ScatteringParams, bool> closestLightSrc;
		closestLightSrc.second = false;

		// Obtenemos el puntero a la entidad del jugador que en este caso es lo que nos interesa
		Logic::CEntity* player = Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar();
		if(player != NULL) {
			Vector3 playerPosition = player->getPosition();
			Vector2 playerPos2d(playerPosition.x, playerPosition.z);

			auto it = lightList.begin();
			Vector2 pos2d(it->lightPosition.x, it->lightPosition.z);
			auto closestLight = it++;
			float closestDist = (playerPos2d - pos2d).length();
			float dist;

			for(; it != lightList.end(); ++it) {
				pos2d.x = it->lightPosition.x;
				pos2d.y = it->lightPosition.z;

				dist = (playerPos2d - pos2d).length();
				if(dist < closestDist) {
					closestLight = it;
					closestDist = dist;
				}
			}

			closestLightSrc.second = true;
			closestLightSrc.first = *closestLight;
		}

		return closestLightSrc;
	}

	//________________________________________________________________________

	void CVolumetricLightScattering::initLightSources(const std::string& sceneName) {
		std::pair< std::string, std::list<ScatteringParams> > temp;
		std::list<ScatteringParams> lightInfoList;
		ScatteringParams params;
		
		if( sceneName.find("map3") != std::string::npos ) {
			temp.first = sceneName;

			// Luz de lava 1
			
			params.lightPosition	= Vector3(56.0013, -80.1929, 122.114);
			params.density			= 0.2f;
			params.decay			= 0.99f;
			params.weight			= 1.0f;
			params.exposure			= 0.1f;

			lightInfoList.push_back(params);
		
			// Luz de lava 2
			params.lightPosition	= Vector3(74.8055f, 20.179f, -134.406f);

			lightInfoList.push_back(params);

			// Luz del vortice
			params.lightPosition	= Vector3(-256.412f, -500.0f, -98.3735f);
			params.density			= 0.22f;

			lightInfoList.push_back(params);

			// Introducimos las luces del mapa de angel
			temp.second = lightInfoList;

			_mapScatterParams.insert(temp);
		}
		else if( sceneName.find("DungeonsOfAgony") != std::string::npos ) {
			temp.first = sceneName;

			// Luz de lava pipes
			params.lightPosition	= Vector3(48.2968f, -200.1517f, 122.804f);
			params.density			= 0.2f;
			params.decay			= 0.99f;
			params.weight			= 1.0f;
			params.exposure			= 0.1f;

			lightInfoList.push_back(params);
		
			// Luz de lava altar
			params.lightPosition	= Vector3(82.3584f, -45.033f, -133.341f);

			lightInfoList.push_back(params);

			// Luz del vortice
			
			params.lightPosition	= Vector3(-255.078f, -400.725f, -105.462f);
			params.density			= 0.22f;
			params.exposure			= 0.2f;

			lightInfoList.push_back(params);

			// Introducimos las luces del mapa de angel por trozos
			temp.second = lightInfoList;

			_mapScatterParams.insert(temp);
		}
		else if( sceneName.find("map2") != std::string::npos ) {
			// Luces del mapa de alberto
			temp.first = sceneName;
			
			params.lightPosition	= Vector3(-180.146f, -598.734f, -48.2743f);
			params.density			= 0.13f;
			params.exposure			= 0.1f;
			params.decay			= 0.9f;
			params.weight			= 1.0f;

			lightInfoList.push_back(params);
			temp.second = lightInfoList;
		
			_mapScatterParams.insert(temp);
		}
	}
}
