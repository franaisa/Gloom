/**
@file Shadow.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Shadow".
 
@see Logic::CShadow
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Shadow.h"

namespace Logic {

	IMP_FACTORY(CShadow);

	//__________________________________________________________________

	CShadow::CShadow() : CPlayerClass("shadow") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CShadow::~CShadow() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CShadow::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		// Leer atributos propios
	} // spawn

	//__________________________________________________________________

	void CShadow::primarySkill() {
		std::cout << "Primary Skill - Shadow" << std::endl;
	}

	//__________________________________________________________________

	void CShadow::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Shadow" << std::endl;
	}

} // namespace Logic

