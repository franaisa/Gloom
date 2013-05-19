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

#include "Logic/Entity/Components/ShootProjectile.h"
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

	class CShootShotGun : public CShootProjectile {
		DEC_FACTORY(CShootShotGun);
	public:

		/** Constructor por defecto. */
		CShootShotGun() : CShootProjectile("shotGun"), _dispersionAngle(0) {
			
		}

		virtual ~CShootShotGun();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void primaryShoot();
		
		virtual void secondaryShoot();

		virtual void fireWeapon();

		void magnetizeBullet();

		virtual void onFixedTick(unsigned int msecs);


	private:

		struct TProjectile{
			Vector3 _projectileDirection;
			CPhysicDynamicEntity* projectile;
		};
		float _dispersionAngle;

		std::vector<TProjectile> _projectiles;
	};

	REG_FACTORY(CShootShotGun);

} // namespace Logic

#endif // __Logic_Shoot_H