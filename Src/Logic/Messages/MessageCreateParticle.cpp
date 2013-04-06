#include "MessageCreateParticle.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageCreateParticle);

	CMessageCreateParticle::CMessageCreateParticle() : CMessage(Message::CREATE_PARTICLE) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageCreateParticle::setParticle(const std::string &name){
		_name = name;
	}//
	//----------------------------------------------------------

	Vector3 CMessageCreateParticle::getPosition(){
		return _position;
	}//

	//----------------------------------------------------------

	void CMessageCreateParticle::setPosition(const Vector3 &pos){
		_position = pos;
	}//
	//----------------------------------------------------------

	std::string CMessageCreateParticle::getParticle(){
		return _name;
	}//

	//----------------------------------------------------------
		
	Net::CBuffer* CMessageCreateParticle::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_name)+ sizeof(_position));
		_tempBuffer->serialize(std::string("CMessageCreateParticle"),true);
		_tempBuffer->serialize(_name, false);
		_tempBuffer->serialize(_position);
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageCreateParticle::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_name);
		buffer.deserialize(_position);
	}

};
