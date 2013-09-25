//---------------------------------------------------------------------------
// VolumetricLightScattering.h
//---------------------------------------------------------------------------

/**
@file VolumetricLightScattering.h

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Graphics_VolumetricLightScattering_H
#define __Graphics_VolumetricLightScattering_H

#include "BaseSubsystems/Math.h"
#include <OgreMatrix4.h>
#include <OgreCamera.h>
#include <OgreCompositorInstance.h>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre {
	class CompositorManager;
}

namespace Graphics {
	class CCamera;
}

/**

@author Francisco Aisa García
@date Agosto, 2013
*/
namespace Graphics {
	
	/**
	Clase para administrar las variables pasadas a los compositors.

	@author Francisco Aisa García
	@date Agosto, 2013
	*/
	
	class CVolumetricLightScattering : public Ogre::CompositorInstance::Listener {
	public:

		CVolumetricLightScattering(Ogre::CompositorManager* compositorManager, Graphics::CCamera* camera, const std::string& sceneName);
		~CVolumetricLightScattering();

		virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

	protected:

		struct ScatteringParams {
			Vector3 lightPosition;
			float density;
			float decay;
			float weight;
			float exposure;
		};

		void initLightSources(const std::string& sceneName);
		std::pair<ScatteringParams, bool> getClosestLightSource(const std::list<ScatteringParams>& lightList);
		
		Ogre::CompositorInstance* _compositor;
		Ogre::Camera* _sceneCamera;
		Ogre::SceneNode* _cameraNode;

		std::map< std::string, std::list<ScatteringParams> > _mapScatterParams;
		std::map< std::string, std::list<ScatteringParams> >::iterator _currentScene;
	};

} // namespace Graphics

#endif // __Graphics_MotionBlur_H
