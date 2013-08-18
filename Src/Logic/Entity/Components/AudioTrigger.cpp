/**
@file AudioTrigger.cpp

@see Logic::CAudioTrigger
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "AudioTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Audio/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"

using namespace std;

namespace Logic {
	IMP_FACTORY(CAudioTrigger);

	CAudioTrigger::CAudioTrigger() {
		// Nada que hacer
	}

	bool CAudioTrigger::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("audioTrigger_name") );
		assert( entityInfo->hasAttribute("audioTrigger_stream") );
		
		_name = entityInfo->getStringAttribute("audioTrigger_name");
		_stream = entityInfo->getBoolAttribute("audioTrigger_stream");

		return true;
	} // spawn

	bool CAudioTrigger::accept(const std::shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::TOUCHED		||
			   msgType == Message::UNTOUCHED;
	} // accept

	void CAudioTrigger::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TOUCHED: {
				Audio::CServer::getSingletonPtr()->playSound3D(_name, _entity->getPosition(), Vector3::ZERO, false, _stream);
				break;
			}
			/*case Message::UNTOUCHED: {
				Audio::CServer::getSingletonPtr()->stopSound(_name);
				break;
			}*/
		}
	} // process

} // namespace Logic


