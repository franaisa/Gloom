#include "LocalShadow.h"
#include "Graphics/Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageChangeMaterial.h"

namespace Logic{

	IMP_FACTORY(CLocalShadow);

	bool CLocalShadow::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		//creating the effect and hiding
		_scene = _entity->getMap()->getScene();
		

		return true;
	} // spawn

	void CLocalShadow::onActivate(){
		_distorsion = "Old TV";
		_scene->createCompositor(_distorsion);
		_scene->setCompositorVisible(_distorsion, false);
		_colourEffect = "Old Movie";
		_scene->createCompositor(_colourEffect);
		_scene->setCompositorVisible(_colourEffect, false);
	}

	bool CLocalShadow::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		return msgType == Message::CHANGE_MATERIAL;
	} // accept

	void CLocalShadow::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageChangeMaterial> materialMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);

		if (materialMsg->getMaterialName() == "original"){
			_scene->setCompositorVisible(_distorsion, false);
			_scene->setCompositorVisible(_colourEffect, false);
		}else{
			_scene->setCompositorVisible(_distorsion, true);
			_scene->setCompositorVisible(_colourEffect, true);
		}
	}

}