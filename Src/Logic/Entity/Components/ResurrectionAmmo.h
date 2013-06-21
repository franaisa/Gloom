//---------------------------------------------------------------------------
// ResurrectionAmmo.h
//---------------------------------------------------------------------------

/**
@file ResurrectionAmmo.h

@see Logic::CResurrectionAmmo
@see Logic::ISpellAmmo

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
*/

#ifndef __Logic_ResurrectionAmmo_H
#define __Logic_ResurrectionAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CResurrectionAmmo : public ISpellAmmo {
		DEC_FACTORY(CResurrectionAmmo);
	public:

		/** Constructor por defecto. */
		CResurrectionAmmo();

		//__________________________________________________________________

		virtual ~CResurrectionAmmo();

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

	REG_FACTORY(CResurrectionAmmo);

} // namespace Logic

#endif // __Logic_HardDeathAmmo_H