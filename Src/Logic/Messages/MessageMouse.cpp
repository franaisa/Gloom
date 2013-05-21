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

	// Esta mal hecho, tendria que llamar al serialize del padre
	// para que hacer lo mismo dos veces?
	Net::CBuffer CMessageMouse::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(float) * 2);
		buffer.serialize(std::string("CMessageMouse"),true);
		buffer.serialize(_seq);
		buffer.serialize(_mouse[0]);
		buffer.serialize(_mouse[1]);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageMouse::deserialize(Net::CBuffer& buffer) {
		_controlType = Control::MOUSE;
		buffer.deserialize(_seq);
		buffer.deserialize(_mouse[0]);
		buffer.deserialize(_mouse[1]);
	}
};
