//---------------------------------------------------------------------------
// PUParticle.h
//---------------------------------------------------------------------------

/**
@file PUParticle.h

Contiene la declaración de la clase que maneja el wrapper de partículas
de Particle Universe.

@see Graphics::PUParticle

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Graphics_PUParticle_H
#define __Graphics_PUParticle_H

#include "Graphics\Particle.h"
#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"
#include "OgreParticleSystem.h"

#include "ParticleUniverse\ParticleUniverseCommon.h"
#include "ParticleUniverse\ParticleUniverseSystemListener.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre {
	class SceneNode;
}

namespace Graphics {
	class CScene;
	class CServer;
	class CEntity;
}

namespace ParticleUniverse {
	class ParticleSystem;
}

namespace Graphics {
	
	/**
	Clase de PUParticle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/

	class PUParticle : public ParticleUniverse::ParticleSystemListener {
	public:

		class IObserver {
		public:
			virtual void onEmitterStart() { };
			virtual void onEmitterStop() { };
			virtual void onParticlesExpired() { };
		};

		/**
		Constructor de la clase.

		@param name Nombre del OgreParticle.
		*/
		PUParticle(const std::string& scriptName, Graphics::CEntity* parent = NULL);

		/**
		Destructor de la partícula.
		*/
		~PUParticle();

		virtual void handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem, 
											   ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent);

		void start();
		void start(float stopTime);
		void startAndStopFade(float stopTime);
		
		void stop();
		void stop(float stopTime);
		void stopFade(float stopTime);

		void pause();
		void pause(float pauseTime);

		void resume();


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
		Setea la direccion del emisor que se le introduce a la particula, lleva la magnitud ya multiplicada por la direccion

		@param directionWithForce direcion de la particula con la fuerza ya multiplicada
		*/
		void setDirection(const Vector3 &direction);

		void setOrientation(const Quaternion &orientation);

		void addObserver(IObserver* observer);
		void removeObserver(IObserver* observer);

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

	private:

		std::list<IObserver*> _observers;

		static unsigned int _counter;

	}; // class PUParticle

} // namespace Graphics

#endif // __Graphics_PUParticle_H