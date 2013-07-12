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
		CLight():_light(NULL) {}
		~CLight();
		/**
		Método que crea una luz direccional en la posición dada, que mira en la
		dirección que se le indica como parámetro
		@param name El nombre de la luz que se va a crear
		@param position la posición de la escena donde se va a crear la luz
		@param direction Dirección en la que mira la luz que se crea
		@return true si todo fue bien, false en caso contrario
		*/
		bool createDirectionalLight(CScene* scene, std::string name, Vector3 position, Vector3 direction);
		/**
		Método que crea una luz puntual en la posición dada, que mira en la
		dirección que se le indica como parámetro
		@param name El nombre de la luz que se va a crear
		@param position la posición de la escena donde se va a crear la luz
		@param direction Dirección en la que mira la luz que se crea
		@return true si todo fue bien, false en caso contrario
		*/
		bool createPointLight(CScene* scene, std::string name, Vector3 position);
		/**
		Método que crea una luz local en la posición dada.
		@param name El nombre de la luz que se va a crear
		@param position la posición de la escena donde se va a crear la luz
		@return true si todo fue bien, false en caso contrario
		*/
		bool createSpotlLight(CScene* scene, std::string name, Vector3 position, Vector3 direction);

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
		void setColour(float r, float g, float b);
		/**
		Método para setear la atenuación de la luz
		@param Range rango de alcance de la luz.
		@param Kc Constante de atenuación constante.
		@param Kl Constante de atenuación lineal.
		@param Kq Constante de atenuación cuadrática.
		*/
		void setAttenuation(float Range, float Kc, float Kl, float Kq);
		/**
		Método que setea el color difuso de la luz
		@param r el rojo
		@param g el verde
		@param b el azul
		*/
		void setSpecularColour(float r, float g, float b);

		/**
		Método que devuelve la dirección de la luz
		@return la dirección en la que mira la luz
		*/
		Vector3 getDirection(){return _light->getDirection();}
		/**
		Método que setea la dirección de la luz
		@param direction la dirección en la que queremos que mire la luz
		*/
		void setDirection(Vector3 direction){_light->setDirection(direction);}
		/**
		Método que devuelve la posición de la luz
		@return la posición de la luz
		*/
		Vector3 getPosition(){return _light->getDirection();}
		/**
		Método que setea la posición de la luz
		@param position la posición en la que queremos la luz
		*/
		void setposition(Vector3 position){_light->setPosition(position);}

		/**
		Método que setea la fuerza de la luz
		@param intensiy la fuerza emisora de la luz.
		*/
		void setIntensity(float intensity){_light->setPowerScale(intensity);}

		/**
		Método que setea el angulo de salida de un Sporlight y en angulo Maximo.
		@pamam innerAngle angulo de salida del SpotLight
		@pamam outerAngle angulo de salida del SpotLight
		*/
		void setRange(float innerAngle, float outerAngle){_light->setSpotlightRange(Ogre::Radian(innerAngle), Ogre::Radian(outerAngle));}	

	protected:
		Ogre::Light* _light;
	};

}

#endif