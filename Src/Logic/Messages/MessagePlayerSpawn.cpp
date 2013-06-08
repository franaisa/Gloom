#include "MessagePlayerSpawn.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerSpawn);
	
	
	CMessagePlayerSpawn::CMessagePlayerSpawn() : CMessage(Message::PLAYER_SPAWN) {
		// Nada que hacer
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::setSpawnPosition(const Vector3& position) {
		_position = position;
	}
	
	//________________________________________________________________________

	Vector3 CMessagePlayerSpawn::getSpawnPosition(){
		return _position;
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::setSpawnOrientation(const Quaternion& orientation) {
		_orientation = orientation;
	}
	
	//________________________________________________________________________

	Quaternion CMessagePlayerSpawn::getSpawnOrientation(){
		return _orientation;
	}
	
	//________________________________________________________________________

	Net::CBuffer CMessagePlayerSpawn::serialize() {
		//3 del vector3 y 4 del quaternion
		Net::CBuffer buffer(sizeof(int) + sizeof(float) * 7);
		buffer.serialize(std::string("CMessagePlayerSpawn"), true);
		buffer.serialize(_position);
		buffer.serialize(_orientation);
		
		return buffer;
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_position);
		buffer.deserialize(_orientation);
	}
};
