#ifndef __Logic_SpellType_H
#define __Logic_SpellType_H

#include <string>

namespace Logic {

	struct SpellType {
		enum Enum {
			eAMPLIFY_DAMAGE,// Hecho		(falta feedback y test)
			eCOOLDOWN,		// Hecho		(falta feedback y test)
			eSIZE,
		};

		static std::string toString(SpellType::Enum spellType) {
			switch(spellType) {
				
			case eAMPLIFY_DAMAGE:
				return "amplifyDamage";
			case eCOOLDOWN:
				return "cooldown";
			}
		}
	};

}

#endif