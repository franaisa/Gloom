//---------------------------------------------------------------------------
// ShootMiniGun.h
//---------------------------------------------------------------------------

/**
@file ShootMiniGun.h

Contiene la declaración del componente que implementa la minigun.

Contiene la declaración de la cabecera del componente
de disparo del lanzagranadas.

@see Logic::CShootGrenadeLauncher
@see Logic::CShootProjectile

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ShootGrenadeLauncher_H
#define __Logic_ShootGrenadeLauncher_H

#include "Logic/Entity/Components/ShootProjectile.h"

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad del lanzagranadas.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CShootGrenadeLauncher : public CShootProjectile {
		DEC_FACTORY(CShootGrenadeLauncher);

	public:

		/** Constructor por defecto. */
		CShootGrenadeLauncher() : CShootProjectile("GrenadeLauncher") { }

		//__________________________________________________________________

		/** Al ejecutarse dispara una granada. */
		virtual void fireWeapon() { }
	};

	REG_FACTORY(CShootGrenadeLauncher);

} // namespace Logic

#endif // __Logic_Shoot_H
