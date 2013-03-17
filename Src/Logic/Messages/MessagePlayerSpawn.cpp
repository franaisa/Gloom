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

	Net::CBuffer* CMessagePlayerSpawn::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(float) * 5);
		_tempBuffer->serialize(std::string("CMessagePlayerSpawn"), true);
		_tempBuffer->serialize(_spawnTransform);
		
		return _tempBuffer;
	}
	
	//________________________________________________________________________

	void CMessagePlayerSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spawnTransform);
	}
};
