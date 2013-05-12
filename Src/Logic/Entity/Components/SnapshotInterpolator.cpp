
/**
@file SnapshotInterpolator.cpp
 
@see Logic::CSnapshotInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "SnapshotInterpolator.h"
#include "PhysicController.h"

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

	CSnapshotInterpolator::CSnapshotInterpolator() : _lostTicks(0) {
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
		_ticksPerInterpolation = 6;
		_ticksPerSample = 2;
		_samplesPerSnapshot = 3;

		return true;
	}

	//__________________________________________________________________

	bool CSnapshotInterpolator::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::TRANSFORM_SNAPSHOT;
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::interpolateSnapshot(const vector<Matrix4>& buffer) {
		Matrix4 incrementMatrix, temp;
		unsigned int bufferSize = buffer.size();
		
		if(!_buffer.empty()) {
			unsigned int lastIndex = _buffer.size() - 1;
			Matrix4 inc = (buffer[0] - _buffer[lastIndex]) * (1.0f / (float)_ticksPerSample);

			Matrix4 aux = _buffer[lastIndex] + inc;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
				_buffer.push_back(aux);
				aux = aux + inc;
			}
		}
		
		for(int i = 0; i < bufferSize - 1; ++i) {
			// Insertamos la posicion inicial
			_buffer.push_back(buffer[i]);
			
			// Calculamos el incremento para las posiciones
			// interpoladas
			incrementMatrix = (buffer[i+1] - buffer[i]) * (1.0f / (float)_ticksPerSample);
			// Generamos las posiciones intermedias
			temp = buffer[i] + incrementMatrix;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
                _buffer.push_back(temp);
                temp = temp + incrementMatrix;
            }
		}

		// Insertamos la ultima posicion
		_buffer.push_back( buffer[bufferSize - 1] );

		// Interpolar la primera posicion
		/*if(!_buffer.empty()) {
			unsigned int lastIndex = _buffer.size() - 1;
			Vector3 inc = (buffer[0].getTrans() - _buffer[lastIndex]) / (float)_ticksPerSample;

			Vector3 vec = _buffer[lastIndex] + inc;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
				_buffer.push_back(vec);
				vec += inc;
			}
		}

		for(int i = 0; i < bufferSize - 1; ++i) {
			_buffer.push_back(buffer[i].getTrans());

			Vector3 inc = (buffer[i+1].getTrans() - buffer[i].getTrans()) / (float)_ticksPerSample;

			Vector3 vec = buffer[i].getTrans() + inc;
			for(int k = 0; k < _ticksPerSample - 1; ++k) {
				_buffer.push_back(vec);
				vec += inc;
			}
		}

		_buffer.push_back( buffer[bufferSize - 1].getTrans() );*/
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TRANSFORM_SNAPSHOT: {
				vector<Matrix4> buffer = static_pointer_cast<CMessageTransformSnapshot>(message)->getBuffer();

				interpolateSnapshot(buffer);

				break;
			}
		}
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::onStart() {
		_controller = _entity->getComponent<CPhysicController>("CPhysicController");
		//_buffer.push_back( _entity->getPosition() );
	}

	//__________________________________________________________________

	void CSnapshotInterpolator::onFixedTick(unsigned int msecs) {
		if( !_buffer.empty() ) {
			_controller->setPhysicPosition(_buffer.front().getTrans());
			_entity->setTransform(_buffer.front());
			_buffer.pop_front();
		}
		/*if( !_buffer.empty() ) {
			_controller->setPhysicPosition(_buffer.front());
			_buffer.pop_front();
		}*/
		// Estamos perdiendo ticks por algun motivo
		// hay que extrapolar y descartar del buffer
		// que recibamos las que hemos perdido
		else {
			//cout << "Perdiendo snapshots" << endl;
		}
	}

} // namespace Logic


