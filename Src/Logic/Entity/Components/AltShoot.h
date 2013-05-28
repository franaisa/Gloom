//---------------------------------------------------------------------------
// AltShoot.h
//---------------------------------------------------------------------------

/**
@file AltShoot.h

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_AltShoot_H
#define __Logic_AltShoot_H

#include "Logic/Entity/Component.h"

namespace Logic {

	/**
	@ingroup logicGroup


	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CAltShoot : public IComponent {
	public:

		CAltShoot();

		virtual ~CAltShoot();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void process(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		// @abstract
		virtual void primaryFire() = 0;
		// @abstract
		virtual void secondaryFire() = 0;

		virtual void stopPrimaryFire() { /* Por defecto nada */ }

		virtual void stopSecondaryFire() { /* Por defecto nada */ }

	protected:

		virtual void onTick(unsigned int msecs);

		virtual void onAvailable();

	private:

		unsigned int _maxAmmo;

		unsigned int _currentAmmo;



	}; // class CAltShoot

} // namespace Logic

#endif // __Logic_AltShoot_H
