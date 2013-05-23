//---------------------------------------------------------------------------
// Particle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaración de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

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
		CParticle(const std::string &particleName, bool isOverlay = false);

		/**
		Destructor de la partícula.
		*/
		~CParticle();


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
		Fuerza la carga de los recursos de la particula
		*/
		void loadResources();

		/**
		Setea la direccion del emisor que se le introduce a la particula, lleva la magnitud ya multiplicada por la direccion

		@param directionWithForce direcion de la particula con la fuerza ya multiplicada
		*/
		void setDirection(const Vector3 &directionWithForce);

		/**
		Devuelve el objeto ogre de la particula

		@return objeto ogre de la particula.
		*/
		Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}

		Ogre::SceneNode * getSceneNode(){ return _sceneNode;}

		

	protected:
		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		Ogre::ParticleSystem* _particleSystem;

		/**
		Nombre del ParticleSystem.
		*/
		std::string _nameParticle;

		/**
		scene donde se crea la particula
		*/
		Ogre::SceneNode *_sceneNode;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H