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

	std::vector<AudioInfo> CMessagePlayerSnapshot::getAudioBuffer() {
		return _audioBuffer;
	}

	//__________________________________________________________________
		
	Net::CBuffer CMessagePlayerSnapshot::serialize() {
		// Tamaño igual = cabecera(int) + tambuffer(int) + num matrices (5 floats * tamBuffer)
		unsigned int transformBufferSize( _transformBuffer.size() ), animationBufferSize( _animationBuffer.size() ), audioBufferSize( _audioBuffer.size() );

		Net::CBuffer buffer;
		buffer.serialize(std::string("CMessagePlayerSnapshot"), true);
		
		// Copiamos las posiciones y rotaciones del player
		buffer.serialize(transformBufferSize);
		for(unsigned int i = 0; i < transformBufferSize; ++i) {
			buffer.serialize(_transformBuffer[i]);
		}
		
		// Copiamos las animaciones que se hayan producido
		buffer.serialize(animationBufferSize);
		for(unsigned int i = 0; i < animationBufferSize; ++i) {
			buffer.serialize(_animationBuffer[i].tick);
			buffer.serialize(_animationBuffer[i].animName, false);
			buffer.write(&_animationBuffer[i].loop, sizeof(bool));
			buffer.write(&_animationBuffer[i].stop, sizeof(bool));
			buffer.write(&_animationBuffer[i].exclude, sizeof(bool));
		}
		
		// Copiamos los sonidos que se hayan reproducido
		buffer.serialize(audioBufferSize);
		for(unsigned int i = 0; i < audioBufferSize; ++i) {
			buffer.serialize(_audioBuffer[i].tick);
			buffer.serialize(_audioBuffer[i].audioName, false);
			buffer.write(&_audioBuffer[i].loopSound, sizeof(bool));
			buffer.write(&_audioBuffer[i].play3d, sizeof(bool));
			buffer.write(&_audioBuffer[i].streamSound, sizeof(bool));
		}

		return buffer;
	}

	//__________________________________________________________________

	void CMessagePlayerSnapshot::deserialize(Net::CBuffer& buffer) {
		int transformBufferSize, animationBufferSize, audioBufferSize;
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
			buffer.read(&_animationBuffer[i].loop, sizeof(bool));
			buffer.read(&_animationBuffer[i].stop, sizeof(bool));
			buffer.read(&_animationBuffer[i].exclude, sizeof(bool));
		}

		buffer.deserialize(audioBufferSize);

		// Resize del buffer de sonido al tamaño leido
		_audioBuffer.clear();
		_audioBuffer.resize(audioBufferSize);
		// Leemos tantos strings como nos diga el tamaño
		for(int i = 0; i < audioBufferSize; ++i) {
			buffer.deserialize(_audioBuffer[i].tick);
			buffer.deserialize(_audioBuffer[i].audioName);
			buffer.read(&_audioBuffer[i].loopSound, sizeof(bool));
			buffer.read(&_audioBuffer[i].play3d, sizeof(bool));
			buffer.read(&_audioBuffer[i].streamSound, sizeof(bool));
		}
	}

};
