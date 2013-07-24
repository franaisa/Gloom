/**
@file DynamicLight.h


@see Logic::CDynamicLight
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#ifndef __Logic_DynamicLight_H
#define __Logic_DynamicLight_H

#include "Logic/Entity/Component.h"

#include "Light.h"

//declaración de la clase
namespace Logic {

	/**
	Este componente controla la luz que tiene un objeto. 
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Julio, 2013
	*/

	class CDynamicLight : public CLight {
		DEC_FACTORY(CDynamicLight);
	public:

		/** Constructor por defecto.*/
		CDynamicLight();

		/** Destructor. */
		virtual ~CDynamicLight();

		virtual bool accept(const std::shared_ptr<CMessage>& message) { return false; }

		void turnOn(const Vector3& offset = Vector3::ZERO, float timeToLive = 0.0f);

		void turnOff();

	protected:

		virtual void onTick(unsigned int msecs);

		int _timer;
		Vector3 _offset;
	}; // class CDynamicLight

	REG_FACTORY(CDynamicLight);

} // namespace Logic

#endif // __Logic_Light_H
