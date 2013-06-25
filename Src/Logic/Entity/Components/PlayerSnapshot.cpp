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
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Maps/WorldState.h"

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

		return msgType == Message::SET_ANIMATION	|| 
			   msgType == Message::STOP_ANIMATION	||
			   msgType == Message::AUDIO;
	}

	//__________________________________________________________________

	void CPlayerSnapshot::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::SET_ANIMATION: {
				shared_ptr<CMessageSetAnimation> setAnimMsg = static_pointer_cast<CMessageSetAnimation>(message);

				// Struct con información sobre la animación
				AnimInfo info;
				info.tick = _tickCounter;
				info.animName = setAnimMsg->getAnimation();
				info.loop = setAnimMsg->getLoop();
				info.stop = false;
				info.exclude = setAnimMsg->getExclude();

				// informamos al worldState de la ultima animación que está poniendo
				// la entidad, para que si alguien se conecta directamente le ponga
				// la animación que le corresponde
				Logic::CWorldState::getSingletonPtr()->addChange(_entity, message);

				_animationBuffer.push_back(info);

				break;
			}
			case Message::STOP_ANIMATION: {
				shared_ptr<CMessageStopAnimation> stopAnimMsg = static_pointer_cast<CMessageStopAnimation>(message);

				// Struct con información sobre la animación
				AnimInfo info;
				info.tick = _tickCounter;
				info.animName = stopAnimMsg->getString();
				info.loop = false;
				info.stop = true;

				_animationBuffer.push_back(info);

				break;
			}
			case Message::AUDIO: {
				shared_ptr<CMessageAudio> audioMsg = static_pointer_cast<CMessageAudio>(message);

				AudioInfo info;
				info.tick = _tickCounter;
				info.audioName = audioMsg->getAudioName();
				info.loopSound = audioMsg->isLoopable();
				info.play3d = audioMsg->is3dSound();
				info.streamSound = audioMsg->streamSound();

				_audioBuffer.push_back(info);

				break;
			}
		}
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
		//snapshotMsg->setAudioBuffer(_audioBuffer);
		_entity->emitMessage(snapshotMsg);

		// Limpiar el buffer para la siguiente snapshot
		_transformBuffer.clear();
		_animationBuffer.clear();
		//_audioBuffer.clear();
	}

	//__________________________________________________________________

	void CPlayerSnapshot::takeSnapshot() {
		Matrix4 transform;
		transform.makeTransform(_entity->getPosition(),Vector3::UNIT_SCALE,_entity->getOrientation());
		_transformBuffer.push_back( transform );
	}

} // namespace Logic


