#ifndef __Logic_SpellType_H
#define __Logic_SpellType_H

#include <string>

namespace Logic {

	struct SpellType {
		enum Enum {
			eAMPLIFY_DAMAGE,// Hecho		(falta feedback y test)
			eCOME_BACK,		//
			eCOOLDOWN,		// Hecho		(falta feedback y test)
			eGRAVITY,		// Hecho		(falta feedback)
			eHARD_DEATH,	// Hecho		(falta feedback y test)
			eHUNGRY,		//
			eSHIELD,		// Hecho		(falta feedback y test)
			eRESURECTION,	//
			eSIZE,
		};

		static std::string toString(SpellType::Enum spellType) {
			switch(spellType) {
				
			case eAMPLIFY_DAMAGE:
				return "amplifyDamage";
			case eCOME_BACK:
				return "comeBack";
			case eCOOLDOWN:
				return "cooldown";
			case eGRAVITY:
				return "gravity";
			case eHARD_DEATH:
				return "hardDeath";
			case eHUNGRY:
				return "hardDeath";
			case eSHIELD:
				return "shield";
			case eRESURECTION:
				return "resurection";
			}
		}
	};

}

#endif