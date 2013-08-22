//---------------------------------------------------------------------------
// Plane.h
//---------------------------------------------------------------------------

/**
@file Plane.h


@see Graphics::CPlane

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Graphics_Plane_H
#define __Graphics_Plane_H

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre {
	class Entity;
	class SceneNode;
}

namespace Graphics {
	class CScene;
	class CEntity;
}

namespace Graphics {

	class CPlane{
	public:
		CPlane(CScene* scene, const std::string& name, const Vector3& position, const Vector3& orientation, 
			   float width, float height, unsigned int xSegments, unsigned int ySegments, float uTiling, 
			   float vTiling, const Vector3& upVector, const std::string& materialName, unsigned int lightMask, 
			   bool castShadows = false);
		
		~CPlane();

	private:
		Ogre::Entity* _entity;
		Ogre::SceneNode* _sceneNode;
	};

}

#endif