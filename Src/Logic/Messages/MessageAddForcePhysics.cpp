#include "MessageAddForcePhysics.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddForcePhysics);

	CMessageAddForcePhysics::CMessageAddForcePhysics() : CMessage(Message::ADD_FORCE_PHYSICS) {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CMessageAddForcePhysics::setForce(const Vector3& force, Physics::ForceMode mode) {
		this->_force = force;
		_forceMode = mode;
	}

	//__________________________________________________________________

	Physics::ForceMode CMessageAddForcePhysics::getForceMode() {
		return _forceMode;
	}

	//__________________________________________________________________

	Vector3 CMessageAddForcePhysics::getForceVector() {
		return _force;
	}

	//__________________________________________________________________


	void CMessageAddForcePhysics::setGravity(bool gravity){
		_gravity = gravity;
	}

	//__________________________________________________________________

	bool CMessageAddForcePhysics::getGravity() {
		return _gravity;
	}

	//__________________________________________________________________


	Net::CBuffer CMessageAddForcePhysics::serialize() {
		// Tamaño de la cabecera (int) + tipo de fuerza (int) + Vector3 (float * 3)
		// + booleano de gravedad (bool).
		Net::CBuffer buffer( (sizeof(int) * 2) + (sizeof(float) * 3) + sizeof(bool) );
		buffer.serialize(std::string("CMessageAddForcePhysics"), true);
		buffer.serialize(_force);
		// No uso la funcion de serializar por ser un enumerado (que dan problemillas).
		buffer.write(&_forceMode, sizeof(_forceMode));
		buffer.serialize(_gravity);

		return buffer;
	}

	//__________________________________________________________________

	void CMessageAddForcePhysics::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_force);
		// Leo directamente por ser un enumerado para evitar problemas
		buffer.read(&_forceMode, sizeof(_forceMode));
		buffer.deserialize(_gravity);
	}

};
