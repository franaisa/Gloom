#include "MessageAddForcePlayer.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddForcePlayer);

	CMessageAddForcePlayer::CMessageAddForcePlayer() : CMessage(Message::ADDFORCEPLAYER) {
		// Nada que hacer
	} //
	//----------------------------------------------------------
	float CMessageAddForcePlayer::getPower(){
		return _power;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::setPower(float power){
		_power=power;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::setVelocity(float velocity){
		_velocity=velocity;;
	}//
	//----------------------------------------------------------

	float CMessageAddForcePlayer::getVelocity(){
		return _velocity;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::setDirection(Vector3 direction){
		_direction=direction;
	}//
	//----------------------------------------------------------

	Vector3 CMessageAddForcePlayer::getDirection(){
		return _direction;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageAddForcePlayer::serialize() {
		assert(_tempBuffer == NULL);
		//
		_tempBuffer = new Net::CBuffer(sizeof(int) + (sizeof(float) * 5));
		_tempBuffer->serialize(std::string("CMessageAddForcePlayer"), true);
		_tempBuffer->serialize(_power);
		_tempBuffer->serialize(_velocity);
		_tempBuffer->serialize(_direction);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAddForcePlayer::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_power);
		buffer.deserialize(_velocity);
		buffer.deserialize(_direction);
	}

};
