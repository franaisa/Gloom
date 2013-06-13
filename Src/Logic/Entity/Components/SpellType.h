#ifndef __Logic_SpellType_H
#define __Logic_SpellType_H

#include <string>

namespace Logic {

	struct SpellType {
		enum Enum {
			eAMPLIFY_DAMAGE,
			eCOME_BACK,
			eZOOM,
			eGRAVITY,  // hecho
			eHARD_DEATH,
			eSHIELD,
			eCOOLDOWN,
			eRESURECTION,
			eSIZE,
		};

		static std::string toString(SpellType::Enum spellType) {
			switch(spellType) {
				
			case eAMPLIFY_DAMAGE:
				return "amplifyDamage";
			case eCOME_BACK:
				return "comeBack";
			case eZOOM:
				return "zoom";
			case eGRAVITY:
				return "gravity";
			case eHARD_DEATH:
				return "hardDeath";
			case eSHIELD:
				return "shield";
			case eCOOLDOWN:
				return "cooldown";
			case eRESURECTION:
				return "resurection";
			}
		}
	};

}

#endif