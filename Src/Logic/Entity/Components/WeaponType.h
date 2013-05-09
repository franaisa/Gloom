#ifndef __Logic_WeaponType_H
#define __Logic_WeaponType_H

#include <string>

namespace Logic {

	struct WeaponType {
		enum Enum {
			eHAMMER,
			eSHOTGUN,
			eMINIGUN,
			eROCKET_LAUNCHER,
			eGRENADE_LAUNCHER,
			eSNIPER
		};

		static std::string toString(WeaponType::Enum weaponType) {
			switch(weaponType) {
				case eHAMMER:
					return "Hammer";
				case eSHOTGUN:
					return "Shotgun";
				case eMINIGUN:
					return "Minigun";
				case eROCKET_LAUNCHER:
					return "RocketLauncher";
				case eGRENADE_LAUNCHER:
					return "GrenadeLauncher";
				case eSNIPER:
					return "Sniper";
			}
		}
	};

}

#endif