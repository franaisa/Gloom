#include "MessageMouse.h"

namespace Logic {

	CMessageMouse::CMessageMouse(TMessageType t = TMessageType::CONTROL):CMessageControl(t){

	} //
	//----------------------------------------------------------
	void CMessageMouse::setMouse(float mouse[]){
		_mouse[0]=mouse[0];
		_mouse[1]=mouse[1];
	}//
	//----------------------------------------------------------

	float* CMessageMouse::getMouse(){
		return _mouse;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageMouse::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(float) * 2);
		_tempBuffer->serialize(_mouse[0]);
		_tempBuffer->serialize(_mouse[1]);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
