//---------------------------------------------------------------------------
// ShootRocketLauncher.h
//---------------------------------------------------------------------------

/**
@file ShootRocketLauncher.h

Contiene la declaración del componente que implementa el lanzacohetes.

Contiene la declaración de la cabecera del componente
de disparo del lanzacohetes.

@see Logic::CShootRocketLauncher
@see Logic::CShootProjectile

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#ifndef __Logic_ShootRocketLauncher_H
#define __Logic_ShootRocketLauncher_H

#include "Logic/Entity/Components/Weapon.h"

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad del lanzacohetes.

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
	*/

	class CShootRocketLauncher : public IWeapon {
		DEC_FACTORY(CShootRocketLauncher);

	public:

		/** Constructor por defecto. */
		CShootRocketLauncher() : IWeapon("rocketLauncher") { }

		virtual ~CShootRocketLauncher();

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		void drawRaycast(const Ray& raycast);
		//__________________________________________________________________

		/** Al ejecutarse dispara un cohete. */
		virtual void fireWeapon();

	private:

		float _shootForce;
		int _temporal;

	};

	REG_FACTORY(CShootRocketLauncher);

} // namespace Logic

#endif // __Logic_RocketLauncher_H
