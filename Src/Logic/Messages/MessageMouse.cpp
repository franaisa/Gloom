#include "MessageMouse.h"

namespace Logic {

	CMessageMouse::CMessageMouse(TMessageType t):CMessageControl(t){

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

	Net::CBuffer CMessageMouse::serialize() {
		CMessageControl::serialize();
		_tempBuffer.write(&_mouse, 2 * sizeof(float));
		_tempBuffer.reset();

		return _tempBuffer;
	}//
	//----------------------------------------------------------

};
