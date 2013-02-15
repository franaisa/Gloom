//---------------------------------------------------------------------------
// GameNetPlayersManager.h
//---------------------------------------------------------------------------

/**
@file GameNetPlayersManager.h

Contiene la declaracion de la clase GameNetPlayersManager para el proyecto de logica.

@see Logic::CGameNetPlayersManager

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_GameNetPlayersManager_H
#define __Logic_GameNetPlayersManager_H

#include <map>
#include <string>
#include "PlayerInfo.h"

namespace Net {
	/** ID de identificación en la red. */
	typedef unsigned int NetID;
};

namespace Logic {

	typedef unsigned int TEntityID;

	/**
	Esta clase es la encargada de controlar toda la información lógica 
	asociada a los jugadores conectados a una partida (nombre, mesh, 
	estadisticas etc).

	@ingroup LogicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	class CGameNetPlayersManager {
	public:
		
		/** Iterador para obtener o recorrer los jugadores de la partida */
		class iterator;
		/** Iterador constante para obtener o recorrer los jugadores de la partida */
		class const_iterator;

		/**
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CGameNetPlayersManager* getSingletonPtr() { return _instance; }

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CGameNetPlayersManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada para activar la escucha.
		*/
		void activate();

		/**
		Función llamada desactivar la escucha.
		*/
		void deactivate();

		/** Inserta un nuevo jugador */
		bool addPlayer(Net::NetID idPlayer, const std::string& name, const std::string& mesh);

		/** Inserta un nuevo jugador con la info asociada sin rellenar */
		bool addPlayer(Net::NetID idPlayer);

		bool removePlayer(Net::NetID idPlayer);



		void setPlayerNickname(Net::NetID idPlayer, const std::string& nickname);

		void setPlayerMesh(Net::NetID idPlayer, const std::string& mesh);

		void setEntityID(Net::NetID idPlayer, TEntityID entityId);

		//void incrementPlayersLoaded(Net::NetID idPlayer);

		//void decrementPlayersLoaded(Net::NetID idPlayer);

		//void resetPlayersLoaded(Net::NetID idPlayer);


		void loadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToLoad);
		void unloadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToUnload);
		unsigned int getPlayersLoaded(Net::NetID idPlayer);


		/** Devuelve un jugador si es que ha sido registrado */
		CPlayerInfo getPlayer(Net::NetID idPlayer);

		std::string getPlayerNickname(Net::NetID idPlayer);

		unsigned int getNumberOfPlayersConnected();



		/** Devuelve el primer player de la tabla */
		CGameNetPlayersManager::iterator begin();
		
		/** Devuelve el último player de la tabla */
		CGameNetPlayersManager::iterator end();

	protected:
		/** Constructor de la clase */
		CGameNetPlayersManager();

		/** Destructor */
		~CGameNetPlayersManager();

	private:
		typedef std::pair<Net::NetID, CPlayerInfo> TConnectedPlayersPair;
		typedef std::map<Net::NetID, CPlayerInfo> TConnectedPlayersTable;

		/** Map que contiene pares Net::ID e info de los players */
		TConnectedPlayersTable _connectedPlayers;

		/** Única instancia de la clase. */
		static CGameNetPlayersManager* _instance;
	};

	/**
	Esta clase es un wrapper para los iteradores que internamente manejan a la
	clase CGameNetPlayersManager, de esta manera podemos recorrer sin ningún 
	tipo de compromiso a los players (por si en un futuro se cambia el entramado).

	@ingroup LogicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	class CGameNetPlayersManager::iterator {
	public:
		inline iterator() { }
		inline iterator(const TConnectedPlayersTable::iterator& src) : _it(src) { }
		inline iterator(const iterator& src) : _it(src._it) { }
		inline ~iterator() { }

		inline iterator& operator=(const iterator& rhs) {
			if(this != &rhs) {
				_it = rhs._it;
			}

			return *this;
		};

		inline iterator& operator++() { 
			++_it; 
			return *this;
		}

		inline iterator operator++(int notUsed) { 
			iterator tmp(*this);
			++(*this);
			return tmp; 
		}

		inline bool operator==(const iterator& rhs) const { return _it == rhs._it; }
		inline bool operator!=(const iterator& rhs) const { return _it != rhs._it; }

		inline CPlayerInfo* operator->() const { return &(_it->second); }
		inline CPlayerInfo& operator*() const { return *( operator->() ); }

	private:
		TConnectedPlayersTable::iterator _it;
	};

};

#endif