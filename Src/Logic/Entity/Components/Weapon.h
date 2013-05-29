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

		virtual void stopPrimaryFire(unsigned int elapsedTime) { /* Por defecto nada */ }

		virtual void stopSecondaryFire(unsigned int elapsedTime) { /* Por defecto nada */ }

		void addAmmo(int weapon, int ammo, bool iAmCatch);

		virtual void resetAmmo();

		void amplifyDamage(unsigned int percentage);

		void reduceCooldown(unsigned int percentage);

	protected:

		virtual void onTick(unsigned int msecs);

		virtual void onAvailable();

		void emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay = false);

		void decrementAmmo(unsigned int ammoSpent = 1);

		// Pinta un decal dada una entidad y una posicion
		void drawDecal(Logic::CEntity* pEntity, Vector3 vPos);



		inline bool canUsePrimaryFire();

		inline bool canUseSecondaryFire();

		//__________________________________________________________________

		inline void readMustAttributes(const Map::CEntity* entityInfo);

		inline void readOptionalAttributes(const Map::CEntity* entityInfo);

		inline void readToggleAttributes(const Map::CEntity* entityInfo);

		//__________________________________________________________________

		// Municion 

		/** Cuanta munición puede llevar este arma como máximo. */
		unsigned int _maxAmmo;
		/** Cuanta munición tenemos actualmente en este arma. */
		unsigned int _currentAmmo;
		
		unsigned int _defaultPrimaryFireDamage;
		unsigned int _defaultSecondaryFireDamage;

		unsigned int _primaryFireDamage;
		unsigned int _secondaryFireDamage;

		float _capsuleRadius;
		float _heightShoot;
		
		/** Cuantas balas se gastan por disparo primario. */
		unsigned int _ammoSpentPerPrimaryShot;
		/** Cuantos disparos se hacen al activar el disparo primario. */
		unsigned int _shotsPerPrimaryFire;

		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _ammoSpentPerSecondaryShot;
		/** Cuantas balas se gastan por disparo secundario. */
		unsigned int _shotsPerSecondaryFire;

		float _shotsDistance;
		// Cooldowns
		unsigned int _defaultPrimaryFireCooldown;
		unsigned int _primaryFireCooldown;
		int _primaryFireTimer;

		unsigned int _defaultSecondaryFireCooldown;
		unsigned int _secondaryFireCooldown;
		int _secondaryFireTimer;

		WeaponType::Enum _weaponID;

		// Nombre del arma
		std::string _weaponName;


		// Para cosas toggle
		unsigned int _elapsedTimeSincePrimaryFire; // timer
		unsigned int _elapsedTimeSinceSecondaryFire; // timer

		bool _primaryFireIsActive;
		bool _secondaryFireIsActive;

		bool _primaryFireIsToggle;
		bool _secondaryFireIsToggle;

		// Si el arma es toggle y su tiempo de carga es 0 entonces
		// la municion baja por los intervalos dictados por el cooldown
		unsigned int _primaryFireLoadTime;
		unsigned int _secondaryFireLoadTime;

		unsigned int _maxAmmoSpentPerPrimaryShot;
		unsigned int _maxAmmoSpentPerSecondaryShot;
	}; // class IWeapon

} // namespace Logic

#endif // __Logic_Weapon_H
