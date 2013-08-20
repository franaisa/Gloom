//---------------------------------------------------------------------------
// Billboard.h
//---------------------------------------------------------------------------

/**
@file Billboard.h


@see Graphics::CBillboard

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Graphics_Billboard_H
#define __Graphics_Billboard_H

#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Ogre {
	class BillboardSet;
	class Billboard;
	class SceneNode;
}

namespace Graphics {
	class CScene;
	class CEntity;
}

namespace Graphics {

	/**
	
	*/

	class CBillboard{
	public:
		CBillboard();
		~CBillboard();

		void load(CScene* scene, CEntity* parent, const std::string& materialName, const Vector3& position, const Vector2& defaultDimensions);
		void setVisible(bool isVisible);
		bool isVisible();
		void setPosition(const Vector3& position);

	private:

		Ogre::BillboardSet* _billboardSet;
		Ogre::Billboard* _billboard;
		Ogre::SceneNode* _sceneNode;
	};

}

#endif