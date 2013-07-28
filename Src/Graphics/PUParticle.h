//---------------------------------------------------------------------------
// PUParticle.h
//---------------------------------------------------------------------------

/**
@file PUParticle.h

Contiene la declaración de la clase que maneja el wrapper de partículas
de Particle Universe.

@see Graphics::PUParticle

@author Rubén Mulero Guerrero
@author Antonio Jesús Narváez Corrales

@date Enero, 2013
*/

#ifndef __Graphics_PUParticle_H
#define __Graphics_PUParticle_H

#include "Graphics\Particle.h"
#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"
#include "OgreParticleSystem.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace ParticleUniverse
{
	class ParticleSystem;
}

namespace Graphics 
{
	/**
	Clase de PUParticle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class PUParticle : public CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del OgreParticle.
		*/
		PUParticle(const std::string &ogreParticleName, bool isOverlay = false);

		/**
		Destructor de la partícula.
		*/
		~PUParticle();


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

		void setOrientation(const Matrix3 &orientation){};
		/**
		Devuelve el objeto ogre de la particula

		@return objeto ogre de la particula.
		*/
		ParticleUniverse::ParticleSystem* getParticleSystem() {return _particleSystem;}

		Ogre::SceneNode * getSceneNode(){ return _sceneNode;}


		bool isLoaded();
		

	protected:
		/** 
		OgreParticleSystem _ogreParticleSystem(sistema de Particulas)
		*/
		ParticleUniverse::ParticleSystem* _particleSystem;

		/**
		Nombre del OgreParticleSystem.
		*/
		std::string _name;

		/**
		scene donde se crea la particula
		*/
		Ogre::SceneNode *_sceneNode;

	}; // class PUParticle

} // namespace Graphics

#endif // __Graphics_PUParticle_H