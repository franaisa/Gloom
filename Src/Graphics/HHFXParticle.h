//---------------------------------------------------------------------------
// HHFXParticle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaración de la clase que maneja el Particle.

@see Graphics::HHFXParticle


@ingroup graphicsGroup
@author Rubén Mulero
@date May, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

#include "HHFX.h"
// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;

}
namespace Graphics 
{
	class CScene;
	class CServer;
	class HHFX;
}

namespace Graphics 
{
	/**
	Clase de Particle extendida basada en HHFX. Abstrae toda la lógica de
	creación de particulas de este tipo a la lógica, de manera que solo le
	pide un objeto de este tipo y se maneja de la misma manera que el 
	resto de particulas
	
	*/
	class HHFXParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Particle.
		*/
		HHFXParticle(const std::string &particleName, Vector3 position = Vector3::ZERO, bool isOverlay = false);

		/**
		Destructor de la partícula.
		*/
		~HHFXParticle();


		/**
		posiciona la particula en la pos dada
		@params position, es un vector3 con la posicion donde se ubicada la particula
		*/
		void setPosition(const Vector3 &position);

		/**
		devuelve  la particula en la pos dada
		@return Vector3 con la posicion donde se ubicada la particula
		*/
		Vector3 getPosition();

		/**
		Pone la particula a visible o invisible
		param visible, visibilidad de la particula
		*/
		void setVisible(bool visible);

		/**
		Pone a la particula en modo activa
		*/
		void activate();

		/**
		Pone a la particula en modo pausa
		*/
		void deactivate();

		/**
		Fuerza la carga de los recursos de la particula
		*/
		void loadResources();
		
		/**
		Indica si la particula esta emitiendo
		
		@return bool, true si la particula esta emitiendo, falso si no
		*/
		bool isEmitting();

		/**
		Setea la direccion del emisor que se le introduce a la particula, lleva la magnitud ya multiplicada por la direccion

		@param directionWithForce direcion de la particula con la fuerza ya multiplicada
		*/
		void setDirection(const Vector3 &directionWithForce);

		/**
		Devuelve el objeto ogre de la particula

		@return objeto ogre de la particula.
		*/
		//Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}

		Ogre::SceneNode * getSceneNode(){ return _particleNode;}

		

	protected:
		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		HHFX* _particleSystem;

		/**
		Nombre del ParticleSystem.
		*/
		std::string _nameParticle;

		/**
		scene donde se crea la particula
		*/
		IHHFXOgre *_particle;

		Ogre::SceneNode* _particleNode;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H