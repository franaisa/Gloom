/**
@file Hound.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Hound".
 
@see Logic::CHound
@see Logic::IComponent

@author Francisco Aisa García.
@date Marzo, 2013
*/

#include "Hound.h"

namespace Logic {

	IMP_FACTORY(CHound);

	//__________________________________________________________________

	CHound::CHound() : CPlayerClass("hound") {
		// Nada que hacer
	}

	//__________________________________________________________________

	CHound::~CHound() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CHound::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) return false;

		// Leer atributos propios
	} // spawn

	//__________________________________________________________________

	void CHound::primarySkill() {
		std::cout << "Primary Skill - Hound" << std::endl;
	}

	//__________________________________________________________________

	void CHound::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Hound" << std::endl;
	}

} // namespace Logic

