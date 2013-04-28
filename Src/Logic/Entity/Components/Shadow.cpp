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
#include "Logic/Messages/MessageChangeMaterialHudWeapon.h"

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

		_invisibilityTimer = 0;

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		// Leer el tiempo que dura la invisibilidad
		assert( entityInfo->hasAttribute("invisibilityDuration") );
		// Pasamos el tiempo a msecs
		_invisibilityDuration = entityInfo->getFloatAttribute("invisibilityDuration") * 1000;
		return true;
	} // spawn

	//__________________________________________________________________

	void CShadow::onTick(unsigned int msecs) {
		// Si la habilidad primaria esta en uso, controlar el tiempo
		// efectivo de la invisibilidad. Cuando se cumpla el tiempo,
		// deshabilitamos el shader de transparencia.
		if(_invisibilityTimer > 0) {
			_invisibilityTimer -= msecs;

			if(_invisibilityTimer < 0) {
				_invisibilityTimer = 0;

				// Desactivamos el shader de invisibilidad
				std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
				materialMsg->setMaterialName("original");
				_entity->emitMessage(materialMsg);

				/*
				std::shared_ptr<CMessageChangeMaterialHudWeapon> materialMsgHud = std::make_shared<CMessageChangeMaterialHudWeapon>();
				materialMsgHud->setMaterialName("original");
				_entity->emitMessage(materialMsgHud);
				*/
			}
		}
	}

	//________________________________________________________________________

	void CShadow::onActivate() {
		CPlayerClass::onActivate();

		// Ñapa temporal para el ideame
		// Cambiamos el color del marine en funcion de la clase con un changeMaterial

		
	}

	//__________________________________________________________________

	void CShadow::onStart(unsigned int msecs) {

		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("original");
		_entity->emitMessage(materialMsg);

		_invisibilityTimer = 1;
	}

	//__________________________________________________________________

	void CShadow::primarySkill() {
		std::cout << "Primary Skill - Shadow" << std::endl;

		// Arrancamos el cronometro
		_invisibilityTimer = _invisibilityDuration;
		
		// Activamos el shader de invisibilidad
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("shadowInvisibility");
		_entity->emitMessage(materialMsg);

		/*
		std::shared_ptr<CMessageChangeMaterialHudWeapon> materialMsgHud = std::make_shared<CMessageChangeMaterialHudWeapon>();
		materialMsgHud->setMaterialName("shadowInvisibility");
		_entity->emitMessage(materialMsgHud);
		*/



	}

	//__________________________________________________________________

	void CShadow::secondarySkill() {
		// Habilidad por definir
		std::cout << "Secondary Skill - Shadow" << std::endl;
	}

} // namespace Logic

