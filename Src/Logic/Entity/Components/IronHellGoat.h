//---------------------------------------------------------------------------
// IronHellGoat.h
//---------------------------------------------------------------------------

/**
@file IronHellGoat.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CIronHellGoat
@see Logic::CShootProjectile

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_IronHellGoat_H
#define __Logic_IronHellGoat_H

#include "Logic/Entity/Components/ShootProjectile.h"

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CIronHellGoat : public CShootProjectile {
		DEC_FACTORY(CIronHellGoat);

	public:

		/** Constructor por defecto. */
		CIronHellGoat() : CShootProjectile("ironHellGoat") { }

		//__________________________________________________________________

		virtual ~CIronHellGoat();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		virtual void primaryShoot();

		//__________________________________________________________________

		virtual void secondaryShoot();

		//__________________________________________________________________

		virtual void fireWeapon() { /* Esto esta deprecado */ }

	private:


	};

	REG_FACTORY(CIronHellGoat);

} // namespace Logic

#endif // __Logic_Shoot_H
