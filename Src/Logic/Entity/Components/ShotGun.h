//---------------------------------------------------------------------------
// ShotGun.h
//---------------------------------------------------------------------------

/**
@file ShotGun.h

Contiene la declaración del componente que implementa la escopeta.
 
@author Antonio Jesus Narváez 
@date mayo, 2013
*/

#ifndef __Logic_ShotGun_H
#define __Logic_ShotGun_H

#include "Logic/Entity/Components/Weapon.h"
#include <vector>

namespace Logic {

	class CPhysicDynamicEntity;
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la escopeta. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la escopeta.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	*/

	class CShotGun : public IWeapon {
		DEC_FACTORY(CShotGun);
	public:

		/** Constructor por defecto. */
		CShotGun();

		virtual ~CShotGun();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void primaryFire();
		
		virtual void secondaryFire();

		virtual void amplifyDamage(unsigned int percentage);

		/**
		Metodo llamado por el proyectil para que este sea borrado como entidad y que lo borre de la lista de 
		proyectiles.
		@param projectile, puntero al proyectil que debera ser destruido
		*/
		void destroyProjectile(CEntity *projectile, CEntity *killedBy);

		void setCurrentAmmo(unsigned int ammo);
	protected:

		virtual void onTick(unsigned int msecs);
		
		/**
		Llamado cuando el arma pasa a ser activa.
		*/
		virtual void onActivate();	
	private:

		unsigned int _defaultPrimaryFireCooldown;
		unsigned int _primaryFireCooldown;
		int _primaryFireCooldownTimer;

		float _defaultPrimaryFireDamage;
		float _primaryFireDamage;

		unsigned int _numberOfShots;

		/**
		variable que lee del mapa la dispersion del arma
		*/
		float _dispersionAngle;

		/**
		variable que contiene el daño cuando la municon esta incendiada
		*/
		float _damageBurned;

		/**
		Fuerza de los proyectiles, equivale a la velocidad de estos
		*/
		float _projectileShootForce;

		/**
		radio de los proyectiles, equivale al tamaño de estos
		*/
		float _projectileRadius;

		/**
		Lista con los punteros a los projectiles.
		*/
		std::set<CEntity*> _projectiles;
	};

	REG_FACTORY(CShotGun);

} // namespace Logic

#endif // __Logic__H