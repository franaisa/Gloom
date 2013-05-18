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

#include <set>

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
		CIronHellGoat();

		//__________________________________________________________________

		virtual ~CIronHellGoat();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		// El propio shoot deberia encargarse de llamar a lo que toque teniendo
		// en cuenta la municion y las demas historias
		virtual void primaryShoot();

		//__________________________________________________________________

		virtual void stopPrimaryShoot();

		//__________________________________________________________________

		virtual void secondaryShoot();

		//__________________________________________________________________

		virtual void stopSecondaryShoot();

		//__________________________________________________________________

		virtual void fireWeapon() { /* Esto esta deprecado */ }

		//__________________________________________________________________

		void forgetFireBall(CEntity* fireBall);

	protected:

		virtual void onTick(unsigned int msecs);

	private:

		bool _primaryFireIsActive;

		bool _secondaryFireIsActive;

		std::set<CEntity*> _controllableFireBalls;
	};

	REG_FACTORY(CIronHellGoat);

} // namespace Logic

#endif // __Logic_Shoot_H
