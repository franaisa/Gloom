
/**
@file PlayerInterpolator.cpp
 
@see Logic::CPlayerInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PlayerInterpolator.h"
#include "PhysicController.h"

// Map
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessageAudio.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CPlayerInterpolator);

	//__________________________________________________________________

	CPlayerInterpolator::CPlayerInterpolator() : _tickCounter(0),
												 _lostTicks(0),
												 _extrapolatedTicks(0),
												 _connecting(true),
												 _extrapolatedMotion(Vector3::ZERO) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CPlayerInterpolator::~CPlayerInterpolator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CPlayerInterpolator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute("ticksPerSample") );
		assert( entityInfo->hasAttribute("samplesPerSnapshot") );

		// Cada cuantos fixed ticks tomamos una muestra
		_ticksPerSample = entityInfo->getIntAttribute("ticksPerSample");
		// Cada cuantas muestras tomamos una snapshot
		_samplesPerSnapshot = entityInfo->getIntAttribute("samplesPerSnapshot");
		// Número de ticks por buffer interpolado
		_ticksPerBuffer = _samplesPerSnapshot + ( (_samplesPerSnapshot - 1) * (_ticksPerSample - 1) );

		return true;
	}

	//__________________________________________________________________

	bool CPlayerInterpolator::accept(const shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::PLAYER_SNAPSHOT;
	}

	//__________________________________________________________________

	void CPlayerInterpolator::interpolateSnapshot(const vector<Matrix4>& buffer) {
		Matrix4 incrementMatrix, temp;
		unsigned int bufferSize = buffer.size();
		
		// Interpolamos las posiciones intermedias del buffer
		for(int i = 0; i < bufferSize - 1; ++i) {
			// Insertamos la posicion inicial
			_transformBuffer.push_back(buffer[i]);
			
			// Calculamos el incremento para las posiciones interpoladas
			incrementMatrix = (buffer[i+1] - buffer[i]) * (1.0f / (float)_ticksPerSample);
			// Generamos las posiciones intermedias
			temp = buffer[i] + incrementMatrix;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
                _transformBuffer.push_back(temp);
                temp = temp + incrementMatrix;
            }
		}

		// Insertamos la ultima posicion
		_transformBuffer.push_back( buffer[bufferSize - 1] );
	}

	//__________________________________________________________________

	void CPlayerInterpolator::process(const shared_ptr<CMessage>& message) {
		shared_ptr<CMessagePlayerSnapshot> snapshotMsg = static_pointer_cast<CMessagePlayerSnapshot>(message);
		
		interpolateSnapshot( snapshotMsg->getTransformBuffer() );

		if(_connecting) {
			// Preparamos el buffer doble
			if(_transformBuffer.size() == 2 * _ticksPerBuffer)
				_connecting = false;
		}

		vector<AnimInfo> tempAnimBuffer = snapshotMsg->getAnimationBuffer();
		_animationBuffer.insert( _animationBuffer.end(), tempAnimBuffer.begin(), tempAnimBuffer.end() );

		vector<AudioInfo> tempAudioBuffer = snapshotMsg->getAudioBuffer();
		_audioBuffer.insert( _audioBuffer.end(), tempAudioBuffer.begin(), tempAudioBuffer.end() );

		// Si hemos perdido ticks, los descartamos del buffer
		if(_lostTicks > 0) {
			// Comprobamos el buffer de transforms
			unsigned int transformBufferSize = _transformBuffer.size();

			if( _lostTicks >= transformBufferSize ) {
				_transformBuffer.clear();
				_lostTicks -= transformBufferSize;
			}
			else {
				_transformBuffer.erase( _transformBuffer.begin(), _transformBuffer.begin() + _lostTicks );
				_lostTicks = 0;
			}
		}
	}

	//__________________________________________________________________

	void CPlayerInterpolator::onStart() {
		_controller = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_controller != NULL && "Error: Como piensas interpolar al player si no tiene controlador fisico?");
	}

	//__________________________________________________________________

	void CPlayerInterpolator::onFixedTick(unsigned int msecs) {
		if( !_transformBuffer.empty() && !_connecting ) {
			// Posicionamos el grafico y el controlador fisico donde nos indique
			// el buffer de posiciones interpolado
			Matrix4 newTransform = _transformBuffer.front();
			Vector3 newPosition = newTransform.getTrans();
			_extrapolatedMotion = newPosition - _entity->getPosition();

			_controller->setPhysicPosition( newPosition );
			_entity->setOrientation( newTransform.extractQuaternion() );
			_transformBuffer.pop_front();

			if( !_animationBuffer.empty() ) {
				if( _animationBuffer.front().tick == _tickCounter ) {
					// Reproducir animacion
					AnimInfo info = _animationBuffer.front();
					_animationBuffer.pop_front();

					// Emitimos el mensaje de animación
					if(info.stop) {
						// Mandar animación de stop
						shared_ptr<CMessageStopAnimation> stopAnimMsg = make_shared<CMessageStopAnimation>();
						stopAnimMsg->setString(info.animName);

						_entity->emitMessage(stopAnimMsg);
					}
					else {
						// Mandar set animation
						shared_ptr<CMessageSetAnimation> setAnimMsg = make_shared<CMessageSetAnimation>();
						setAnimMsg->setAnimation(info.animName);
						setAnimMsg->setLoop(info.loop);
						setAnimMsg->setExclude(info.exclude);
						_entity->emitMessage(setAnimMsg);
					}
				}
			}

			if( !_audioBuffer.empty() ) {
				if( _audioBuffer.front().tick == _tickCounter ) {
					AudioInfo info = _audioBuffer.front();
					_audioBuffer.pop_front();

					shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
					audioMsg->setAudioName(info.audioName);
					audioMsg->isLoopable(info.loopSound);
					audioMsg->is3dSound(info.play3d);
					audioMsg->streamSound(info.streamSound);
					audioMsg->stopSound(info.stopSound);

					_entity->emitMessage(audioMsg);
				}
			}

			_tickCounter = (_tickCounter + 1) % _ticksPerBuffer;
		}
		else {
			// Estamos ejecutando ticks con el buffer vacio
			// tendremos que descartar estos ticks del siguiente buffer
			// recibido
			if(!_connecting) {
				++_lostTicks;
				
				if( _extrapolatedTicks < 2) {
					_controller->setPhysicPosition( _entity->getPosition() + _extrapolatedMotion );
					
					if( ++_extrapolatedTicks == 2 )
						_extrapolatedMotion = Vector3::ZERO;
				}
			}
		}
	}

} // namespace Logic


