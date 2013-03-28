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
#include "Graphics.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

#include "Logic/Messages/MessageChangeMaterial.h"

#include <assert.h>

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

		_invisibilityTimer=0;

		// Leer el tiempo que dura la invisibilidad
		assert( entityInfo->hasAttribute("shadowInvisibilityDuration") );
		// Pasamos el tiempo a msecs
		_invisibilityDuration = entityInfo->getFloatAttribute("shadowInvisibilityDuration") * 1000;

	} // spawn

	//__________________________________________________________________

	void CShadow::tick(unsigned int msecs) {
		CPlayerClass::tick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// efectivo de la invisibilidad. Cuando se cumpla el tiempo,
		// deshabilitamos el shader de transparencia.
		if(_invisibilityTimer > 0) {
			_invisibilityTimer -= msecs;

			if(_invisibilityTimer < 0) {
				_invisibilityTimer = 0;

				// Desactivamos el shader de invisibilidad
				CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
				materialMsg->setMaterialName("marine");
				_entity->emitMessage(materialMsg);
			}
		}
	}

	//__________________________________________________________________

	void CShadow::primarySkill() {
		std::cout << "Primary Skill - Shadow" << std::endl;

		// Arrancamos el cronometro
		_invisibilityTimer = _invisibilityDuration;
		
		// Activamos el shader de invisibilidad
		CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
		materialMsg->setMaterialName("shadowInvisibility");
		_entity->emitMessage(materialMsg);
	}

	//__________________________________________________________________

	void CShadow::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Shadow" << std::endl;
	}

} // namespace Logic

