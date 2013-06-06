/**
@file PlayerSnapshot.cpp
 
@see Logic::CPlayerSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#include "PlayerSnapshot.h"

#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CPlayerSnapshot);

	//__________________________________________________________________

	CPlayerSnapshot::CPlayerSnapshot() : _tickCounter(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CPlayerSnapshot::~CPlayerSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CPlayerSnapshot::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !CSnapshotGenerator::spawn(entity, map, entityInfo) ) return false;

		_ticksPerBuffer = _samplesPerSnapshot + ( (_samplesPerSnapshot - 1) * (_ticksPerSample - 1) );

		return true;
	}

	//__________________________________________________________________

	bool CPlayerSnapshot::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_ANIMATION || msgType == Message::STOP_ANIMATION;
	}

	//__________________________________________________________________

	void CPlayerSnapshot::process(const shared_ptr<CMessage>& message) {
		// Struct con información sobre la animación
		AnimInfo info;

		// Anotamos el tick en el que hemos recibido el mensaje de animación
		info.tick = _tickCounter;
		switch( message->getMessageType() ) {
			case Message::SET_ANIMATION: {
				shared_ptr<CMessageSetAnimation> setAnimMsg = static_pointer_cast<CMessageSetAnimation>(message);

				info.animName = setAnimMsg->getAnimation();
				info.loop = setAnimMsg->getLoop();
				info.stop = false;

				break;
			}
			case Message::STOP_ANIMATION: {
				shared_ptr<CMessageStopAnimation> stopAnimMsg = static_pointer_cast<CMessageStopAnimation>(message);

				info.animName = stopAnimMsg->getString();
				info.loop = false;
				info.stop = true;

				break;
			}
		}

		_animationBuffer.push_back(info);
	}

	//__________________________________________________________________

	void CPlayerSnapshot::onFixedTick(unsigned int msecs) {
		CSnapshotGenerator::onFixedTick(msecs);

		_tickCounter = (_tickCounter + 1) % _ticksPerBuffer;
	}

	//__________________________________________________________________

	void CPlayerSnapshot::sendSnapshot() {
		// Enviar un mensaje con el buffer
		shared_ptr<CMessagePlayerSnapshot> snapshotMsg = make_shared<CMessagePlayerSnapshot>();
		snapshotMsg->setTransformBuffer(_transformBuffer);
		snapshotMsg->setAnimationBuffer(_animationBuffer);
		_entity->emitMessage(snapshotMsg);

		// Limpiar el buffer para la siguiente snapshot
		_transformBuffer.clear();
		_animationBuffer.clear();
	}

	//__________________________________________________________________

	void CPlayerSnapshot::takeSnapshot() {
		_transformBuffer.push_back( _entity->getTransform() );
	}

} // namespace Logic


