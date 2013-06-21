#ifndef __Logic_SpellType_H
#define __Logic_SpellType_H

#include <string>

namespace Logic {

	struct SpellType {
		enum Enum {
			eAMPLIFY_DAMAGE,// Activo	// funcionando	(falta feedback)
			eCOME_BACK,		// Activo	// hecho		(falta feedback y test)
			eSCOPE,			// Activo	// funcionando	(falta feedback)
			eGRAVITY,		// Activo	// funcionando	(falta feedback)
			eHARD_DEATH,	// Pasivo	// hecho		(falta feedback y test)
			eSHIELD,		// Activo	// en camino
			eCOOLDOWN,		// Pasivo	// funcionando	(falta feedback)
			eRESURECTION,	// Pasivo	// por hacer
			eSIZE,
		};

		static std::string toString(SpellType::Enum spellType) {
			switch(spellType) {
				
			case eAMPLIFY_DAMAGE:
				return "amplifyDamage";
			case eCOME_BACK:
				return "comeBack";
			case eSCOPE:
				return "scope";
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