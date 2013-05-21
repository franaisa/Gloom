#include "MessagePositionSnapshot.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePositionSnapshot);

	//__________________________________________________________________

	CMessagePositionSnapshot::CMessagePositionSnapshot() : CMessage(Message::POSITION_SNAPSHOT) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CMessagePositionSnapshot::~CMessagePositionSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessagePositionSnapshot::setBuffer(const std::vector<Vector3> buffer) {
		_positionBuffer = buffer;
	}

	//__________________________________________________________________
		
	std::vector<Vector3> CMessagePositionSnapshot::getBuffer() {
		return _positionBuffer;
	}

	//__________________________________________________________________
		
	Net::CBuffer CMessagePositionSnapshot::serialize() {
		// Tamaño igual = cabecera(int) + tambuffer(int) + num matrices (5 floats * tamBuffer)
		int bufferSize = _positionBuffer.size();
		Net::CBuffer buffer( (2 * sizeof(int)) + (bufferSize * 3 * sizeof(float)) );
		buffer.serialize(std::string("CMessagePositionSnapshot"), true);
		buffer.serialize(bufferSize);
		for(int i = 0; i < bufferSize; ++i) {
			buffer.serialize(_positionBuffer[i]);
		}
		
		return buffer;
	}

	//__________________________________________________________________

	void CMessagePositionSnapshot::deserialize(Net::CBuffer& buffer) {
		int bufferSize;
		// Deserializar el tamaño del buffer
		buffer.deserialize(bufferSize);

		// Resize del buffer al tamaño leido
		_positionBuffer.clear();
		_positionBuffer.resize(bufferSize);
		// Leer tantas matrices como nos diga el tamaño
		for(int i = 0; i < bufferSize; ++i) {
			buffer.deserialize(_positionBuffer[i]);
		}
	}

};
