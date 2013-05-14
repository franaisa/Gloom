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
			eROCKET_LAUNCHER,
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
				case eROCKET_LAUNCHER:
					return "rocketLauncher";
			}
		}
	};

}

#endif