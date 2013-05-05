#include "LocalHound.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageChangeMaterial.h"

namespace Logic{

	IMP_FACTORY(CLocalHound);

	bool CLocalHound::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		//creating the effect and hiding
		_scene = _entity->getMap()->getScene();
		
		return true;
	} // spawn

	void CLocalHound::onActivate(){
		_effect = "berserkCompositor";
		_scene->createCompositor(_effect);
		_scene->setCompositorVisible(_effect, false);
		_strengthEffect = "strength";
		_timestamp = 0;
		this->putToSleep();
	}

	bool CLocalHound::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		return msgType == Message::CHANGE_MATERIAL;
	} // accept

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

// Descomentar esta linea para ver los dos modos que se pueden poner (obviamente las variables son seteables)
//*
	void CLocalHound::onWake(){
		_timestamp = 0;
	}

	void CLocalHound::onTick(unsigned int msecs) {

		_timestamp += msecs;
		if(_timestamp < 2200){
			_scene->updateCompositorVariable(_effect, _strengthEffect, _timestamp*0.001);
			printf("\nDisminuyendo: %d \t\t le paso: %f", _timestamp, _timestamp*0.001);
		}else{
			_scene->updateCompositorVariable(_effect, _strengthEffect, std::sin(_timestamp*0.001)+2.2);
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
		
	} // tick
	//----------------------------------------------------------

}