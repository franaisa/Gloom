/**
@file Archangel.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Archangel".
 
@see Logic::CArchangel
@see Logic::IComponent

@author Francisco Aisa García.
@author Jaime Chapinal Cervantes
@date Marzo, 2013
*/

#include "Archangel.h"
#include "Graphics.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

#include "Logic/Messages/MessageSetReducedDamage.h"
#include "Logic/Messages/MessageChangeMaterial.h"

#include <assert.h>

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
		if( !CPlayerClass::spawn(entity,map,entityInfo) ) 
			return false;

		// Leer atributos propios
		_inmuneTimer = 0;

		// Leer el tiempo que dura la invisibilidad
		assert( entityInfo->hasAttribute("archangelPrimarySkillCooldown") );
		// Pasamos el tiempo a msecs
		_inmuneDuration = entityInfo->getFloatAttribute("archangelPrimarySkillCooldown") * 1000;
		
		return true; //Para quitar el warning?
	} // spawn

	//__________________________________________________________________

	void CArchangel::tick(unsigned int msecs) {
		CPlayerClass::tick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// efectivo siendo inmune. Cuando se cumpla el tiempo,
		// deshabilitamos la reducción de daño
		if(_inmuneTimer > 0) {
			std::cout << "Inmune" << _inmuneTimer << std::endl; //Para probar el tiempo mientras el que es inmune
			_inmuneTimer -= msecs;

			if(_inmuneTimer < 0) {
				_inmuneTimer = 0;

				// Seteamos la reducción de daño a 0 de manera que recibimos los daños normales
				CMessageSetReducedDamage* pReducedDmgMsg = new CMessageSetReducedDamage();
				pReducedDmgMsg->setReducedDamage(0);
				_entity->emitMessage(pReducedDmgMsg);

				// Desactivamos el shader de inmunidad
				CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
				materialMsg->setMaterialName("marine");
				_entity->emitMessage(materialMsg);
			}
		}
	}

	//________________________________________________________________________

	void CArchangel::activate() {
		CPlayerClass::activate();

		// Ñapa temporal para el ideame
		// Cambiamos el color del marine en funcion de la clase con un changeMaterial
		CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
		materialMsg->setMaterialName("marine_amarillo");
		_entity->emitMessage(materialMsg);

		_inmuneTimer = 0;
	}

	//__________________________________________________________________

	void CArchangel::primarySkill() {
		std::cout << "Primary Skill - Archangel" << std::endl;

		// Arrancamos el cronometro
		_inmuneTimer = _inmuneDuration;

		// Seteamos la reducción de daño al máximo de manera que no se reciban daños
		CMessageSetReducedDamage* pReducedDmgMsg = new CMessageSetReducedDamage();
		pReducedDmgMsg->setReducedDamage(1);
		_entity->emitMessage(pReducedDmgMsg);

		// Activamos el shader de inmunidad
		CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
		materialMsg->setMaterialName("shadowInvisibility"); // En el futuro debe ser el material del archangel
		_entity->emitMessage(materialMsg);
	}

	//__________________________________________________________________

	void CArchangel::secondarySkill() {
		std::cout << "Secondary Skill - Archangel" << std::endl;
	}

} // namespace Logic

