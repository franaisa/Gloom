//---------------------------------------------------------------------------
// Weapon.h
//---------------------------------------------------------------------------

/**
@file Weapon.h

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_Weapon_H
#define __Logic_Weapon_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

#include <string>

namespace Logic {

	/**
	@ingroup logicGroup

	IMPORTANTE: Decrementar la munición y emitir los sonidos
	de disparo es responsabilidad del cliente. Las animaciones
	de las armas tambien son responsabilidad del cliente.

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class IWeapon : public IComponent {
	public:

		IWeapon(const std::string& weaponName);

		virtual ~IWeapon();

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

		void addAmmo(int weapon, int ammo, bool iAmCatch);

		virtual void resetAmmo();

		virtual void amplifyDamage(unsigned int percentage) = 0;

		virtual void reduceCooldown(unsigned int percentage) = 0;

	protected:

		//virtual void onTick(unsigned int msecs);

		virtual void onAvailable();

		void emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay = false);

		void decrementAmmo(unsigned int ammoSpent = 1);

		// Pinta un decal dada una entidad y una posicion
		void drawDecal(Logic::CEntity* pEntity, Vector3 vPos);



		virtual bool canUsePrimaryFire() = 0;

		virtual bool canUseSecondaryFire() = 0;

		//__________________________________________________________________

		inline void readMustAttributes(const Map::CEntity* entityInfo);

		inline void readOptionalAttributes(const Map::CEntity* entityInfo);

		//__________________________________________________________________


		/** Cuanta munición puede llevar este arma como máximo. */
		unsigned int _maxAmmo;
		/** Cuanta munición tenemos actualmente en este arma. */
		unsigned int _currentAmmo;
		
		float _capsuleRadius;
		float _heightShoot;
		
		WeaponType::Enum _weaponID;

		// Nombre del arma
		std::string _weaponName;

	}; // class IWeapon

} // namespace Logic

#endif // __Logic_Weapon_H
