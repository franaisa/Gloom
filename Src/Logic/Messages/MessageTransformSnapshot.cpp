#include "MessageTransformSnapshot.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageTransformSnapshot);

	//__________________________________________________________________

	CMessageTransformSnapshot::CMessageTransformSnapshot() : CMessage(Message::TRANSFORM_SNAPSHOT) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessageTransformSnapshot::~CMessageTransformSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageTransformSnapshot::setBuffer(const std::vector<Matrix4> buffer) {
		_transformBuffer = buffer;
	}

	//__________________________________________________________________
		
	std::vector<Matrix4> CMessageTransformSnapshot::getBuffer() {
		return _transformBuffer;
	}

	//__________________________________________________________________
		
	Net::CBuffer* CMessageTransformSnapshot::serialize() {
		assert(_tempBuffer == NULL);

		// Tamaño igual = cabecera(int) + tambuffer(int) + num matrices (5 floats * tamBuffer)
		int bufferSize = _transformBuffer.size();
		_tempBuffer = new Net::CBuffer( (2 * sizeof(int)) + (bufferSize * 5 * sizeof(float)) );
		_tempBuffer->serialize(std::string("CMessageTransformSnapshot"), true);
		_tempBuffer->serialize(bufferSize);
		for(int i = 0; i < bufferSize; ++i) {
			_tempBuffer->serialize(_transformBuffer[i]);
		}
		
		return _tempBuffer;
	}

	//__________________________________________________________________

	void CMessageTransformSnapshot::deserialize(Net::CBuffer& buffer) {
		int bufferSize;
		// Deserializar el tamaño del buffer
		buffer.deserialize(bufferSize);

		// Resize del buffer al tamaño leido
		_transformBuffer.clear();
		_transformBuffer.resize(bufferSize);
		// Leer tantas matrices como nos diga el tamaño
		for(int i = 0; i < bufferSize; ++i) {
			buffer.deserialize(_transformBuffer[i]);
		}
	}

};
