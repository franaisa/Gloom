#include "DynamicAudio.h"

#include "Audio/Server.h"
#include "Logic/Messages/MessageAudio.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CDynamicAudio);

	//__________________________________________________________________

	CDynamicAudio::CDynamicAudio() : _previousPosition(Vector3::ZERO),
									 _speedVector(Vector3::ZERO) {
	}

	//__________________________________________________________________

	CDynamicAudio::~CDynamicAudio() {
		// Se entiende que deactivate se llama antes de llamar al
		// destructor, por lo que paramos los sonidos en ese metodo
	}

	//__________________________________________________________________

	bool CDynamicAudio::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::AUDIO;
	} // accept
	
	//__________________________________________________________________

	void CDynamicAudio::process(const std::shared_ptr<CMessage>& message) {
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

				if(stopSound) {
					auto it = _tracksBeingPlayed.find(name);
					if( it != _tracksBeingPlayed.end() ) {
						_audioServer->stopSound( it->second );
						_tracksBeingPlayed.erase(it);
					}
				}
				else {
					// Si el sonido lo reproduce el jugador en primera persona, reproducimos
					// el audio en estereo para evitar problemas, en cualquier otro caso
					// reproducimos el sonido en 3d (si corresponde)
					if( _entity->isPlayer() ) {
						unsigned int channelIndex = _audioServer->playSound(name, loopSound, streamSound, this);
						_tracksBeingPlayed[name] = channelIndex;
					}
					else if(!playerOnly) {
						unsigned int channelIndex;
						
						if(play3dSound)
							channelIndex = _audioServer->playSound3D(name, _entity->getPosition(), _speedVector, loopSound, streamSound, this);
						else
							channelIndex = _audioServer->playSound(name, loopSound, streamSound, this);

						_tracksBeingPlayed[name] = channelIndex;
					}
				}

				break;
			}
		}

	} // process

	//__________________________________________________________________

	void CDynamicAudio::onActivate() {
		_previousPosition = _entity->getPosition();
	}

	//__________________________________________________________________

	void CDynamicAudio::onDeactivate() {
		Audio::CServer* audioServer = Audio::CServer::getSingletonPtr();
		auto it = _tracksBeingPlayed.begin();
		for(; it != _tracksBeingPlayed.end(); ++it) {
			audioServer->stopSound(it->second);
		}

		_tracksBeingPlayed.clear();
	}

	//__________________________________________________________________

	void CDynamicAudio::onStart() {
		_audioServer = Audio::CServer::getSingletonPtr();
	}

	//__________________________________________________________________

	void CDynamicAudio::onTick(unsigned int msecs) {
		Vector3 currentPosition = _entity->getPosition();
		_speedVector = (currentPosition - _previousPosition) / (msecs * 0.001f);
		_previousPosition = currentPosition;

		// Actualizamos la posicion de todos los sonidos que se esten reproduciendo
		auto it = _tracksBeingPlayed.begin();
		for(; it != _tracksBeingPlayed.end(); ++it) {
			_audioServer->update3DAttributes(it->second, currentPosition, _speedVector);
		}
	}

	//__________________________________________________________________

	void CDynamicAudio::trackEnd(int channelIndex) {
		auto it = _tracksBeingPlayed.begin();
		for(; it != _tracksBeingPlayed.end(); ++it) {
			if(it->second == channelIndex) {
				_tracksBeingPlayed.erase(it);
				break;
			}
		}
	}

} // namespace Logic


