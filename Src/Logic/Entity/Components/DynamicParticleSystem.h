#ifndef __Logic_DynamicParticleSystem_H
#define __Logic_DynamicParticleSystem_H

#include "ParticleSystem.h"

namespace Logic {

	class CDynamicParticleSystem : public CParticleSystem {
		DEC_FACTORY(CDynamicParticleSystem);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CDynamicParticleSystem() { };
		~CDynamicParticleSystem() { };

		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		virtual void onTick(unsigned int msecs);

	}; // class CDynamicParticleSystem

	REG_FACTORY(CDynamicParticleSystem);

} // namespace Logic

#endif // __Logic_Particle_H
