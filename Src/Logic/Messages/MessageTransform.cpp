#include "MessageTransform.h"

namespace Logic {

	CMessageTransform::CMessageTransform(TMessageType type) : CMessage(type) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Matrix4 CMessageTransform::getTransform(){
		return _transform;
	}//
	//----------------------------------------------------------

	void CMessageTransform::setTransform(Matrix4 transform){
		_transform=transform;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageTransform::serialize() {
		assert(_tempBuffer == NULL);

		// IMPLEMENTAR EL ENVIO DEL MATRIX4
		_tempBuffer = new Net::CBuffer( sizeof(float) * 5);
		_tempBuffer->serialize(_transform);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
