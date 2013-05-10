
/**
@file PlayerSnapshot.cpp
 
@see Logic::CPlayerSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PlayerSnapshot.h"
#include "Logic/Messages/MessageTransformSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CPlayerSnapshot);

	CPlayerSnapshot::CPlayerSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CPlayerSnapshot::~CPlayerSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CPlayerSnapshot::sendSnapshot() {
		// Enviar un mensaje con el buffer
		shared_ptr<CMessageTransformSnapshot> snapshotMsg = make_shared<CMessageTransformSnapshot>();
		snapshotMsg->setBuffer(_transformBuffer);
		_entity->emitMessage(snapshotMsg);

		// Limpiar el buffer para la siguiente snapshot
		_transformBuffer.clear();
	}

	//__________________________________________________________________

	void CPlayerSnapshot::takeSnapshot() {
		_transformBuffer.push_back( _entity->getTransform() );
	}

} // namespace Logic


