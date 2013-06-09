//---------------------------------------------------------------------------
// ShotGunFeedback.h
//---------------------------------------------------------------------------

/**
@file ShotGunFeedback.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CShotGunFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_ShotGunFeedback_H
#define __Logic_ShotGunFeedback_H

#include "Logic/Entity/Components/WeaponFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CShotGunFeedback : public IWeaponFeedback {
		DEC_FACTORY(CShotGunFeedback);

	public:

		/** Constructor por defecto. */
		CShotGunFeedback();

		//__________________________________________________________________

		virtual ~CShotGunFeedback();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		// El propio shoot deberia encargarse de llamar a lo que toque teniendo
		// en cuenta la municion y las demas historias
		virtual void primaryFire();

		//__________________________________________________________________

		virtual void stopPrimaryFire();

		//__________________________________________________________________

		virtual void secondaryFire();

	};

	REG_FACTORY(CShotGunFeedback);

} // namespace Logic

#endif // __Logic_ShotGunFeedback_H