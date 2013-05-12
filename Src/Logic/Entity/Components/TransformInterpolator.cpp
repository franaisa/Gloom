
/**
@file TransformInterpolator.cpp
 
@see Logic::CTransformInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "TransformInterpolator.h"
#include "PhysicController.h"

// Map
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageTransformSnapshot.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CTransformInterpolator);

	//__________________________________________________________________

	CTransformInterpolator::CTransformInterpolator() : _lostTicks(0) {
		// Nada que hacer
	}

	//__________________________________________________________________

	CTransformInterpolator::~CTransformInterpolator() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CTransformInterpolator::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		assert( entityInfo->hasAttribute("ticksPerSample") );

		// Cada cuantos fixed ticks tomamos una muestra
		_ticksPerSample = entityInfo->getIntAttribute("ticksPerSample");

		return true;
	}

	//__________________________________________________________________

	bool CTransformInterpolator::accept(const shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::TRANSFORM_SNAPSHOT;
	}

	//__________________________________________________________________

	void CTransformInterpolator::interpolateSnapshot(const vector<Matrix4>& buffer) {
		Matrix4 incrementMatrix, temp;
		unsigned int bufferSize = buffer.size();
		
		// Si el buffer no estaba vacio calculamos los puntos intermedios
		// con la nueva snapshot para que la transicion siga siendo suave
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
			
			// Calculamos el incremento para las posiciones interpoladas
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
	}

	//__________________________________________________________________

	void CTransformInterpolator::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::TRANSFORM_SNAPSHOT: {
				vector<Matrix4> buffer = static_pointer_cast<CMessageTransformSnapshot>(message)->getBuffer();

				interpolateSnapshot(buffer);

				break;
			}
		}
	}

	//__________________________________________________________________

	void CTransformInterpolator::onStart() {
		_controller = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(_controller != NULL && "Error: Como crees que voy a interpolar si no tengo physicController? meh!");
	}

	//__________________________________________________________________

	void CTransformInterpolator::onFixedTick(unsigned int msecs) {
		if( !_buffer.empty() ) {
			_controller->setPhysicPosition(_buffer.front().getTrans());
			_entity->setTransform(_buffer.front());
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


