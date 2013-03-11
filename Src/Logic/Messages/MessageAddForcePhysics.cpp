#include "MessageAddForcePhysics.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddForcePhysics);

	CMessageAddForcePhysics::CMessageAddForcePhysics() : CMessage(Message::ADD_FORCE_PHYSICS) {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageAddForcePhysics::setForceVector(const Vector3& force){
		this->_force = force;
	}

	//__________________________________________________________________

	Vector3 CMessageAddForcePhysics::getForceVector() {
		return _force;
	}

	//__________________________________________________________________


	void CMessageAddForcePhysics::setGravity(bool gravity){
		_gravity=gravity;
	}

	//__________________________________________________________________

	bool CMessageAddForcePhysics::getGravity() {
		return _gravity;
	}

	//__________________________________________________________________


	Net::CBuffer* CMessageAddForcePhysics::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + (sizeof(float) * 3));
		_tempBuffer->serialize(std::string("CMessageAddForcePhysics"), true);
		_tempBuffer->serialize(_force);
		_tempBuffer->serialize(_gravity);

		return _tempBuffer;
	}

	//__________________________________________________________________

	void CMessageAddForcePhysics::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_force);
		buffer.deserialize(_gravity);
	}

};
