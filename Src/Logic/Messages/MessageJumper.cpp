#include "MessageJumper.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageJumper);

	CMessageJumper::CMessageJumper() : CMessage(TMessageType::JUMPER) {
		// Nada que hacer
	} //
	//----------------------------------------------------------
	float CMessageJumper::getPower(){
		return _power;
	}//
	//----------------------------------------------------------

	void CMessageJumper::setPower(float power){
		_power=power;
	}//
	//----------------------------------------------------------

	void CMessageJumper::setVelocity(float velocity){
		_velocity=velocity;;
	}//
	//----------------------------------------------------------

	float CMessageJumper::getVelocity(){
		return _velocity;
	}//
	//----------------------------------------------------------

	void CMessageJumper::setDirection(Vector3 direction){
		_direction=direction;
	}//
	//----------------------------------------------------------

	Vector3 CMessageJumper::getDirection(){
		return _direction;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageJumper::serialize() {
		assert(_tempBuffer == NULL);
		//
		_tempBuffer = new Net::CBuffer(sizeof(int) + (sizeof(float) * 5));
		_tempBuffer->serialize(std::string("CMessageJumper"), true);
		_tempBuffer->serialize(_power);
		_tempBuffer->serialize(_velocity);
		_tempBuffer->serialize(_direction);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageJumper::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_power);
		buffer.deserialize(_velocity);
		buffer.deserialize(_direction);
	}

};
