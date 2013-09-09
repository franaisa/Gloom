/**
@file ParticleSystem.h

Componente que contiene la declaracion de las particulas.

@see Logic::CParticleSystem
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/
#ifndef __Logic_ParticleSystem_H
#define __Logic_ParticleSystem_H

#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
#include "Graphics/PUParticle.h"

namespace Logic {

	/**
	Este componente controla la particula que tiene un objeto. 
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/

	class CParticleSystem : public IComponent, public Graphics::PUParticle::IObserver {
		DEC_FACTORY(CParticleSystem);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CParticleSystem();

		virtual ~CParticleSystem();

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);

		virtual void onParticlesExpired();

		void startEmitting() { _particleSystem->start(); }
		void startEmitting(float stopTime) { _particleSystem->start(stopTime); }
		void startAndStopFadeEmitting(float stopTime) { _particleSystem->startAndStopFade(stopTime); }
		
		void stopEmitting() { _particleSystem->stop(); }
		void stopEmitting(float stopTime) { _particleSystem->stop(stopTime); }
		void stopFadeEmitting(float stopTime) { _particleSystem->stopFade(stopTime); }

		void pauseEmitting() { _particleSystem->pause(); }
		void pauseEmitting(float pauseTime) { _particleSystem->pause(pauseTime); }

		void resumeEmitting() { _particleSystem->resume(); }

		void setDirection(const Vector3& direction) { _particleSystem->setDirection(direction); }
		void setOrientation(const Quaternion& orientation) { _particleSystem->setOrientation(orientation); }

	protected:

		virtual void onStart();



		Graphics::PUParticle* _particleSystem;

		std::string _scriptName;

		Vector3 _direction;
		Vector3 _offset;

		bool _destroyEntityOnExpiry;
		bool _emitting;

	}; // class CParticleSystem

	REG_FACTORY(CParticleSystem);

} // namespace Logic

#endif // __Logic_Particle_H
