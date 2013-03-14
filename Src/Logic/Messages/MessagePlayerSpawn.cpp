#include "MessagePlayerSpawn.h"
#include "Logic/Entity/MessageFactory.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerSpawn);
	
	
	CMessagePlayerSpawn::CMessagePlayerSpawn() : CMessage(Message::PLAYER_SPAWN) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessagePlayerSpawn::setSpawnTransform(const Matrix4& transform) {
		_spawnTransform = transform;
	}//
	//----------------------------------------------------------

	Matrix4 CMessagePlayerSpawn::getSpawnTransform(){
		return _spawnTransform;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessagePlayerSpawn::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(float) * 5);
		_tempBuffer->serialize(std::string("CMessagePlayerSpawn"), true);
		_tempBuffer->serialize(_spawnTransform);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerSpawn::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spawnTransform);
	}
};
