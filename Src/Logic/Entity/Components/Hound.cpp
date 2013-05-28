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
#include "Logic/Maps/WorldState.h"
#include "Physics/Server.h"
#include "Logic/Messages/MessageDamageAmplifier.h"
#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageParticleVisibility.h"
#include "PhysicController.h"
#include "AvatarController.h"

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
		//_berserkerDamagePercent = entityInfo->getFloatAttribute("berserkerDamagePercent");
		//_berserkerCooldownPercent = entityInfo->getFloatAttribute("berserkerCooldownPercent");

		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
		_avatarController = _entity->getComponent<CAvatarController>("CAvatarController");

		return true;
	} // spawn

	//__________________________________________________________________

	void CHound::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		// Si la habilidad primaria esta en uso, controlar el tiempo
		// de duración de la habilidad. Cuando se cumpla el tiempo,
		// deshabilitamos el shader y el efecto beserker.
		/*
		if(_doingPrimarySkill){
			if(_berserkerTimer > 0) {
				_berserkerTimer -= msecs;
			}else{
				// Volvemos a los valores de daño y cd's originales
				//Ponemos los valores de daño y cd's del berserker (mensaje con porcentajes de incremento y reduccion respecto al original)
 				std::shared_ptr<CMessageDamageAmplifier> damageAmplifierMsg = std::make_shared<CMessageDamageAmplifier>();
				std::shared_ptr<CMessageReducedCooldown> reducedCdMsg = std::make_shared<CMessageReducedCooldown>();
				reducedCdMsg->setPercentCooldown(0);
				damageAmplifierMsg->setPercentDamage(0);
				_entity->emitMessage(damageAmplifierMsg);
				_entity->emitMessage(reducedCdMsg);
	
				//le seteamos el material al material por defecto
				std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
				materialMsg->setMaterialName("original");
				_entity->emitMessage(materialMsg);
				Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
				_berserkerTimer = _berserkerDuration;
				_doingPrimarySkill = false;
			}
		}
		*/

		if(_doingSecondarySkill){
			if(_biteTimer > 0) {
				_biteTimer -= msecs;
			}else{
				stopSecondarySkill();
			}
		}

		if(_doingPrimarySkill){
			if(_biteTimer > _biteDuration*0.5) {
				_biteTimer -= msecs;
				
			}else if(_biteTimer > 0) {
				_biteTimer -= msecs;

				charge = true;
			}else{
				stopPrimarySkill();
				charge = false;
				_doingPrimarySkill = false;
				_avatarController->activate();
			}
		}
	}

	void CHound::onFixedTick(unsigned int msecs) {
		if(charge && _doingPrimarySkill){
			Vector3 direction = Math::getDirection(_entity->getOrientation()).normalisedCopy();
				direction *= msecs * _biteMaxVelocity;
				_physicController->move(direction, Physics::CollisionGroup::eWORLD, msecs);
		}
	}

	//________________________________________________________________________

	void CHound::onActivate() {
		CPlayerClass::onActivate();

		_berserkerTimer = _berserkerDuration;

		_doingSecondarySkill = false;
	}

	//__________________________________________________________________

	void CHound::primarySkill() {

		_doingPrimarySkill = true;

		//lo primero de todo cambiamos los valores del avatarController para que nos mueva mucho mas lento
		//_avatarController->setMaxVelocity(_bitetVelocity);
		_avatarController->deactivate();

		_biteTimer = _biteDuration;
		/*
		//Arrancamos el cronometro
		_berserkerTimer= _berserkerDuration;

		
		//Ponemos los valores de daño y cd's del berserker (mensaje con porcentajes de incremento y reduccion respecto al original)
		std::shared_ptr<CMessageDamageAmplifier> damageAmplifierMsg = std::make_shared<CMessageDamageAmplifier>();
		std::shared_ptr<CMessageReducedCooldown> reducedCdMsg = std::make_shared<CMessageReducedCooldown>();
		reducedCdMsg->setPercentCooldown(_berserkerCooldownPercent);
		damageAmplifierMsg->setPercentDamage(_berserkerDamagePercent);
		_entity->emitMessage(damageAmplifierMsg);
		_entity->emitMessage(reducedCdMsg);

		//le cambiamos el material para que empiece a brillar
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::make_shared<CMessageChangeMaterial>();
		materialMsg->setMaterialName("berserk");
		_entity->emitMessage(materialMsg);
		Logic::CWorldState::getSingletonPtr()->addChange(_entity,materialMsg);
		_doingPrimarySkill = true;
		*/
	} // primarySkill

	//__________________________________________________________________

	void CHound::stopPrimarySkill() {
		/*_doingPrimarySkill = false;
		_avatarController->setMaxVelocity(_maxDefaultVelocity);
		charge = false;
		_biteTimer = 0;*/
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

	void CHound::endTimeSecondarySkill(){

		printf("\n Terminando habilidad");

		auto materialMsg = std::make_shared<CMessageParticleVisibility>();
		materialMsg->setNameParticle("Odor");
		materialMsg->setVisibility(false);
		_entity->emitMessage(materialMsg);

		_doingSecondarySkill = false;
	}
	

} // namespace Logic

