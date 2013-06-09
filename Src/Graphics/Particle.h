//---------------------------------------------------------------------------
// Particle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaración de la clase que maneja el Particle.

@see Graphics::CParticle

@author Antonio Jesus Narvaez
@date Junio, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de Particle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Particle.
		*/
		CParticle(){};

		/**
		Destructor de la partícula.
		*/
		~CParticle(){};


		/**
		posiciona la particula en la pos dada
		@params position, es un vector3 con la posicion donde se ubicada la particula
		*/
		virtual void setPosition(const Vector3 &position) = 0; 

		/**
		devuelve  la particula en la pos dada
		@return Vector3 con la posicion donde se ubicada la particula
		*/
		virtual Vector3 getPosition() = 0;

		/**
		Pone la particula a visible o invisible
		param visible, visibilidad de la particula
		*/
		virtual void setVisible(bool visible) = 0;

		/**
		Pone a la particula en modo activa
		*/
		virtual void activate() = 0;

		/**
		Pone a la particula en modo pausa
		*/
		virtual void deactivate() = 0;

		/**
		Fuerza la carga de los recursos de la particula
		*/
		virtual void loadResources() = 0;
		
		/**
		Indica si la particula esta emitiendo
		
		@return bool, true si la particula esta emitiendo, falso si no
		*/
		virtual bool isEmitting() = 0;

		/**
		Setea la direccion del emisor que se le introduce a la particula, lleva la magnitud ya multiplicada por la direccion

		@param directionWithForce direcion de la particula con la fuerza ya multiplicada
		*/
		virtual void setDirection(const Vector3 &directionWithForce) = 0;

		/**
		Devuelve el objeto ogre de la particula

		@return objeto ogre de la particula.
		*/
		//Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}

		virtual Ogre::SceneNode * getSceneNode() = 0;

		/**
		Cambia la orientación de la entidad.

		@param orientation Referencia a la matriz de rotación con la que debe 
		orientarse la entidad.
		*/
		virtual void setOrientation(const Matrix3 &orientation) = 0;

		/**
		Indica si la particula se a creado correctamente o por el contrario ha dado algun error.
		*/
		virtual bool isLoaded() = 0;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H