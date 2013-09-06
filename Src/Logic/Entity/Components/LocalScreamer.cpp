/**
@file LocalScreamer.cpp

Contiene la implementación del componente que controla los efectos de
cámara de la entidad screamer.

@author Javier Fernández Villanueva
@date Agosto, 2013
*/
#include "LocalScreamer.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageChangeMaterial.h"

namespace Logic{

	IMP_FACTORY(CLocalScreamer);

	bool CLocalScreamer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		//creating the effect and hiding
		_scene = _entity->getMap()->getScene();
		
		return true;
	} // spawn

	void CLocalScreamer::onActivate(){
		_effect = "ArchangelShieldFP";
		_scene->createCompositor(_effect);
		_scene->setCompositorVisible(_effect, false);
		_strengthEffect = "strength";
		_timestamp = 0;
	}
	//------------------------------------------------------

	bool CLocalScreamer::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		return msgType == Message::CHANGE_MATERIAL;
	} // accept

	void CLocalScreamer::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);

		if (materialMsg->getMaterialName() == "original"){
			_scene->setCompositorVisible(_effect, false);
			this->putToSleep();
			//esto no deberia de estar
		}else{
			_scene->setCompositorVisible(_effect, true);
			_timestamp = 1300;
		}
	}
	
	void CLocalScreamer::onWake(){
		_timestamp = 1300;
	}
	//------------------------------------------------------

	void CLocalScreamer::onTick(unsigned int msecs) {

		_timestamp -= msecs;
		if(_timestamp > 100){
			_scene->updateCompositorVariable(_effect, _strengthEffect, _timestamp*0.01);
			
		}else{
			_timestamp = 1300;
			this->putToSleep();
		}
		
	} // tick
	//----------------------------------------------------------

}