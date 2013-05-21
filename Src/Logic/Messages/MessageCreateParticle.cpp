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

	Vector3 CMessageCreateParticle::getDirectionWithForce(){
		return _directionWithForce;
	}//

	//----------------------------------------------------------

	void CMessageCreateParticle::setDirectionWithForce(const Vector3 &directionWithForce){
		_directionWithForce = directionWithForce;
	}//
	//----------------------------------------------------------


	std::string CMessageCreateParticle::getParticle(){
		return _name;
	}//

	//----------------------------------------------------------
		
	Net::CBuffer CMessageCreateParticle::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_name)+ sizeof(_position));
		buffer.serialize(std::string("CMessageCreateParticle"),true);
		buffer.serialize(_name, false);
		buffer.serialize(_position);
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageCreateParticle::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_name);
		buffer.deserialize(_position);
	}

};
