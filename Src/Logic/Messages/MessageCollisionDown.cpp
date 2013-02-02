#include "MessageCollisionDown.h"

namespace Logic {

	CMessageCollisionDown::CMessageCollisionDown(TMessageType type): CMessage(type){
		
	}//
	//----------------------------------------------------------

	void CMessageCollisionDown::setCollisionDown(bool collision){
		_collision = collision;
	}//
	//----------------------------------------------------------

	bool CMessageCollisionDown::getCollisionDown(){
		return _collision;
	}//
	Net::CBuffer CMessageCollisionDown::serialize() {
		CMessage::serialize();


		return _tempBuffer;
	}//
	//----------------------------------------------------------

};