/**
@file Hound.cpp

Contiene la implementación de la clase que
implementa las habilidades del personaje
"Hound".
 
@see Logic::CHound
@see Logic::IComponent

@author Jose Antonio García Yáñez.
@author Rubén Mulero Guerrero.
@date Marzo, 2013
*/

#include "Hound.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/WorldState.h"
#include "Physics/Server.h"
#include "Logic/Messages/MessageDamageAmplifier.h"
#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageParticleVisibility.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"
#include "PhysicController.h"
#include "AvatarController.h"
#include "PhysicDynamicEntity.h"

#include "Graphics/poolParticle.h"

namespace Logic {

	IMP_FACTORY(CHound);

	//__________________________________________________________________

	CHound::CHound() : CPlayerClass("hound"), _biteTimer(0), charge(0), _doingPrimarySkill(0) {
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

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		// Leer el tiempo que dura el Berserker
		assert( entityInfo->hasAttribute("berserkerDuration") );
		// Pasamos el tiempo a msecs
		_berserkerDuration = entityInfo->getFloatAttribute("berserkerDuration") * 1000;

		assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		_maxDefaultVelocity = entityInfo->getFloatAttribute("maxVelocity");

		assert( entityInfo->hasAttribute("biteMaxVelocity") && "Error: No se ha definido el atributo biteMaxVelocity en el mapa" );
		_biteMaxVelocity = entityInfo->getFloatAttribute("biteMaxVelocity");

		assert( entityInfo->hasAttribute("biteVelocity") && "Error: No se ha definido el atributo biteVelocity en el mapa" );
		_bitetVelocity = entityInfo->getFloatAttribute("biteVelocity");

		assert( entityInfo->hasAttribute("biteDuration") && "Error: No se ha definido el atributo biteDuration en el mapa" );
		_biteDuration = entityInfo->getFloatAttribute("biteDuration") * 1000;

		assert( entityInfo->hasAttribute("biteDamage") && "Error: No se ha definido el atributo biteDamage en el mapa" );
		_biteDamage = entityInfo->getFloatAttribute("biteDamage");

		//_berserkerDamagePercent = entityInfo->getFloatAttribute("berserkerDamagePercent");
		//_berserkerCooldownPercent = entityInfo->getFloatAttribute("berserkerCooldownPercent");

		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");


		Map::CEntity* trigger = CEntityFactory::getSingletonPtr()->getInfo("Bite");

		_biteTrigger = CEntityFactory::getSingletonPtr()->createEntity(trigger, map, false);
		_trigger = _biteTrigger->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");

		return true;
	} // spawn

	//__________________________________________________________________

	void CHound::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		if(_doingPrimarySkill){
			if(_biteTimer > 0) {
				_biteTimer -= msecs;
				_trigger->setPosition(_entity->getPosition(),true);
			}else{
				stopSecondarySkill();
			}
		}
	}

	void CHound::onStart(unsigned int msecs){
		_biteTrigger->deactivate();
	}

	//________________________________________________________________________

	/*void CHound::onFixedTick(unsigned int msecs) {
		if(charge && _doingPrimarySkill){
			Vector3 direction = _entity->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
			direction.normalise();
			direction *= msecs * _biteMaxVelocity;
			_physicController->move(direction, Physics::CollisionGroup::eWORLD, msecs);
		}
	}*/

	//________________________________________________________________________

	void CHound::onActivate() {
		CPlayerClass::onActivate();

		_berserkerTimer = _berserkerDuration;

		_doingSecondarySkill = false;

		if ( _biteTrigger->isActivated() )
			_biteTrigger->deactivate();
	}


	bool CHound::accept(const std::shared_ptr<CMessage>& message) {
		return CPlayerClass::accept(message) ||
			message->getMessageType() == Message::TOUCHED;
	}

	void CHound::process(const std::shared_ptr<CMessage>& message){
		CPlayerClass::process(message);
		if ( message->getMessageType() == Message::TOUCHED){
			std::shared_ptr<CMessageTouched> msg = std::static_pointer_cast<CMessageTouched>(message);
			CEntity* entity = msg->getEntity();

			std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
			damage->setEnemy(_entity);
			damage->setDamage(_biteDamage);
			entity->emitMessage(damage);
		}
	}

	//__________________________________________________________________

	void CHound::primarySkill() {
		_doingPrimarySkill = true;
		_biteTimer = _biteDuration;
		_physicController->deactivateSimulation();
	} // primarySkill

	//__________________________________________________________________

	void CHound::stopPrimarySkill() {
		_doingPrimarySkill = false;
		
		_biteTimer = 0;
		_physicController->activateSimulation();
	}

	//__________________________________________________________________

	void CHound::secondarySkill() {
		// Habilidad por definir

		printf("\n haciendo habilidad");

		//Arrancamos el cronometro
		_berserkerTimer= _berserkerDuration;

		auto materialMsg = std::make_shared<CMessageParticleVisibility>();
		materialMsg->setNameParticle("Odor");
		materialMsg->setVisibility(true);
		_entity->emitMessage(materialMsg);

		_doingSecondarySkill = true;
	} // secondarySkill
	//__________________________________________________________________

	void CHound::stopSecondarySkill(){

		printf("\n Terminando habilidad");

		auto materialMsg = std::make_shared<CMessageParticleVisibility>();
		materialMsg->setNameParticle("Odor");
		materialMsg->setVisibility(false);
		_entity->emitMessage(materialMsg);

		_doingSecondarySkill = false;
	}
	

} // namespace Logic

