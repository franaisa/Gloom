
/**
@file TransformSnapshot.cpp
 
@see Logic::CTransformSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "TransformSnapshot.h"
#include "Logic/Messages/MessageTransformSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CTransformSnapshot);

	CTransformSnapshot::CTransformSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CTransformSnapshot::~CTransformSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CTransformSnapshot::sendSnapshot() {
		// Enviar un mensaje con el buffer
		shared_ptr<CMessageTransformSnapshot> snapshotMsg = make_shared<CMessageTransformSnapshot>();
		snapshotMsg->setBuffer(_transformBuffer);
		_entity->emitMessage(snapshotMsg);

		// Limpiar el buffer para la siguiente snapshot
		_transformBuffer.clear();
	}

	//__________________________________________________________________

	void CTransformSnapshot::takeSnapshot() {
		Matrix4 transform;
		transform.makeTransform(_entity->getPosition(),Vector3::UNIT_SCALE,_entity->getOrientation());
		_transformBuffer.push_back( transform );
	}

	//__________________________________________________________________

} // namespace Logic


