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
	
	Net::CBuffer CMessageTransform::serialize() {
		CMessage::serialize();
		_tempBuffer.write(&_transform, sizeof(_transform));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
