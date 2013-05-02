//---------------------------------------------------------------------------
// NetIdDispatcher.h
//---------------------------------------------------------------------------

/**
@file NetIdDispatcher.h

Contiene la declaración de la clase NetIdDispatcher que
se encarga de asignar id's de red.

@see Net::CNetIdDispatcher

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __NetIdDispatcher_H
#define __NetIdDispatcher_H

#include <stack>

namespace Net {

	class CNetIdDispatcher {
	public:
		CNetIdDispatcher(unsigned int nbClients) {
			for(int clientId = nbClients; clientId > 0; --clientId) {
				_availableClientIds.push(clientId);
			}
		}

		~CNetIdDispatcher() { }

		// Devuelve el id del server, por defecto 0
		inline unsigned int getServerId() { return 0; }
		
		// Devuelve el siguiente id disponible
		// Cuidado con intentar sacar mas id's de la cuenta
		inline unsigned int getNextClientId() { 
			unsigned int nextId = _availableClientIds.top();
			_availableClientIds.pop();
			return nextId;
		}

		// Cuidado con meter dos veces el mismo id
		inline void returnClientId(unsigned int clientId) {
			_availableClientIds.push(clientId);
		}

	private:

		std::stack<unsigned int> _availableClientIds;
	};

} // namespace Net

#endif // __NetIdDispatcher_H
