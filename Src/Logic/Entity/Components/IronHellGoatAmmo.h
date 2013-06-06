//---------------------------------------------------------------------------
// IronHellGoat.h
//---------------------------------------------------------------------------

/**
@file IronHellGoat.h

@see Logic::CIronHellGoat
@see Logic::IAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_IronHellGoatAmmo_H
#define __Logic_IronHellGoatAmmo_H

#include "Logic/Entity/Components/Ammo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CIronHellGoatAmmo : public IAmmo {
		DEC_FACTORY(CIronHellGoatAmmo);
	public:

		/** Constructor por defecto. */
		CIronHellGoatAmmo();

		//__________________________________________________________________

		virtual ~CIronHellGoatAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		virtual void reduceCooldown(unsigned int percentage);

		//__________________________________________________________________

		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void primaryFire();

		//__________________________________________________________________

		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopPrimaryFire();

	protected:

		virtual void onActivate();

		virtual void onAvailable();

		virtual void onTick(unsigned int msecs);

		virtual bool canUsePrimaryFire();

		virtual bool canUseSecondaryFire();

	private:

		std::string _shootAudio;

		unsigned int _primaryFireCooldown;
		unsigned int _defaultPrimaryFireCooldown;
		int _primaryFireCooldownTimer;

		bool _primaryFireIsActive;

		unsigned int _elapsedTime;
		unsigned int _maxLoadingTime;
		unsigned int _ammoSpentTimeStep;
		unsigned int _ammoSpentTimer;
		unsigned int _currentSpentAmmo;

		unsigned int _maxAmmoPerShot;
	};

	REG_FACTORY(CIronHellGoatAmmo);

} // namespace Logic

#endif // __Logic_IronHellGoatAmmo_H