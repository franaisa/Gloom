//---------------------------------------------------------------------------
// ShotGun.h
//---------------------------------------------------------------------------

/**
@file ShotGun.h

@see Logic::CShotGun
@see Logic::IAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_ShotGunAmmo_H
#define __Logic_ShotGunAmmo_H

#include "Logic/Entity/Components/Ammo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CShotGunAmmo : public IAmmo {
		DEC_FACTORY(CShotGunAmmo);
	public:

		/** Constructor por defecto. */
		CShotGunAmmo();

		//__________________________________________________________________

		virtual ~CShotGunAmmo();

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

		template <typename T>
		std::string toString(const T& data) const {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);
			ss << data;
			return ss.str();
		}

		std::string toString(const Vector3& data) const {
			std::stringstream ss (std::stringstream::in | std::stringstream::out);

			ss << data.x;
			ss << " ";
			ss << data.y;
			ss << " ";
			ss << data.z;

			return ss.str();
		}

	private:

		std::string _shootAudio;

		unsigned int _primaryFireCooldown;
		unsigned int _defaultPrimaryFireCooldown;
		int _primaryFireCooldownTimer;		
	};

	REG_FACTORY(CShotGunAmmo);

} // namespace Logic

#endif // __Logic_ShotGunAmmo_H