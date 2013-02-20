/**
@file ShootGrenadeLauncher.h

Contiene la declaración de la cabecera del componente
de disparo del lanzagranadas.

@see Logic::CShootGrenadeLauncher
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ShootGrenadeLauncher_H
#define __Logic_ShootGrenadeLauncher_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CShootGrenadeLauncher : public CShoot {
		DEC_FACTORY(CShootGrenadeLauncher);

	public:
		CShootGrenadeLauncher() : CShoot("GrenadeLauncher") { }

		virtual void shoot() { }
	};

	REG_FACTORY(CShootGrenadeLauncher);

} // namespace Logic

#endif // __Logic_Shoot_H
