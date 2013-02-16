#include "MessageSyncPosition.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSyncPosition);
	
	
	CMessageSyncPosition::CMessageSyncPosition() : CMessage(TMessageType::SYNC_POSITION) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::setTransform(const Matrix4& transform) {
		this->_transform = transform;
	}//
	//----------------------------------------------------------

	Matrix4 CMessageSyncPosition::getTransform(){
		return _transform;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageSyncPosition::serialize() {
		assert(_tempBuffer == NULL);

		// El tamaño de 3 floats para el Vector3 de direccion y 
		// 2 floats para la orientacion (yaw y pitch)
		_tempBuffer = new Net::CBuffer( sizeof(int) + (sizeof(float) * 5) );
		_tempBuffer->serialize(std::string("CMessageSyncPosition"), true);
		_tempBuffer->serialize(_transform);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageSyncPosition::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_transform);
	}
};
