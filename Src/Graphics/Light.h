//---------------------------------------------------------------------------
// Light.h
//---------------------------------------------------------------------------

/**
@file Light.h

Contiene la declaración de la clase que representa una Luz.

@see Graphics::Clight

@author Rubén Mulero
@date March, 2013
*/

#ifndef __Graphics_Light_H
#define __Graphics_Light_H

#include "BaseSubsystems/Math.h"

/*

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class Light;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{
	class CLight{
	public:
		CLight(): _node(0), _light(0){}
		virtual ~CLight();

		bool createLight(std::string name,LightType type, Vector3 position);

		bool destroyLight();

		void attachToScene(CScene *scene);

	

	protected:
		Ogre::Light* _light;

		Ogre::SceneNode * _node;

	};

}
*/
#endif