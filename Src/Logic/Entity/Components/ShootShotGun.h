//---------------------------------------------------------------------------
// ShootShotGun.h
//---------------------------------------------------------------------------

/**
@file ShootShotGun.h

Contiene la declaración del componente que implementa la escopeta.
 
@author Antonio Jesus Narváez 
@date mayo, 2013
*/

#ifndef __Logic_ShootShotGun_H
#define __Logic_ShootShotGun_H

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

	class CShootShotGun : public IWeapon {
		DEC_FACTORY(CShootShotGun);
	public:

		/** Constructor por defecto. */
		CShootShotGun() : IWeapon("shotGun"), _dispersionAngle(0) {
			
		}

		virtual ~CShootShotGun();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void primaryShoot();
		
		virtual void secondaryShoot();

		virtual void fireWeapon();

		/**
		Metodo llamado por el proyectil para que este sea borrado como entidad y que lo borre de la lista de 
		proyectiles.
		@param projectile, puntero al proyectil que debera ser destruido
		*/
		void destroyProjectile(CEntity *projectile, CEntity *killedBy);

		//virtual void onFixedTick(unsigned int msecs);


	private:

		/**
		variable que lee del mapa la dispersion del arma
		*/
		float _dispersionAngle;

		/**
		variable que contiene el daño cuando la municon esta incendiada
		*/
		float _damageBurned;

		/**
		Lista con los punteros a los projectiles.
		*/
		std::set<CEntity*> _projectiles;
	};

	REG_FACTORY(CShootShotGun);

} // namespace Logic

#endif // __Logic_Shoot_H