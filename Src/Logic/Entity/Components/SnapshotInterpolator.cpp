
/**
@file SnapshotInterpolator.cpp
 
@see Logic::CSnapshotInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "SnapshotInterpolator.h"

// Map
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageTransformSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CSnapshotInterpolator);

	//__________________________________________________________________

	CSnapshotInterpolator::CSnapshotInterpolator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSnapshotInterpolator::~CSnapshotInterpolator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSnapshotInterpolator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		//assert( entityInfo->hasAttribute("interpolationTimestep") );

		// Tiempo de interpolación en msecs
		//_interpolationTimestep = _interpolationTimer = entityInfo->getIntAttribute("interpolationTimestep");
		_interpolationTimestep = _interpolationTimer = 50.0f;

		return true;
	}

	//__________________________________________________________________

	bool CSnapshotInterpolator::accept(const std::shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::TRANSFORM_SNAPSHOT;
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TRANSFORM_SNAPSHOT: {
				std::vector<Matrix4> buffer = static_pointer_cast<CMessageTransformSnapshot>(message)->getBuffer();
				std::cout << "Recibo un buffer de tamano = " << buffer.size() << std::endl;
				break;
			}
		}
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::onTick(unsigned int msecs) {
		_interpolationTimer -= msecs;
		if(_interpolationTimer < 0) {
			_interpolationTimer = _interpolationTimestep;
		}
	}

} // namespace Logic


