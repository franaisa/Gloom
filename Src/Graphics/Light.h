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

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre {
	class SceneNode;
	class Light;
}

namespace Graphics {
	class CScene;
}

namespace Graphics {

	struct LightType {
		enum Enum {
			eDIRECTIONAL_LIGHT,
			ePOINT_LIGHT,
			eSPOT_LIGHT
		};
	};

	/**
	Clase que representa las luces en el escenario de ogre, contiene una referencia
	que representa la luz de ogre y otra que es el nodo de la escena al que está 
	asociada esa luz
	Al construir la clase no se crean las luces, la construcción de un objeto de
	esta clase representa la luz, pero hay que usar los métodos que nos proporciona
	para crear y manejar la clase a nuestro antojo
	*/
	class CLight{
	public:
		CLight(const LightType::Enum lightType, const std::string& lightName, const Vector3& position = Vector3::ZERO, const Vector3& direction = Vector3::NEGATIVE_UNIT_Y);
		~CLight();

		/**
		Método que setea la posición de la luz
		@param position la posición en la que queremos la luz
		*/
		void setPosition(const Vector3& position);
		/**
		Método que setea la dirección de la luz
		@param direction la dirección en la que queremos que mire la luz
		*/
		void setDirection(const Vector3& direction);
		/**
		Método controla las sombras que castea la luz.
		@param enabled indica si las sombras se activan o se desactivan
		*/
		void setCastShadows(bool enabled);
		/**
		Método que setea el color de la luz
		@param r el rojo
		@param g el verde
		@param b el azul
		*/
		void setColor(float r, float g, float b);
		/**
		Método para setear la atenuación de la luz
		@param Range rango de alcance de la luz.
		@param Kc Constante de atenuación constante.
		@param Kl Constante de atenuación lineal.
		@param Kq Constante de atenuación cuadrática.
		*/
		void setAttenuation(float Range, float Kc, float Kl, float Kq);
		/**
		Método para configurar los parámetros de los spotlights.
		@param innerAngle Ángulo del cono interior (en radianes).
		@param outerAngle Ángulo del cono exterior (en radianes).
		*/
		void setSpotLightParams(float innerAngle, float outerAngle);
		
		/**
		Método que devuelve la dirección de la luz
		@return la dirección en la que mira la luz
		*/
		Vector3 getDirection();
		/**
		Método que devuelve la posición de la luz
		@return la posición de la luz
		*/
		Vector3 getPosition();
		

	protected:

		Ogre::Light* _light;
	};

}

#endif