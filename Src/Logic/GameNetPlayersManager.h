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

// Predeclaracion del typedef NetID
namespace Net {
	typedef unsigned int NetID;
};

namespace Logic {

	// Predeclaracion del typedef TEntityID
	typedef unsigned int TEntityID;

	/**
	Esta clase es la encargada de controlar toda la información lógica 
	asociada a los jugadores conectados a una partida (nombre, clase, 
	estadisticas etc).

	@ingroup LogicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CGameNetPlayersManager {
	public:


		// =======================================================================
		//                   PREDECLARACION DE CLASES INTERNAS
		// =======================================================================
		

		/** Iterador para obtener o recorrer los jugadores de la partida */
		class iterator;


		// =======================================================================
		//                  METODOS DE INICIALIZACION Y ACTIVACION
		// =======================================================================


		/**
		Devuelve la única instancia de la clase CGameNetPlayersManager.
		
		@return Única instancia de la clase CGameNetPlayersManager.
		*/
		static CGameNetPlayersManager* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera la instancia de CGameNetPlayersManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		//________________________________________________________________________

		/** Función llamada para activar la escucha. */
		void activate();

		//________________________________________________________________________

		/** Función llamada desactivar la escucha. */
		void deactivate();


		// =======================================================================
		//                             MODIFICADORES
		// =======================================================================


		/**
		Inserta un nuevo jugador con la info asociada sin rellenar.
		
		@param idPlayer Identificador de red del player a insertar.
		@return true si no existe otro player con el mismo identificador de red.
		*/
		bool addPlayer(Net::NetID idPlayer);

		//________________________________________________________________________

		/**
		Elimina a un jugador del gestor dado su identificador de red.

		@param idPlayer Identificador de red del player que se desea eliminar.
		@return true si el player estaba registrado.
		*/
		bool removePlayer(Net::NetID idPlayer);

		//________________________________________________________________________

