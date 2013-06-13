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

	

	private:

		
	};

	REG_FACTORY(CCoolDownAmmo);

} // namespace Logic

#endif // __Logic_CoolDownAmmo_H