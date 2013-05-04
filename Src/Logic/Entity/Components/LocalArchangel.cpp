#include "LocalArchangel.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageChangeMaterial.h"

namespace Logic{

	IMP_FACTORY(CLocalArchangel);

	bool CLocalArchangel::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		//creating the effect and hiding
		_scene = _entity->getMap()->getScene();
		

		return true;
	} // spawn

	void CLocalArchangel::onActivate(){
		_effect = "ArchangelShieldFP";
		_scene->createCompositor(_effect);
		_scene->setCompositorVisible(_effect, false);
	}

	bool CLocalArchangel::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		return msgType == Message::CHANGE_MATERIAL;
	} // accept

	void CLocalArchangel::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);

		if (materialMsg->getMaterialName() == "original"){
			_scene->setCompositorVisible(_effect, false);
		}else{
			_scene->setCompositorVisible(_effect, true);
		}
	}

}