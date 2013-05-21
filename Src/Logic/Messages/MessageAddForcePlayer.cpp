#include "MessageAddForcePlayer.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddForcePlayer);

	CMessageAddForcePlayer::CMessageAddForcePlayer() : CMessage(Message::ADDFORCEPLAYER) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	void CMessageAddForcePlayer::setForce(Vector3 force){
		_force=force;
	}//
	//----------------------------------------------------------

	Vector3 CMessageAddForcePlayer::getForce(){
		return _force;
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessageAddForcePlayer::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(float));
		buffer.serialize(std::string("CMessageAddForcePlayer"), true);
		buffer.serialize(_force);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_force);
	}

};
