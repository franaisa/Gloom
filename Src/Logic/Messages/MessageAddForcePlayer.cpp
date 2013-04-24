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

	Net::CBuffer* CMessageAddForcePlayer::serialize() {
		assert(_tempBuffer == NULL);
		//
		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(float));
		_tempBuffer->serialize(std::string("CMessageAddForcePlayer"), true);
		_tempBuffer->serialize(_force);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_force);
	}

};
