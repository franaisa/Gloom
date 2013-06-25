/**
@file PlayerClass.cpp

Contiene la implementación del componente que define
el comportamiento general de las habilidades de cada
clase.
 
@see Logic::CPlayerClass
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PlayerClass.h"
#include "Hound.h"
#include "Archangel.h"
#include "Shadow.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageHud.h"
#include "Logic/Maps/WorldState.h"

#include "Logic/Messages/MessageChangePlayerClass.h"
#include "Logic/Messages/MessageControl.h"

namespace Logic {
	//IMP_FACTORY(CPlayerClass);
	
	CPlayerClass::CPlayerClass(const std::string& playerClassName) : _primarySkillTimer(0),
																	 _secondarySkillTimer(0)
																		{
		
		// No hay memoria dinamica que reservar
	}

	//__________________________________________________________________

	CPlayerClass::~CPlayerClass() {
		// No hay memoria dinamica que liberar
	}

	//__________________________________________________________________

	bool CPlayerClass::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Leemos el tiempo de cooldown de la habilidad primaria
		assert( entityInfo->hasAttribute("primarySkillCooldown") && "Error: primarySkillCooldown no esta definido en el mapa" );
		// Pasamos el tiempo a msecs
		_primarySkillCooldown = entityInfo->getFloatAttribute("primarySkillCooldown") * 1000;

		// Leemos el tiempo de cooldown de la habilidad secundaria
		assert( entityInfo->hasAttribute("secondarySkillCooldown") && "Error: secondarySkillCooldown no esta definido en el mapa" );
		// Pasamos el tiempo a msecs
		_secondarySkillCooldown = entityInfo->getFloatAttribute("secondarySkillCooldown") * 1000;

		// Leemos la altura de disparo
		assert( entityInfo->hasAttribute("heightShoot") && "Error: heightShoot no esta definido en el mapa" );
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");

		// Leemos el radio de la cápsula del player
		assert( entityInfo->hasAttribute("physic_radius") && "Error: physic_radius no esta definido en el mapa" );
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");

		return true;
	} // spawn
	
	//__________________________________________________________________

	bool CPlayerClass::accept(const std::shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de skills.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		if(message->getMessageType() == Message::CONTROL) {
			std::shared_ptr<CMessageControl> controlMsg = std::static_pointer_cast<CMessageControl>(message);
			
			ControlType type = controlMsg->getType();
			
			return type == Control::USE_PRIMARY_SKILL		||
				   type == Control::USE_SECONDARY_SKILL		||
				   type == Control::STOP_PRIMARY_SKILL		||
				   type == Control::STOP_SECONDARY_SKILL;
		}

		return false;
	} // accept

	//__________________________________________________________________

	void CPlayerClass::onDeactivate() {
		stopPrimarySkill();
		stopSecondarySkill();
	}// onDeactivate
	//__________________________________________________________________

	void CPlayerClass::onStart() {
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("original");
		_entity->emitMessage(materialMsg);
		Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
	}
	//__________________________________________________________________

	void CPlayerClass::process(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		switch(msgType) {
			case Message::CONTROL: {
				ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

				if(type == Control::USE_PRIMARY_SKILL) {
					if( canUsePrimarySkill() ) {
						primarySkill();
						std::shared_ptr<CMessageHud> hudmsg = std::make_shared<CMessageHud>();
						hudmsg->setType(CMessageHud::HudType::PRIMARY_SKILL);
						_entity->emitMessage(hudmsg);
						_primarySkillTimer = _primarySkillCooldown;
					}
				}
				else if(type == Control::USE_SECONDARY_SKILL) {
					if( canUseSecondarySkill() ) {
						secondarySkill();
						std::shared_ptr<CMessageHud> hudmsg = std::make_shared<CMessageHud>();
						hudmsg->setType(CMessageHud::HudType::SECONDARY_SKILL);
						_entity->emitMessage(hudmsg);
						_secondarySkillTimer = _secondarySkillCooldown;
					}
				}
				else if(type == Control::STOP_PRIMARY_SKILL) {
					stopPrimarySkill();
				}
				else if(type == Control::STOP_SECONDARY_SKILL) {
					stopSecondarySkill();
				}

				break;
			}
		}
	} // process

	//__________________________________________________________________

	void CPlayerClass::onTick(unsigned int msecs) {
		// Controlamos el cooldown de la habilidad primaria y secundaria
		if(_primarySkillTimer > 0) {
			_primarySkillTimer -= msecs;
			
			if(_primarySkillTimer < 0)
				_primarySkillTimer = 0;
		}

		if(_secondarySkillTimer > 0) {
			_secondarySkillTimer -= msecs;

			if(_secondarySkillTimer < 0)
				_secondarySkillTimer = 0;
		}
	} // tick

	//__________________________________________________________________

	void CPlayerClass::onActivate() {
		// Restea el valor de los timers
		_primarySkillTimer = _secondarySkillTimer = 0;
	}

	//__________________________________________________________________

	void CPlayerClass::stopPrimarySkill() {
		// Si las clases hijas no sobreescriben este método su invoación
		// es ignorada
	}

	//__________________________________________________________________

	void CPlayerClass::stopSecondarySkill() {
		// Si las clases hijas no sobreescriben este método su invoación
		// es ignorada
	}

	//__________________________________________________________________

	bool CPlayerClass::canUsePrimarySkill() {
		return _primarySkillTimer == 0;
	}

	//__________________________________________________________________

	bool CPlayerClass::canUseSecondarySkill() {
		return _secondarySkillTimer == 0;
	}

} // namespace Logic

