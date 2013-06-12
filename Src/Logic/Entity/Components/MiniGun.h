//---------------------------------------------------------------------------
// MiniGun.h
//---------------------------------------------------------------------------

/**
@file MiniGun.h

Contiene la declaración del componente que implementa el arma minigun.

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#ifndef __Logic_MiniGun_H
#define __Logic_MiniGun_H

#include "Logic/Entity/Components/Weapon.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CMiniGun : public IWeapon {
		DEC_FACTORY(CMiniGun);

	public:

		/** Constructor por defecto. */
		CMiniGun();

		//__________________________________________________________________

		virtual ~CMiniGun();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

		virtual void primaryFire();

		virtual void secondaryFire();

		virtual void stopPrimaryFire();

		virtual void stopSecondaryFire();

		virtual void amplifyDamage(unsigned int percentage);

		virtual void reduceCooldown(unsigned int percentage);

	protected:

		virtual void onFixedTick(unsigned int msecs);

		//Método que efectua el disparo
		void		 shoot				();

		CEntity*	 fireWeapon			();

		void		 triggerHitMessages	(CEntity* entityHit);

		void		 secondaryShoot		();

	private:

		unsigned int _defaultPrimaryFireCooldown;
		unsigned int _primaryFireCooldown;
		int _primaryFireCooldownTimer;

		unsigned int _defaultDamage;
		unsigned int _damage;

		unsigned int _secondaryFireCooldown;
		unsigned int _defaultSecondaryFireCooldown;
		int _secondaryFireCooldownTimer;

		float _ammoSpentTimer;
		float _ammoSpentTimeStep;
		float _defaultAmmoSpentTimeStep;
		unsigned int _currentSpentSecondaryAmmo;
		unsigned int _maxAmmoSpentPerSecondaryShot;
		float _secondaryFireLoadTime;

		bool _secondaryFireIsActive;

		/** Dispersión del arma. */
		float _dispersion;

		/** Dispersión original del arma. Variable auxiliar para guardar la referencia leída del mapa.*/
		float _dispersionOriginal;

		/** Distancia de alcance del arma. */
		float _distance;

		bool _primaryFireIsActive;
	}; // class CMiniGun

	REG_FACTORY(CMiniGun);

} // namespace Logic

#endif // __Logic__H
