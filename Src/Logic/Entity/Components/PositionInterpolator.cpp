
/**
@file PositionInterpolator.cpp
 
@see Logic::CPositionInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "PositionInterpolator.h"

// Map
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessagePositionSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CPositionInterpolator);

	//__________________________________________________________________

	CPositionInterpolator::CPositionInterpolator() : _lostTicks(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CPositionInterpolator::~CPositionInterpolator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CPositionInterpolator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
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

	bool CPositionInterpolator::accept(const shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::POSITION_SNAPSHOT;
	}

	//__________________________________________________________________

	void CPositionInterpolator::interpolateSnapshot(const vector<Vector3>& buffer) {
		Vector3 inc, temp;
		unsigned int bufferSize = buffer.size();

		// Interpolamos las posiciones intermedias del buffer
		for(int i = 0; i < bufferSize - 1; ++i) {
			_buffer.push_back(buffer[i]);

			inc = (buffer[i+1] - buffer[i]) / (float)_ticksPerSample;

			temp = buffer[i] + inc;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
				_buffer.push_back(temp);
				temp += inc;
			}
		}

		_buffer.push_back(buffer[bufferSize - 1]);
	}

	//__________________________________________________________________

	void CPositionInterpolator::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::POSITION_SNAPSHOT: {
				vector<Vector3> buffer = static_pointer_cast<CMessagePositionSnapshot>(message)->getBuffer();

				interpolateSnapshot(buffer);

				break;
			}
		}
	}

	//__________________________________________________________________

	void CPositionInterpolator::onFixedTick(unsigned int msecs) {
		if( !_buffer.empty() ) {
			// Ojo! Asumimos que no existe entidad física. La interpolación de entidades
			// en el cliente no debería tener componente físico. Si lo tiene, tenemos que
			// mover primero al componente físico para que esto funcione.
			_entity->setPosition( _buffer.front() );
			_buffer.pop_front();
		}
		// Estamos perdiendo ticks por algun motivo
		// hay que extrapolar y descartar del buffer
		// que recibamos las que hemos perdido
		else {
			//cout << "Perdiendo snapshots" << endl;
		}
	}

} // namespace Logic


