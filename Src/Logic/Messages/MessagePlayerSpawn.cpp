#include "MessagePlayerSpawn.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerSpawn);
	
	
	CMessagePlayerSpawn::CMessagePlayerSpawn() : CMessage(Message::PLAYER_SPAWN) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessagePlayerSpawn::setSpawnPosition(const Vector3& position) {
		_spawnPosition = position;
	}//
	//----------------------------------------------------------

	Vector3 CMessagePlayerSpawn::getSpawnPosition(){
		return _spawnPosition;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessagePlayerSpawn::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_spawnPosition.x) * 3);
		_tempBuffer->serialize(std::string("CMessagePlayerSpawn"), true);
		_tempBuffer->serialize(_spawnPosition);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spawnPosition);
	}
};
