
/**
@file SnapshotGenerator.cpp
 
@see Logic::CSnapshotGenerator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "SnapshotGenerator.h"

#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic {
	
	//IMP_FACTORY(CSnapshotGenerator);

	CSnapshotGenerator::CSnapshotGenerator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSnapshotGenerator::~CSnapshotGenerator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSnapshotGenerator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute("snapshotsPerSecond") );
		assert( entityInfo->hasAttribute("samplesPerSnapshot") );

		// Cada cuanto tomamos una snapshot
		//_snapshotTimestep = _snapshotTimer = 1000 / entityInfo->getIntAttribute("snapshotsPerSecond");
		// Cuantas muestras tomamos por snapshot
		//_intervalTimestep = _intervalTimer = _snapshotTimer / entityInfo->getIntAttribute("samplesPerSnapshot");

		_snapshotTimestep = _snapshotTimer = 500;
		_intervalTimestep = _intervalTimer = 100;

		return true;
	}

	//__________________________________________________________________

	bool CSnapshotGenerator::accept(const std::shared_ptr<CMessage>& message) {
		return false;
	}

	//__________________________________________________________________

	void CSnapshotGenerator::process(const std::shared_ptr<CMessage>& message) {

	}

	//__________________________________________________________________

	void CSnapshotGenerator::onTick(unsigned int msecs) {
		_intervalTimer -= msecs;
		if(_intervalTimer < 0) {
			_intervalTimer *= -1;
			_intervalTimer += _intervalTimestep;

			// Almacenar datos
			takeSnapshot();
		}

		_snapshotTimer -= msecs;
		if(_snapshotTimer < 0) {
			_snapshotTimer *= -1;
			_snapshotTimer += _snapshotTimestep;

			// Enviar snapshot
			sendSnapshot();
		}
	}

} // namespace Logic


