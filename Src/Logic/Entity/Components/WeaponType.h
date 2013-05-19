#ifndef __Logic_WeaponType_H
#define __Logic_WeaponType_H

#include <string>

namespace Logic {

	struct WeaponType {
		enum Enum {
			eHAMMER,
			eSNIPER,
			eSHOTGUN,
			eMINIGUN,
			eIRON_HELL_GOAT,
			eSIZE
		};

		static std::string toString(WeaponType::Enum weaponType) {
			switch(weaponType) {
				case eHAMMER:
					return "hammer";
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