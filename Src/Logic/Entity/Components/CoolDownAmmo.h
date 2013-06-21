//---------------------------------------------------------------------------
// CoolDown.h
//---------------------------------------------------------------------------

/**
@file CoolDown.h

@see Logic::CCoolDown
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_CoolDownAmmo_H
#define __Logic_CoolDownAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CCoolDownAmmo : public ISpellAmmo {
		DEC_FACTORY(CCoolDownAmmo);
	public:

		/** Constructor por defecto. */
		CCoolDownAmmo();

		//__________________________________________________________________

		virtual ~CCoolDownAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell();	

	private:

		
	};

	REG_FACTORY(CCoolDownAmmo);

} // namespace Logic

#endif // __Logic_CoolDownAmmo_H