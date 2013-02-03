#include "MessageMouse.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageMouse);

	CMessageMouse::CMessageMouse(): CMessageControl(){

	} //
	//----------------------------------------------------------
	void CMessageMouse::setMouse(float mouse[]){
		_mouse[0] = mouse[0];
		_mouse[1] = mouse[1];
	}//
	//----------------------------------------------------------

	float* CMessageMouse::getMouse(){
		return _mouse;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageMouse::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(float) * 2);
		_tempBuffer->serialize(std::string("CMessageMouse"));
		_tempBuffer->serialize(_mouse[0]);
		_tempBuffer->serialize(_mouse[1]);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageMouse::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_mouse[0]);
		buffer.deserialize(_mouse[1]);
	}

};
