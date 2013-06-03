//---------------------------------------------------------------------------
// SniperFeedback.h
//---------------------------------------------------------------------------

/**
@file SniperFeedback.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CSniperFeedback
@see Logic::IWeaponFeedback

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_SniperFeedback_H
#define __Logic_SniperFeedback_H

#include "Logic/Entity/Components/WeaponFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CSniperFeedback : public IWeaponFeedback {
		DEC_FACTORY(CSniperFeedback);

	public:

		/** Constructor por defecto. */
		CSniperFeedback();

		//__________________________________________________________________

		virtual ~CSniperFeedback();

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

	REG_FACTORY(CSniperFeedback);

} // namespace Logic

#endif // __Logic_SniperFeedback_H