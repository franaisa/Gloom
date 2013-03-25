/**
@file Screamer.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Screamer".
 
@see Logic::CScreamer
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Screamer.h"

namespace Logic {

	IMP_FACTORY(CScreamer);

	//__________________________________________________________________

	CScreamer::CScreamer() : CPlayerClass("screamer") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CScreamer::~CScreamer() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CScreamer::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		// Leer atributos propios
	} // spawn

	//__________________________________________________________________

	void CScreamer::primarySkill() {
		std::cout << "Primary Skill - Screamer" << std::endl;
	}

	//__________________________________________________________________

	void CScreamer::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Screamer" << std::endl;
	}

} // namespace Logic

