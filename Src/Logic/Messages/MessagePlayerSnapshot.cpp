#include "MessagePlayerSnapshot.h"

#include "Logic/Entity/MessageFactory.h"

using namespace std;

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerSnapshot);

	//__________________________________________________________________

	CMessagePlayerSnapshot::CMessagePlayerSnapshot() : CMessage(Message::PLAYER_SNAPSHOT) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessagePlayerSnapshot::~CMessagePlayerSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::setTransformBuffer(const vector<Matrix4>& buffer) {
		_transformBuffer = buffer;
	}

	//__________________________________________________________________
		
	vector<Matrix4> CMessagePlayerSnapshot::getTransformBuffer() {
		return _transformBuffer;
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::setAnimationBuffer(const vector<AnimInfo>& buffer) {
		_animationBuffer = buffer;
	}

	//__________________________________________________________________

	vector<AnimInfo> CMessagePlayerSnapshot::getAnimationBuffer() {
		return _animationBuffer;
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::setAudioBuffer(const std::vector<AudioInfo>& buffer) {
		_audioBuffer = buffer;
	}

	//__________________________________________________________________

	vector<WeaponInfo> CMessagePlayerSnapshot::getWeaponBuffer() {
		return _weaponBuffer;
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::setWeaponBuffer(const std::vector<WeaponInfo> &buffer) {
		_weaponBuffer = buffer;
	}

	//__________________________________________________________________

	std::vector<AudioInfo> CMessagePlayerSnapshot::getAudioBuffer() {
		return _audioBuffer;
	}

	//__________________________________________________________________
		
	Net::CBuffer CMessagePlayerSnapshot::serialize() {
		// Tamaño igual = cabecera(int) + tambuffer(int) + num matrices (5 floats * tamBuffer)
		unsigned int transformBufferSize( _transformBuffer.size() ), animationBufferSize( _animationBuffer.size() ), audioBufferSize( _audioBuffer.size() ), weaponBufferSize( _weaponBuffer.size() );
		// Para mandar una ristra de booleanos usaremos un unico byte
		unsigned char booleanMask;

		Net::CBuffer buffer;
		buffer.serialize(std::string("CMessagePlayerSnapshot"), true);
		
		// Copiamos las posiciones y rotaciones del player
		buffer.serialize(transformBufferSize);
		for(unsigned int i = 0; i < transformBufferSize; ++i) {
			buffer.serialize(_transformBuffer[i]);
		}
		
		booleanMask = 0;

		// Copiamos las animaciones que se hayan producido
		buffer.serialize(animationBufferSize);
		for(unsigned int i = 0; i < animationBufferSize; ++i) {
			buffer.serialize(_animationBuffer[i].tick);
			buffer.serialize(_animationBuffer[i].animName, false);

			booleanMask |= _animationBuffer[i].loop		? (1 << 0) : 0;
			booleanMask |= _animationBuffer[i].stop		? (1 << 1) : 0;
			booleanMask |= _animationBuffer[i].exclude	? (1 << 2) : 0;



			buffer.write( &booleanMask, sizeof(booleanMask) );

			buffer.serialize (_animationBuffer[i].rewind);
		}
		
		booleanMask = 0;

		// Copiamos los sonidos que se hayan reproducido
		buffer.serialize(audioBufferSize);
		for(unsigned int i = 0; i < audioBufferSize; ++i) {
			buffer.serialize(_audioBuffer[i].tick);
			buffer.serialize(_audioBuffer[i].audioName, false);
			
			booleanMask |= _audioBuffer[i].loopSound	? (1 << 0) : 0;
			booleanMask |= _audioBuffer[i].play3d		? (1 << 1) : 0;
			booleanMask |= _audioBuffer[i].streamSound	? (1 << 2) : 0;
			booleanMask |= _audioBuffer[i].stopSound	? (1 << 3) : 0;

			buffer.write( &booleanMask, sizeof(booleanMask) );
		}

		buffer.serialize(weaponBufferSize);
		for(unsigned int i = 0; i < weaponBufferSize; ++i) {
			buffer.serialize(_weaponBuffer[i].tick);
			buffer.serialize(_weaponBuffer[i].weapon);
		}

		return buffer;
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::deserialize(Net::CBuffer& buffer) {
		int transformBufferSize, animationBufferSize, audioBufferSize, weaponBufferSize;
		unsigned char booleanMask;
		// Deserializar el tamaño del buffer
		buffer.deserialize(transformBufferSize);

		// Resize del buffer al tamaño leido
		_transformBuffer.clear();
		_transformBuffer.resize(transformBufferSize);
		// Leer tantas matrices como nos diga el tamaño
		for(int i = 0; i < transformBufferSize; ++i) {
			buffer.deserialize(_transformBuffer[i]);
		}

		// Deserializamos el tamaño del buffer de animaciones
		buffer.deserialize(animationBufferSize);

		// Resize del buffer de animaciones al tamaño leido
		_animationBuffer.clear();
		_animationBuffer.resize(animationBufferSize);
		// Leemos tantos strings como nos diga el tamaño
		for(int i = 0; i < animationBufferSize; ++i) {
			buffer.deserialize(_animationBuffer[i].tick);
			buffer.deserialize(_animationBuffer[i].animName);
			buffer.read( &booleanMask, sizeof(booleanMask) );

			_animationBuffer[i].loop	= booleanMask & (1 << 0);
			_animationBuffer[i].stop	= booleanMask & (1 << 1);
			_animationBuffer[i].exclude	= booleanMask & (1 << 2);

			buffer.deserialize(_animationBuffer[i].rewind);
		}

		buffer.deserialize(audioBufferSize);

		// Resize del buffer de sonido al tamaño leido
		_audioBuffer.clear();
		_audioBuffer.resize(audioBufferSize);
		// Leemos tantos strings como nos diga el tamaño
		for(int i = 0; i < audioBufferSize; ++i) {
			buffer.deserialize(_audioBuffer[i].tick);
			buffer.deserialize(_audioBuffer[i].audioName);
			buffer.read( &booleanMask, sizeof(booleanMask) );

			_audioBuffer[i].loopSound	= booleanMask & (1 << 0);
			_audioBuffer[i].play3d		= booleanMask & (1 << 1);
			_audioBuffer[i].streamSound	= booleanMask & (1 << 2);
			_audioBuffer[i].stopSound	= booleanMask & (1 << 3);
		}

		buffer.deserialize(weaponBufferSize);
		_weaponBuffer.resize(weaponBufferSize);
		for(unsigned int i = 0; i < weaponBufferSize; ++i) {
			buffer.deserialize(_weaponBuffer[i].tick);
			buffer.deserialize(_weaponBuffer[i].weapon);
		}
	}

};
