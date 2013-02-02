#include "MessageCollisionDown.h"

namespace Logic {

	CMessageCollisionDown::CMessageCollisionDown(TMessageType type = TMessageType::COLLISION_DOWN): CMessage(type){
		
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