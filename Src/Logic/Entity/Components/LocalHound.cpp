#include "LocalHound.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "AvatarController.h"
#include "PhysicController.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageChangeMaterial.h"

namespace Logic{

	IMP_FACTORY(CLocalHound);

	bool CLocalHound::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		//creating the effect and hiding
		_scene = _entity->getMap()->getScene();

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
	//------------------------------------------------------

	void CLocalHound::onActivate(){
		_effect = "berserkCompositor";
		_scene->createCompositor(_effect);
		_scene->setCompositorVisible(_effect, false);
		_strengthEffect = "strength";
		_timestamp = 0;
		_offsetTimeSin = 0;
	}
	//------------------------------------------------------

	bool CLocalHound::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		return msgType == Message::CHANGE_MATERIAL;
	} // accept
	//------------------------------------------------------

	void CLocalHound::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);

		if (materialMsg->getMaterialName() == "original"){
			_scene->setCompositorVisible(_effect, false);
			this->putToSleep();
			// Si funcionara el awake no haria falta
			_timestamp = 0;
		}else{
			_scene->setCompositorVisible(_effect, true);
		}
	}
	//------------------------------------------------------

// Descomentar esta linea para ver los dos modos que se pueden poner (obviamente las variables son seteables)
//*
	void CLocalHound::onWake(){
		_timestamp = 0;
	}
	//------------------------------------------------------

	void CLocalHound::onTick(unsigned int msecs) {

		_timestamp += msecs;
		if(_timestamp < 2200){
			_scene->updateCompositorVariable(_effect, _strengthEffect, _timestamp*0.001);
			//printf("\nDisminuyendo: %d \t le paso: %f y el seno: %f - %f", _timestamp, _timestamp*0.003, sin(_timestamp*0.003)*0.5, sin(_timestamp*0.003)*0.5+1.8);
		}else{
			if(_offsetTimeSin == 0){ _offsetTimeSin = ((_timestamp*0.001) - sin(_timestamp*0.003)*0.5); }
			_scene->updateCompositorVariable(_effect, _strengthEffect, (std::sin(_timestamp*0.003)*0.5)+_offsetTimeSin);
/*/
	void CLocalHound::onWake(){
		_timestamp = 2000;
	}

	void CLocalHound::onTick(unsigned int msecs) {

		_timestamp -= msecs;
		if(_timestamp > 0){
			_scene->updateCompositorVariable(_effect, _strengthEffect, _timestamp*0.001);
			printf("\nDisminuyendo: %d \t\t le paso: %f", _timestamp, _timestamp*0.001);
			
		}else{
			_timestamp = 2000;
/* */
			//this->putToSleep();
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
		
	} // tick
	//----------------------------------------------------------

	void CLocalHound::onFixedTick(unsigned int msecs) {
		if(charge && _doingPrimarySkill){
			Vector3 direction = _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
			direction.normalise();
			direction *= msecs * _biteMaxVelocity;
			_physicController->move(direction, Physics::CollisionGroup::eWORLD, msecs);
		}
	}

	//----------------------------------------------------------

	void CLocalHound::primarySkill() {

		_doingPrimarySkill = true;

		//lo primero de todo cambiamos los valores del avatarController para que nos mueva mucho mas lento
		//_avatarController->setMaxVelocity(_bitetVelocity);
		_avatarController->deactivate();

		_biteTimer = _biteDuration;
	}

}