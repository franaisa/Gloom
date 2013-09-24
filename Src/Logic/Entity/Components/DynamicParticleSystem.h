#ifndef __Logic_DynamicParticleSystem_H
#define __Logic_DynamicParticleSystem_H

#include "ParticleSystem.h"

namespace Graphics {
	class CEntity;
}

namespace Logic {

	/**
	Sigue a la entidad a la que pertenece. Tambien se puede
	hacer que siga a una entidad grafica distinto como nodo
	hijo. En ese ultimo caso el offset es muy importante ya
	que representa el desplazamiento local de la particula
	con respecto al nodo padre.
	*/
	class CDynamicParticleSystem : public CParticleSystem {
		DEC_FACTORY(CDynamicParticleSystem);
	public:

		CDynamicParticleSystem();
		~CDynamicParticleSystem();

		virtual bool accept(const std::shared_ptr<CMessage>& message);
		virtual void process(const std::shared_ptr<CMessage>& message);

		// En el caso de tener nodo de escena padre es el vector de
		// desplazamiento relativo al nodo padre
		void setOffset(const Vector3& offset);
		void setGraphicParent(Graphics::CEntity* parent);

	protected:

		virtual void onStart();
		virtual void onTick(unsigned int msecs);

		Graphics::CEntity* _graphicParent;

	}; // class CDynamicParticleSystem

	REG_FACTORY(CDynamicParticleSystem);

} // namespace Logic

#endif // __Logic_Particle_H
