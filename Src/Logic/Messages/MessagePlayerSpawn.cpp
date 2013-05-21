#include "MessagePlayerSpawn.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerSpawn);
	
	
	CMessagePlayerSpawn::CMessagePlayerSpawn() : CMessage(Message::PLAYER_SPAWN) {
		// Nada que hacer
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::setSpawnTransform(const Matrix4& transform) {
		_spawnTransform = transform;
	}
	
	//________________________________________________________________________

	Matrix4 CMessagePlayerSpawn::getSpawnTransform(){
		return _spawnTransform;
	}
	
	//________________________________________________________________________

	Net::CBuffer CMessagePlayerSpawn::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(float) * 5);
		buffer.serialize(std::string("CMessagePlayerSpawn"), true);
		buffer.serialize(_spawnTransform);
		
		return buffer;
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spawnTransform);
	}
};
