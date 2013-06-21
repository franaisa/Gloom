//---------------------------------------------------------------------------
// ScopeFeedback.h
//---------------------------------------------------------------------------

/**
@file ScopeFeedback.h

Contiene la declaración de la cabecera del componente
de disparo de la cabra.

@see Logic::CScopeFeedback
@see Logic::ISpellFeedback

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ScopeFeedback_H
#define __Logic_ScopeFeedback_H

#include "Logic/Entity/Components/SpellFeedback.h"

#include <set>
namespace Graphics{
	class CCamera;
}
namespace Logic {

	/**
    @ingroup logicGroup

	Este componente implementa la funcionalidad de la cabra.

	@author Jaime Chapinal Cervantes
	@date JUnio, 2013
	*/

	class CScopeFeedback : public ISpellFeedback {
		DEC_FACTORY(CScopeFeedback);

	public:

		/** Constructor por defecto. */
		CScopeFeedback();

		//__________________________________________________________________

		virtual ~CScopeFeedback();

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

	private:

		Graphics::CCamera* _camera;
	};

	REG_FACTORY(CScopeFeedback);

} // namespace Logic

#endif // __Logic_ComeBackFeedback_H