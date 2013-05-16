
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

				// Si hemos perdido ticks tenemos que descartar posiciones del buffer
				/*if(_lostTicks > 0) {
					// Si los ticks que hemos perdido son más de los que tenemos al interpolar
					// el buffer, descartamos el buffer entero y extrapolamos o nos quedamos
					// quietos si ha pasado mucho tiempo
					unsigned int interpolatedBufferSize = _samplesPerSnapshot + (_samplesPerSnapshot - 1) * (_ticksPerSample - 1);
					if(_lostTicks >= interpolatedBufferSize) {
						// Los ticks que hemos recuperado equivalen al tamaño del buffer
						_lostTicks -= interpolatedBufferSize;
					}
					else {
						// Calculamos lo que queda del buffer
						interpolateSnapshot(buffer);

						// @deprecated, lo suyo es interpolar a partir
						// del tick que sabes que te corresponde y no esta
						// chapucilla
						for(int i = 0; i < _lostTicks; ++i) {
							_buffer.pop_front();
						}

						_lostTicks = 0;
						_extrapolatedTicks = 0;
					}
				}
				else {
					interpolateSnapshot(buffer);
				}*/

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
			//_extrapolatedMovement = _buffer.front().getTrans() - _entity->getPosition();

			_controller->setPhysicPosition( _buffer.front().getTrans() );
			_entity->setTransform( _buffer.front() );
			_buffer.pop_front();
		}
		// Estamos perdiendo ticks por algun motivo
		// hay que extrapolar y descartar del buffer
		// que recibamos las que hemos perdido
		else {
			// Si hemos extrapolado durante mas del tiempo permitido nos quedamos quietos
			/*if(_extrapolatedTicks <= 5) {
				//_controller->setPhysicPosition(_entity->getPosition() + _extrapolatedMovement);
				++ _extrapolatedTicks;
			}

			++_lostTicks;*/
		}
	}

} // namespace Logic


