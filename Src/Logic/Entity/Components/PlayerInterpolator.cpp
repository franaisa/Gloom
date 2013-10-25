
/**
@file PlayerInterpolator.cpp
 
@see Logic::CPlayerInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PlayerInterpolator.h"
#include "PhysicController.h"

#include "Physics/Server.h"

// Map
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"

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
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PLAYER_SNAPSHOT;
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
		switch( message->getMessageType() ) {
			case Message::PLAYER_SNAPSHOT: {
				storeSnapshot( static_pointer_cast<CMessagePlayerSnapshot>(message) );
				break;
			}
		}
	}

	//__________________________________________________________________

	void CPlayerInterpolator::storeSnapshot(const shared_ptr<CMessagePlayerSnapshot>& snapshotMsg) {
		//no queremos que se nos <acumulen los samples, asi que limpiamos todos menos el primero
		//(para que la interpolacion haga el resto
		interpolateSnapshot( snapshotMsg->getTransformBuffer() );

		while (!_transformBuffer.empty() && _transformBuffer.size()>_ticksPerBuffer*2 ){
			_transformBuffer.pop_front();
		}

		if(_connecting) {
			// Preparamos el buffer doble
			std::cout << "transform buffer size = " << _transformBuffer.size() << std::endl;
			if(_transformBuffer.size() >= 2 * _ticksPerBuffer)
				_connecting = false;
		}
		
		if (!_animationBuffer.empty()){
			AnimInfo info = _animationBuffer.back();
			_animationBuffer.pop_back();

			// Emitimos el mensaje de animación
			if(info.stop) {
				// Mandar animación de stop
				shared_ptr<CMessageStopAnimation> stopAnimMsg = make_shared<CMessageStopAnimation>();
				stopAnimMsg->setString(info.animName);
				_entity->emitMessage(stopAnimMsg);
			}else {
				// Mandar set animation
				shared_ptr<CMessageSetAnimation> setAnimMsg = make_shared<CMessageSetAnimation>();
				setAnimMsg->setAnimation(info.animName);
				setAnimMsg->setLoop(info.loop);
				setAnimMsg->setExclude(info.exclude);
				setAnimMsg->setRewind(info.rewind);
				_entity->emitMessage(setAnimMsg);
			}
			_animationBuffer.clear();
		}
		
		vector<AnimInfo> tempAnimBuffer = snapshotMsg->getAnimationBuffer();
		_animationBuffer.insert( _animationBuffer.end(), tempAnimBuffer.begin(), tempAnimBuffer.end() );

		if(!_audioBuffer.empty()){
			AudioInfo info = _audioBuffer.back();
			_audioBuffer.pop_front();

			shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
			audioMsg->setAudioName(info.audioName);
			audioMsg->isLoopable(info.loopSound);
			audioMsg->is3dSound(info.play3d);
			audioMsg->streamSound(info.streamSound);
			audioMsg->stopSound(info.stopSound);

			_entity->emitMessage(audioMsg);
			_audioBuffer.clear();
		}
		vector<AudioInfo> tempAudioBuffer = snapshotMsg->getAudioBuffer();
		_audioBuffer.insert( _audioBuffer.end(), tempAudioBuffer.begin(), tempAudioBuffer.end() );


		if(!_weaponBuffer.empty()){
			WeaponInfo info = _weaponBuffer.back();
			shared_ptr<CMessageChangeWeaponGraphics> weaponMsg = make_shared<CMessageChangeWeaponGraphics>();
			weaponMsg->setWeapon(info.weapon);
			_entity->emitMessage(weaponMsg);
			_weaponBuffer.clear();
		}

		
		vector<WeaponInfo> tempWeaponBuffer = snapshotMsg->getWeaponBuffer();
		_weaponBuffer.insert ( _weaponBuffer.end(), tempWeaponBuffer.begin(), tempWeaponBuffer.end() );

		//reiniciamos el contador cada vez que recibimos una snapshot para que no haya marrones
		_tickCounter = 0;
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
			_extrapolatedTicks = 0;

			Matrix4 newTransform = _transformBuffer.front();
			Vector3 newPosition = newTransform.getTrans();
			_extrapolatedMotion = (newPosition - _entity->getPosition())*0.5f;

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
						setAnimMsg->setRewind(info.rewind);
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

			if ( !_weaponBuffer.empty() && _weaponBuffer.front().tick == _tickCounter ) {
				WeaponInfo info = _weaponBuffer.front();
				_weaponBuffer.pop_front();
				shared_ptr<CMessageChangeWeaponGraphics> weaponMsg = make_shared<CMessageChangeWeaponGraphics>();
				weaponMsg->setWeapon(info.weapon);
				_entity->emitMessage(weaponMsg);
			}

			_tickCounter = (_tickCounter + 1) % _ticksPerBuffer;
		}
		else {
			// Estamos ejecutando ticks con el buffer vacio
			// tendremos que descartar estos ticks del siguiente buffer
			// recibido
			if(!_connecting) {
				if( _extrapolatedTicks < _ticksPerBuffer ) {
					std::cout << "perdiendo ticks"+ _lostTicks << std::endl;
					Vector3 newPosition = _entity->getPosition() + _extrapolatedMotion * 0.5f;
					Physics::CapsuleGeometry controllerGeometry = Physics::CapsuleGeometry( _controller->getCapsuleRadius(), _controller->getCapsuleHeight() / 2.0f );

					if( !Physics::CServer::getSingletonPtr()->overlapAny( controllerGeometry, newPosition ) ) {
						_controller->setPhysicPosition( newPosition );
					
						if( ++_extrapolatedTicks == 2 )
							_extrapolatedMotion = Vector3::ZERO;
					}
				}
			}
		}
	}

} // namespace Logic


