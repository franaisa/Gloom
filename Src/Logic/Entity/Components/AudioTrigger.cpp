/**
@file AudioTrigger.cpp

@see Logic::CAudioTrigger
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "AudioTrigger.h"

#include "Net/Manager.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Audio/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageAudio.h"

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
				// @deprecated
				// La forma buena de hacer esto no es con la comprobación que voy a meter si
				// no teniendo unos componentes mejor programados, pero como ya estamos al final
				// me voy a meter el lujo de meter este pequeno hack para solucionar la papeleta
				// del sonido online.
				// Si somos el servidor queremos mandar un mensaje de audio.
				if( Net::CManager::getSingletonPtr()->imServer() ) {
					shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
					
					audioMsg->setAudioName(_name);
					audioMsg->is3dSound(true);
					audioMsg->isLoopable(false);
					audioMsg->streamSound(false);

					_entity->emitMessage(audioMsg);
				}
				else {
					Audio::CServer::getSingletonPtr()->playSound3D(_name, _entity->getPosition(), Vector3::ZERO, false, _stream);
				}

				break;
			}
			/*case Message::UNTOUCHED: {
				Audio::CServer::getSingletonPtr()->stopSound(_name);
				break;
			}*/
		}
	} // process

} // namespace Logic


