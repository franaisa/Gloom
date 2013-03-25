/**
@file Archangel.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Archangel".
 
@see Logic::CArchangel
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Archangel.h"

namespace Logic {

	IMP_FACTORY(CArchangel);

	//__________________________________________________________________

	CArchangel::CArchangel() : CPlayerClass("archangel") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CArchangel::~CArchangel() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CArchangel::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		// Leer atributos propios
	} // spawn

	//__________________________________________________________________

	void CArchangel::primarySkill() {
		std::cout << "Primary Skill - Archangel" << std::endl;
	}

	//__________________________________________________________________

	void CArchangel::secondarySkill() {
		std::cout << "Secondary Skill - Archangel" << std::endl;
	}

} // namespace Logic

