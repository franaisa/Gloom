
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

	// Arranco en 1 para tomar la snapshot de la primera posicion, las demas
	// ya van solas
	CSnapshotGenerator::CSnapshotGenerator() : _ticksPerSampleCounter(1), 
											   _samplesPerSnapshotCounter(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CSnapshotGenerator::~CSnapshotGenerator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CSnapshotGenerator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute("ticksPerSample") );
		assert( entityInfo->hasAttribute("samplesPerSnapshot") );

		// Cada cuantos fixed ticks tomamos una muestra
		_ticksPerSample = entityInfo->getIntAttribute("ticksPerSample");
		// Cada cuantas muestras tomamos una snapshot
		_samplesPerSnapshot = entityInfo->getIntAttribute("samplesPerSnapshot");

		return true;
	}

	//__________________________________________________________________

	void CSnapshotGenerator::onFixedTick(unsigned int msecs) {
		if(--_ticksPerSampleCounter == 0) {
			takeSnapshot();
			_ticksPerSampleCounter = _ticksPerSample;
			
			if(++_samplesPerSnapshotCounter == _samplesPerSnapshot) {
				sendSnapshot();
				_samplesPerSnapshotCounter = 0;
			}
		}
	}

} // namespace Logic


