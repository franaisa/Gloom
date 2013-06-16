//---------------------------------------------------------------------------
// CoolDownFeedback.h
//---------------------------------------------------------------------------

/**
@file CoolDownFeedback.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CResurrectionFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ResurrectionFeedback_H
#define __Logic_ResurrectionFeedback_H

#include "Logic/Entity/Components/SpellFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CResurrectionFeedback : public ISpellFeedback {
		DEC_FACTORY(CResurrectionFeedback);

	public:

		/** Constructor por defecto. */
		CResurrectionFeedback();

		//__________________________________________________________________

		virtual ~CResurrectionFeedback();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Método virtual que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el spellCast
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell();

		//__________________________________________________________________
	};

	REG_FACTORY(CResurrectionFeedback);

} // namespace Logic

#endif // __Logic_ResurrectionFeedback_H