		/**
		Añade un jugador a la lista de jugadores cargados de otro jugador.

		@param idPlayer Identificador de red del player que ha cargado un nuevo jugador.
		@param idPlayerToLoad Identificador de red del jugador que ha cargado el player.
		*/
		void loadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToLoad);

		//________________________________________________________________________

		/**
		Elimina un jugador de la lista de jugadores cargados de un player concreto.

		@param idPlayer Identificador de red del player que debe eliminar a un jugador de su lista.
		@param idPlayerToUnload Identificador de red del player que debe eliminarse de la lista.
		*/
		void unloadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToUnload);


		// =======================================================================
		//                                SETTERS
		// =======================================================================


		/**
		Asigna un nickname a un player dado un identificador de red.

		@param idPlayer Identificador de red del player al que queremos cambiarle el nombre.
		@param nickname Nickname que vamos a asignar al player.
		*/
		void setPlayerNickname(Net::NetID idPlayer, const std::string& nickname);

		//________________________________________________________________________

		/**
		Asigna un mesh a un player dado un identificador de red.

		@param idPlayer Identificador de red del player al que queremos cambiarle el mesh.
		@param mesh Mesh que queremos asignar al player.
		*/
		void setPlayerMesh(Net::NetID idPlayer, const std::string& mesh);

		//________________________________________________________________________

		/**
		Asigna un identificador de entidad a un player dado un identificador de red.

		@param idPlayer Identificador de red del player al que queremos asignar un identificador de entidad.
		@param entityId Identificador de entidad que queremos asignar al player.
		*/
		void setEntityID(Net::NetID idPlayer, TEntityID entityId);


		// =======================================================================
		//                                GETTERS
		// =======================================================================
		

		/**
		Devuelve el numero de jugadores que ha cargado un player dado su identificador
		de red.

		@param idPlayer Identificador de red del player.
		@return Numero de jugadores que el player ha cargado.
		*/
		unsigned int getPlayersLoaded(Net::NetID idPlayer);

		//________________________________________________________________________

		/**
		Devuelve toda la informacion asociada a un jugador dado su identificador de red.

		@param idPlayer Identificador de red del player.
		@return Informacion asociada al player buscado.
		*/
		CPlayerInfo getPlayer(Net::NetID idPlayer);

		//________________________________________________________________________

		/**
		Devuelve el nickname de un player dado su identificador de red.

		@param idPlayer Identificador de red del player.
		@return Nickname del player buscado.
		*/
		std::string getPlayerNickname(Net::NetID idPlayer);

		//________________________________________________________________________

		/**
		Devuelve el numero total de jugadores que hay conectados a la partida actualmente.

		@return Numero total de jugadores conectados.
		*/
		unsigned int getNumberOfPlayersConnected();


		// =======================================================================
		//                                ITERADORES
		// =======================================================================


		/** 
		Devuelve un iterador al primer player de la tabla.
		
		@return Un iterador al primer player de la tabla.
		@deprecated Esta funcion existe por un bug con el iterador que funciona como
		wrapper. Lo ideal es que el cliente no necesite conocer los detalles de 
		implementacion internos.
		*/
		std::map<Net::NetID, CPlayerInfo>::iterator begin2() { return _connectedPlayers.begin(); }

		//________________________________________________________________________
		
		/**
		Devuelve un iterador al final de la tabla.
		
		@return Un iterador al final de la tabla.
		@deprecated Esta funcion existe por un bug con el iterador que funciona como
		wrapper. Lo ideal es que el cliente no necesite conocer los detalles de 
		implementacion internos.
		*/
		std::map<Net::NetID, CPlayerInfo>::iterator end2() { return _connectedPlayers.end(); }

		//________________________________________________________________________

		/**
		Devuelve un iterador al primer player de la tabla. 
		
		@return Un iterador al primer player de la tabla.
		*/
		CGameNetPlayersManager::iterator begin();
		
		//________________________________________________________________________

		/** 
		Devuelve un iterador al final de la tabla.

		@return Un iterador al final de la tabla.
		*/
		CGameNetPlayersManager::iterator end();

	private:


		// =======================================================================
		//                          METODOS PRIVADOS
		// =======================================================================
		

		/** Constructor por defecto. Privado por ser la clase singleton. */
		CGameNetPlayersManager();

		/** Destructor. */
		~CGameNetPlayersManager();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		typedef std::pair<Net::NetID, CPlayerInfo> TConnectedPlayersPair;
		typedef std::map<Net::NetID, CPlayerInfo> TConnectedPlayersTable;

		/** Map que contiene pares (Net::ID, CPlayerInfo). */
		TConnectedPlayersTable _connectedPlayers;

		/** Única instancia de la clase. */
		static CGameNetPlayersManager* _instance;

		
		
	};



	// *****************************************************************************
	// *****************************************************************************
	// *****************************************************************************
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                         CGAMENETPLAYERSMANAGER                        ***
	// ***                               ITERATOR                                ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// ***                                                                       ***
	// *****************************************************************************
	// *****************************************************************************
	// *****************************************************************************


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


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		inline iterator() { }

		//________________________________________________________________________

		/**
		Constructor de conversion.

		@param src Iterador del que se quiere hacer un copia.
		*/
		inline iterator(const TConnectedPlayersTable::iterator& src) : _it(src) { }
		
		//________________________________________________________________________

		/**
		Constructor de copias.

		@param src Iterador del que se quiere hacer un copia.
		*/
		inline iterator(const iterator& src) : _it(src._it) { }
		
		//________________________________________________________________________

		/** Destructor. */
		inline ~iterator() { }


		// =======================================================================
		//                              OPERADORES
		// =======================================================================


		/** 
		Operador de asignacion.

		@param rhs Iterador del que se quiere hacer una copia.
		@return Referencia al nuestro iterador.
		*/
		inline iterator& operator=(const iterator& rhs) {
			if(this != &rhs) {
				_it = rhs._it;
			}

			return *this;
		};

		//________________________________________________________________________

		/** 
		Operador de preincremento.

		@return Referencia a nuestro iterador.
		*/
		inline iterator& operator++() { 
			++_it; 
			return *this;
		}

		//________________________________________________________________________

		/**
		Operador de postdecremento.

		@return Copia temporal de nuestro iterador sin incrementar.
		*/
		inline iterator operator++(int notUsed) { 
			iterator tmp(*this);
			++(*this);
			return tmp; 
		}

		//________________________________________________________________________

		/**
		Operador de igualdad.

		@param rhs Iterador con el que vamos a realizar la comparacion.
		@return true Si ambos iteradores apuntan al mismo sitio.
		*/
		inline bool operator==(const iterator& rhs) const { return _it == rhs._it; }
		
		//________________________________________________________________________

		/**
		Operador de desigualdad.

		@param rhs Iterador con el que vamos a realizar la comparacion.
		@return true Si ambos iteradores apuntan a distintos sitios.
		*/
		inline bool operator!=(const iterator& rhs) const { return _it != rhs._it; }

		//________________________________________________________________________

		/**
		Operador puntero.

		@return Puntero a la informacion asociada al player.
		*/
		inline CPlayerInfo* operator->() const { return &(_it->second); }
		
		//________________________________________________________________________

		/**
		Operador de derreferencia.

		@return Referencia a la informacion asociada al player.
		*/
		inline CPlayerInfo& operator*() const { return *( operator->() ); }


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Iterador de tipo std::map<Net::NetID, CPlayerInfo>. */
		TConnectedPlayersTable::iterator _it;
	};

};

#endif