
/**
@file TransformInterpolator.cpp
 
@see Logic::CTransformInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "TransformInterpolator.h"

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

	CTransformInterpolator::CTransformInterpolator() {
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
		assert( entityInfo->hasAttribute("samplesPerSnapshot") );

		// Cada cuantos fixed ticks tomamos una muestra
		_ticksPerSample = entityInfo->getIntAttribute("ticksPerSample");
		// Cada cuantas muestras tomamos una snapshot
		_samplesPerSnapshot = entityInfo->getIntAttribute("samplesPerSnapshot");

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
		
		// Interpolamos las posiciones intermedias del buffer
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

	void CTransformInterpolator::onFixedTick(unsigned int msecs) {
		if( !_buffer.empty() ) {
			// Ojo! Asumimos que no existe entidad física. La interpolación de entidades
			// en el cliente no debería tener componente físico. Si lo tiene, tenemos que
			// mover primero al componente físico para que esto funcione.
			_entity->setTransform( _buffer.front() );
			_buffer.pop_front();
		}
	}

} // namespace Logic
