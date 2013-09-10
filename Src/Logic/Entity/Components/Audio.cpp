/**
@file Audio.cpp

Contiene la implementación del componente que controla el sonido de una entidad.
 
@see Logic::CAudio
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "Audio.h"

#include "../../../Audio/Server.h"

#include "../../Messages/MessageAudio.h"

using namespace std;

namespace Logic {
	IMP_FACTORY(CAudio);

	bool CAudio::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::AUDIO;
	} // accept
	//---------------------------------------------------------

	void CAudio::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::AUDIO: {
				//Recogemos los datos
				std::shared_ptr<CMessageAudio> audioMsg = std::static_pointer_cast<CMessageAudio>(message);

				string name			= audioMsg->getAudioName();
				bool play3dSound	= audioMsg->is3dSound();
				bool loopSound		= audioMsg->isLoopable();
				bool streamSound	= audioMsg->streamSound();
				bool stopSound		= audioMsg->stopSound();
				bool playerOnly		= audioMsg->isPlayerOnlySound();

				Audio::CServer* audioServer = Audio::CServer::getSingletonPtr();
				if(stopSound) {
					auto it = _tracksBeingPlayed.find(name);
					if( it != _tracksBeingPlayed.end() ) {
						audioServer->stopSound( it->second );
						_tracksBeingPlayed.erase(it);
					}
				}
				else {
					// Si el sonido lo reproduce el jugador en primera persona, reproducimos
					// el audio en estereo para evitar problemas, en cualquier otro caso
					// reproducimos el sonido en 3d (si corresponde)
					if( _entity->isPlayer() ) {
						unsigned int channelIndex = audioServer->playSound(name, loopSound, streamSound, this);
						_tracksBeingPlayed[name] = channelIndex;
					}
					else if(!playerOnly) {
						unsigned int channelIndex;

						if(play3dSound)
							channelIndex = audioServer->playSound3D(name, _entity->getPosition(), Vector3::ZERO, loopSound, streamSound, this);
						else
							channelIndex = audioServer->playSound(name, loopSound, streamSound, this);

						_tracksBeingPlayed[name] = channelIndex;
					}
				}

				break;
			}
		}

	} // process

	void CAudio::onDeactivate() {
		Audio::CServer* audioServer = Audio::CServer::getSingletonPtr();
		auto it = _tracksBeingPlayed.begin();
		for(; it != _tracksBeingPlayed.end(); ++it) {
			audioServer->stopSound(it->second);
		}

		_tracksBeingPlayed.clear();
	}

	void CAudio::trackEnd(int channelIndex) {
		auto it = _tracksBeingPlayed.begin();
		for(; it != _tracksBeingPlayed.end(); ++it) {
			if(it->second == channelIndex) {
				_tracksBeingPlayed.erase(it);
				break;
			}
		}
	}

} // namespace Logic


