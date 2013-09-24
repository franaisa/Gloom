#ifndef __Logic_DynamicParticleSystem_H
#define __Logic_DynamicParticleSystem_H

#include "ParticleSystem.h"

namespace Graphics {
	class CEntity;
}

namespace Logic {

	class CDynamicParticleSystem : public CParticleSystem {
		DEC_FACTORY(CDynamicParticleSystem);
	public:

		CDynamicParticleSystem();
		~CDynamicParticleSystem();

		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);

		void setOwner(CEntity* owner);
		void setOffset(const Vector3& offset);
		void setGraphicParent(Graphics::CEntity* parent);

	protected:

		virtual void onStart();
		virtual void onTick(unsigned int msecs);

		CEntity* _owner;
		Graphics::CEntity* _graphicParent;

	}; // class CDynamicParticleSystem

	REG_FACTORY(CDynamicParticleSystem);

} // namespace Logic

#endif // __Logic_Particle_H
