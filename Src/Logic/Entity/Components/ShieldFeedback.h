//---------------------------------------------------------------------------
// ShieldFeedback.h
//---------------------------------------------------------------------------

/**
@file ShieldFeedback.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CShieldFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ShieldFeedback_H
#define __Logic_ShieldFeedback_H

#include "Logic/Entity/Components/SpellFeedback.h"

#include <set>

namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Jaime Chapinal Cervantes
	@date JUnio, 2013
	*/

	class CShieldFeedback : public ISpellFeedback {
		DEC_FACTORY(CShieldFeedback);

	public:

		/** Constructor por defecto. */
		CShieldFeedback();

		//__________________________________________________________________

		virtual ~CShieldFeedback();

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

	REG_FACTORY(CShieldFeedback);

} // namespace Logic

#endif // __Logic_ComeBackFeedback_H