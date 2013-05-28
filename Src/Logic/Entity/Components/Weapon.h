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
	de disparo es responsabilidad del cliente.

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

		inline void resetAmmo();

		// Pinta un decal dada una entidad y una posicion
		void drawDecal(Logic::CEntity* pEntity, Vector3 vPos);

	protected:

		virtual void onTick(unsigned int msecs);

		virtual void onAvailable();

		void emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay = false);

		void decrementAmmo(unsigned int ammoSpent = 1);

	private:

		inline bool canUsePrimaryFire();

		inline bool canUseSecondaryFire();

		inline void readMustAttributes(const Map::CEntity* entityInfo);

		inline void readOptionalAttributes(const Map::CEntity* entityInfo);

		//__________________________________________________________________

		// Municion 

		/** Cuanta munición puede llevar este arma como máximo. */
		unsigned int _maxAmmo;
		/** Cuanta munición tenemos actualmente en este arma. */
		unsigned int _currentAmmo;
		
		unsigned int _primaryFireDamage;
		unsigned int _secondaryFireDamage;

		float _capsuleRadius;
		float _shootHeight;
		
		/** Cuantas balas se gastan por disparo primario. */
		unsigned int _ammoSpentPerPrimaryShot;
		/** Cuantos disparos se hacen al activar el disparo primario. */
		unsigned int _shotsPerPrimaryFire;

		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _ammoSpentPerSecondaryShot;
		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _shotsPerSecondaryFire;


		// Cooldowns

		unsigned int _primaryFireCooldown;
		int _primaryFireTimer;


		unsigned int _secondaryFireCooldown;
		int _secondaryFireTimer;

		WeaponType::Enum _weaponID;

		// Nombre del arma
		std::string _weaponName;

	}; // class IWeapon

} // namespace Logic

#endif // __Logic_Weapon_H
