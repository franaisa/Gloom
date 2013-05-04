//---------------------------------------------------------------------------
// EntityIdDispatcher.h
//---------------------------------------------------------------------------

/**
@file EntityIdDispatcher.h

Contiene la declaración de la clase EntityIdDispatcher que
se encarga de asignar id's de red.

@see Logic::CEntityIdDispatcher

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __EntityIdDispatcher_H
#define __EntityIdDispatcher_H

#include <iostream>
#include <math.h>

namespace Logic {

	template <typename T>
	class CEntityIdDispatcher {
	public:

		CEntityIdDispatcher(unsigned int playerId = 0, unsigned int nbPlayers = 0) {
			// Estimamos el número de bits del tipo T
			_nbBits = countBits();
			// Estimamos cuantos bits necesitamos para indexar a los players
			_bitsForPlayers = ceil( log((float)nbPlayers) / log(2.0f) );

			assert(_bitsForPlayers < _nbBits && "Error: No quedan bits para asignar ids de entidad");

			// Ponemos la máscara todo a unos
			setAllBitsToOne<T>(_entityMask);
			// Reservamos los último bits para el id del player
			_entityMask = _entityMask >> _bitsForPlayers;
			// Generamos la máscara de players (que es justo la contraria a la de entidades)
			_playerMask = ~_entityMask;
			// Creamos la marca de agua para este player, es todo 0 si estamos en SP
			_waterMark = (nbPlayers > 0) ? playerId << (_nbBits - _bitsForPlayers) : 0;

			_nextId = 0 | _waterMark;
		}

		~CEntityIdDispatcher() {
			// Nada que destruir
		}

		template <typename T>
		friend std::ostream& operator<<(std::ostream& out, const CEntityIdDispatcher<T>& dispatcher) {
			out << "EntityMask = " << dispatcher._entityMask << endl;
			out << "PlayerMask = " << dispatcher._playerMask << endl;
			out << "WaterMark = " << dispatcher._waterMark << endl;
			out << "nextId = " << dispatcher._nextId << endl;

			return out;
		}

		// Devuelve el siguiente id de entidad teniendo en cuenta
		// el overflow
		inline T getNextId() {
			// Ret es el valor de retorno del siguiente id
			T ret = _nextId;
			// Preparamos el siguiente id, teniendo en cuenta
			// el desbordamiento
			_nextId = (_nextId + 1) & _entityMask;
			_nextId = _nextId | _waterMark;
			
			return ret;
		}

		// Devuelve el id del player que creo el id dado
		// En SP siempre devuelve 0
		// Debe existir una conversion a unsigned int
		inline unsigned int getPlayerId(T id) {
			// Obtenemos la id del player desplazando sus bits
			// de identificacion al comienzo de una palabra vacia
			T playerId = id;
			playerId = playerId & _playerMask;
			playerId = playerId >> (_nbBits - _bitsForPlayers);
			
			return playerId;
		}

	private:

		// Obtiene el numero de bits del tipo T
		inline unsigned int countBits() {
			return sizeof(T) * 8;
		}

		// Pone todos los bits de una palabra a uno.
		// Es lento, solo deberia usarse en el constructor
		template<typename T>
		inline void setAllBitsToOne(T& word) {
			T temp;
			for(int bit = 0; bit < _nbBits; ++bit) {
				temp = 1 << bit;
				word = word | temp;
			}
		}

		// Especializacion para los unsigned ints
		template<>
		inline void setAllBitsToOne(unsigned int& word) {
			word = -1;
		}

		// Numero de bits del tipo T
		unsigned int _nbBits;
		// Numero de bits reservados para identificar players
		unsigned int _bitsForPlayers;

		// Mascara para ids de entidad creadas
		T _entityMask;
		// Mascara para ids de jugador
		T _playerMask;
		// Marca de agua con el id del jugador
		T _waterMark;
		// Siguiente Id a devolver
		T _nextId; 
	};

} // namespace Logic

#endif // __EntityIdDispatcher_H
