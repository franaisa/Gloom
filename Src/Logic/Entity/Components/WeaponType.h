#ifndef __Logic_WeaponType_H
#define __Logic_WeaponType_H

#include <string>

namespace Logic {

	struct WeaponType {
		enum Enum {
			eSOUL_REAPER,
			eSNIPER,
			eSHOTGUN,
			eMINIGUN,
			eIRON_HELL_GOAT,
			eSIZE
		};

		static std::string toString(WeaponType::Enum weaponType) {
			switch(weaponType) {
				case eSOUL_REAPER:
					return "soulReaper";
				case eSNIPER:
					return "sniper";
				case eSHOTGUN:
					return "shotGun";
				case eMINIGUN:
					return "miniGun";
				case eIRON_HELL_GOAT:
					return "ironHellGoat";
			}
		}
	};

}

#endif