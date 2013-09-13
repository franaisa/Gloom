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
#include "Bite.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/WorldState.h"
#include "Physics/Server.h"
#include "Logic/Server.h"
#include "Logic/Messages/MessageDamageAmplifier.h"
#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageChangeMaterial.h"
#include "Logic/Messages/MessageParticleVisibility.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageHoundCharge.h"
#include "PhysicController.h"
#include "AvatarController.h"
#include "PhysicDynamicEntity.h"

#include "Graphics/poolParticle.h"

namespace Logic {

	IMP_FACTORY(CHound);

	//__________________________________________________________________

	CHound::CHound() : CPlayerClass("hound"), _biteTimer(0), charge(0), _doingPrimarySkill(0), _biteEntity(0) {
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

		/*Map::CEntity* trigger = CEntityFactory::getSingletonPtr()->getInfo("Bite");
		_biteTrigger = CEntityFactory::getSingletonPtr()->createEntity(trigger, map, false);
		_trigger = _biteTrigger->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");*/

		return true;
	} // spawn

	//__________________________________________________________________

	void CHound::onStart(){
		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");
	}

	//________________________________________________________________________

	void CHound::onTick(unsigned int msecs) {
		CPlayerClass::onTick(msecs);

		if(_doingPrimarySkill) {
			if(_biteTimer > 0) {
				_biteTimer -= msecs;
				if(_biteTimer < 0) {
					_biteTimer = 0;
					_doingPrimarySkill = false;

					// Mandamos un mensaje al avatar controller para que
					// vuelva a usar el filtro que corresponde
					std::shared_ptr<CMessageHoundCharge> houndChargeMsg = std::make_shared<CMessageHoundCharge>();
					houndChargeMsg->isActive(false);
					_entity->emitMessage(houndChargeMsg);
					
					// Destruimos la entidad del mordisco
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_biteEntity, false);
					_biteEntity = NULL;
				}
			}
			/*else{
				stopSecondarySkill();
			}*/
		}
	}

	//________________________________________________________________________

	void CHound::onActivate() {
		CPlayerClass::onActivate();

		_berserkerTimer = _berserkerDuration;
		_biteTimer = 0;
		_doingPrimarySkill = _doingSecondarySkill = false;
	}

	//________________________________________________________________________

	void CHound::onDeactivate() {
		if(_biteEntity != NULL)
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_biteEntity, false);
	}

	//__________________________________________________________________

	void CHound::primarySkill() {
		_doingPrimarySkill = true;
		_biteTimer = _biteDuration;

		// Creamos la entidad del mordisco y le indicamos que nosotros somos el owner
		// para que actualice su posicion en funcion de la nuestra
		CEntityFactory* factory = CEntityFactory::getSingletonPtr();
		Map::CEntity* houndTrigger = factory->getInfo("Bite");
		_biteEntity = factory->createEntity(houndTrigger, CServer::getSingletonPtr()->getMap(), _entity->getPosition(), Quaternion::IDENTITY, false);
		_biteEntity->activate();
		_biteEntity->start();

		CBite* biteComponent = _biteEntity->getComponent<CBite>("CBite");
		biteComponent->setOwner(_entity);

		// Queremos desactivar solo las colisiones con los players y los hitboxes
		unsigned int filterMask = Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eHITBOX;
		// Calculamos el complementario
		filterMask = ~filterMask;
		// Eliminamos el filtro del player y los hitboxes usando el complementario
		// con la mascara que use por defecto el controller
		filterMask = _physicController->getDefaultFilterMask() & filterMask;

		filterMask = Physics::CollisionGroup::eWORLD;

		// Mandar un mensaje al avatar controller de empujar al player en la direccion
		// en la que este mirando con el filtro cambiado
		std::shared_ptr<CMessageHoundCharge> houndChargeMsg = std::make_shared<CMessageHoundCharge>();
		houndChargeMsg->setFilterMask(filterMask);
		houndChargeMsg->setForce(5.0f);
		_entity->emitMessage(houndChargeMsg);

		// Emitimos el sonido de carga
		emitSound("character/houndBite.wav", false, true, false, false, false);
	} // primarySkill

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

		emitSound("character/houndSmell.wav", false, true, true, false, false);
	} // secondarySkill

	//__________________________________________________________________

	void CHound::stopSecondarySkill(){

		printf("\n Terminando habilidad");

		auto materialMsg = std::make_shared<CMessageParticleVisibility>();
		materialMsg->setNameParticle("Odor");
		materialMsg->setVisibility(false);
		_entity->emitMessage(materialMsg);

		_doingSecondarySkill = false;

		emitSound("character/houndSmell.wav", false, false, false, true, false);
	}
	

} // namespace Logic

