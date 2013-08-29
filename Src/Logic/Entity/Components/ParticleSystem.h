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

		virtual void onParticlesExpired();

	protected:

		virtual void onStart();



		Graphics::PUParticle* _particleSystem;

		std::string _scriptName;

		Vector3 _direction;

		bool _destroyEntityOnExpiry;
		bool _emitting;

	}; // class CParticleSystem

	REG_FACTORY(CParticleSystem);

} // namespace Logic

#endif // __Logic_Particle_H
