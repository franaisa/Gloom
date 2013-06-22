/**
@file PositionSnapshot.cpp
 
@see Logic::CPositionSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PositionSnapshot.h"
#include "Logic/Messages/MessagePositionSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CPositionSnapshot);

	CPositionSnapshot::CPositionSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CPositionSnapshot::~CPositionSnapshot() {
		// Nada que hacer
	}

	//__________________________________________________________________

	void CPositionSnapshot::sendSnapshot() {
		// Enviar un mensaje con el buffer
		shared_ptr<CMessagePositionSnapshot> snapshotMsg = make_shared<CMessagePositionSnapshot>();
		snapshotMsg->setBuffer(_positionBuffer);
		_entity->emitMessage(snapshotMsg);

		// Limpiar el buffer para la siguiente snapshot
		_positionBuffer.clear();
	}

	//__________________________________________________________________

	void CPositionSnapshot::takeSnapshot() {
		_positionBuffer.push_back( _entity->getPosition() );
	}

} // namespace Logic


