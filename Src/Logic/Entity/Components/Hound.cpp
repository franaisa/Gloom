/**
@file Hound.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Hound".
 
@see Logic::CHound
@see Logic::IComponent

@author Jose Antonio García Yáñez.
@date Marzo, 2013
*/

#include "Hound.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

#include "Logic/Messages/MessageBerserker.h"
#include "Logic/Messages/MessageChangeMaterial.h"

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

		_berserkerTimer=0;

		// Leer el tiempo que dura el Berserker
		assert( entityInfo->hasAttribute("berserkerDuration") );
		// Pasamos el tiempo a msecs
		_berserkerDuration = entityInfo->getFloatAttribute("berserkerDuration") * 1000;
		_berserkerDamagePercent = entityInfo->getFloatAttribute("berserkerDamagePercent");
		_berserkerCooldownPercent = entityInfo->getFloatAttribute("berserkerCooldownPercent");
		
	} // spawn

	//__________________________________________________________________

	void CHound::tick(unsigned int msecs) {
		CPlayerClass::tick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// de duración de la habilidad. Cuando se cumpla el tiempo,
		// deshabilitamos el shader y el efecto beserker.
		if(_berserkerTimer > 0) {
			_berserkerTimer -= msecs;

			if(_berserkerTimer < 0) {
				_berserkerTimer = 0;

				// Volvemos a los valores de daño y cd's originales
				//Ponemos los valores de daño y cd's del berserker (mensaje con porcentajes de incremento y reduccion respecto al original)
				CMessageBerserker* berserkerMsg = new CMessageBerserker();
				berserkerMsg->setPercentCooldown(0);
				berserkerMsg->setPercentDamage(0);
				_entity->emitMessage(berserkerMsg);
			}
		}
	}

	//________________________________________________________________________

	void CHound::activate() {
		CPlayerClass::activate();

		// Ñapa temporal para el ideame
		// Cambiamos el color del marine en funcion de la clase con un changeMaterial
		CMessageChangeMaterial* materialMsg = new CMessageChangeMaterial();
		materialMsg->setMaterialName("marine_rojo");
		_entity->emitMessage(materialMsg);

		_berserkerTimer = 0;
	}

	//__________________________________________________________________

	void CHound::primarySkill() {
		std::cout << "Primary Skill - Hound" << std::endl;

		//Arrancamos el cronometro
		_berserkerTimer= _berserkerDuration;

		//Ponemos los valores de daño y cd's del berserker (mensaje con porcentajes de incremento y reduccion respecto al original)
		CMessageBerserker* berserkerMsg = new CMessageBerserker();
		berserkerMsg->setPercentCooldown(_berserkerCooldownPercent);
		berserkerMsg->setPercentDamage(_berserkerDamagePercent);
		_entity->emitMessage(berserkerMsg);
	}

	//__________________________________________________________________

	void CHound::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Hound" << std::endl;
	}

} // namespace Logic

