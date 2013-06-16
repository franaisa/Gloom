//---------------------------------------------------------------------------
// HardDeath.h
//---------------------------------------------------------------------------

/**
@file HardDeathAmmo.h

@see Logic::CHardDeathAmmo
@see Logic::ISpellAmmo

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
*/

#ifndef __Logic_HardDeathAmmo_H
#define __Logic_HardDeathAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CHardDeathAmmo : public ISpellAmmo {
		DEC_FACTORY(CHardDeathAmmo);
	public:

		/** Constructor por defecto. */
		CHardDeathAmmo();

		//__________________________________________________________________

		virtual ~CHardDeathAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________	

	private:
		
	};

	REG_FACTORY(CHardDeathAmmo);

} // namespace Logic

#endif // __Logic_HardDeathAmmo_H