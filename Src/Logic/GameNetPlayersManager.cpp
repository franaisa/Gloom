//---------------------------------------------------------------------------
// GameNetPlayersManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetPlayersManager.cpp

Contiene la implementación de la clase GameNetPlayersManager para el proyecto de logica.

@see Logic::CGameNetPlayersManager

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "GameNetPlayersManager.h"
#include <cassert>

using namespace std;

namespace Logic {
	
	CGameNetPlayersManager* CGameNetPlayersManager::_instance = NULL;

	//______________________________________________________________________________

	CGameNetPlayersManager::CGameNetPlayersManager() {
		_instance = this;
	} // CServer

	//______________________________________________________________________________

	CGameNetPlayersManager::~CGameNetPlayersManager() {
		// Eliminamos la informacion asociada a los clientes
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.begin();
		for(; it != _netConnectedPlayers.end(); ++it) {
			delete it->second;
		}

		// Vaciamos la tabla de referencias
		_netConnectedPlayers.clear();
		_logicConnectedPlayers.clear();

		_instance = NULL;
	} // ~CServer
	
	//______________________________________________________________________________

	bool CGameNetPlayersManager::Init() {
		assert(!_instance && "Segunda inicialización de Logic::CGameNetMsgManager no permitida!");

		new CGameNetPlayersManager();

		return true;
	} // Init

	//______________________________________________________________________________

	void CGameNetPlayersManager::Release() {
		assert(_instance && "Logic::CGameNetPlayersManager no está inicializado!");

		if(_instance)
			delete _instance;
	} // Release

	//______________________________________________________________________________

	void CGameNetPlayersManager::activate() {
		// De momento no cargamos nada
	} // activate

	//______________________________________________________________________________

	void CGameNetPlayersManager::deactivate() {
		// Eliminamos la informacion asociada a los clientes
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.begin();
		for(; it != _netConnectedPlayers.end(); ++it) {
			delete it->second;
		}

		// Vaciamos la tabla de referencias
		_netConnectedPlayers.clear();
		_logicConnectedPlayers.clear();
	} // deactivate

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID playerNetId) {
		return (_netConnectedPlayers.insert( TLogicConnectedPlayersPair(playerNetId, new CPlayerInfo(playerNetId)) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID playerNetId, const std::string& playerNickname) {
		return (_netConnectedPlayers.insert( TLogicConnectedPlayersPair(playerNetId, new CPlayerInfo(playerNetId, playerNickname)) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	bool CGameNetPlayersManager::removePlayer(Net::NetID playerNetId) {
		// Buscamos el player que queremos borrar. 
		TNetConnectedPlayersTable::iterator itWantedPlayer = _netConnectedPlayers.find(playerNetId);
		if( itWantedPlayer != _netConnectedPlayers.end() ) {
			// Comprobamos si existia una referencia en la tabla de identificadores logicos
			std::pair<Logic::TEntityID, bool> ret = itWantedPlayer->second->getEntityId();
			if(ret.second) {
				// Si existia, borramos su entrada
				_logicConnectedPlayers.erase(ret.first);
			}

			// Borramos el puntero a la informacion asociada al player
			delete itWantedPlayer->second;

			// Borramos la entrada en la tabla de informaciones asociadas a identificadores
			// de red
			_netConnectedPlayers.erase(itWantedPlayer);

			// Exito en el borrado
			return true;
		}

		// El id de red no existia en la tabla, fallo en el borrado
		return false;
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setPlayerNickname(Net::NetID playerNetId, const std::string& nickname) {
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se puede poner nombre al player porque no existe en el Manager");

		it->second->setName(nickname);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setEntityID(Net::NetID playerNetId, Logic::TEntityID entityId) {
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se puede asignar el id de entidad al player porque no existe en el Manager");

		// Seteamos el id de entidad dado al cliente con el netid dado
		it->second->setEntityId(entityId);
		// Añadimos una entrada en la tabla de ids logicos a informacion de cliente
		_logicConnectedPlayers.insert( std::pair<Logic::TEntityID, CPlayerInfo*>(entityId, it->second) );
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setPlayerState(Net::NetID playerNetId, bool isSpawned) {
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se puede asignar un estado al player porque no existe en el Manager");

		it->second->isSpawned(isSpawned);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setPlayerTeam(Net::NetID playerNetId, TeamFaction::Enum team) {
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se puede asignar un estado al player porque no existe en el Manager");

		it->second->setTeam(team);
	}

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::addFragUsingEntityID(Logic::TEntityID entityId) {
		TLogicConnectedPlayersTable::const_iterator it = _logicConnectedPlayers.find(entityId);
		assert(it != _logicConnectedPlayers.end() && "No se ha encontrado el id logico buscado");

		return it->second->addFrag();
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::substractFragUsingEntityID(Logic::TEntityID entityId) {
		TLogicConnectedPlayersTable::const_iterator it = _logicConnectedPlayers.find(entityId);
		assert(it != _logicConnectedPlayers.end() && "No se ha encontrado el id logico buscado");

		it->second->substractFrag();
	}

	//______________________________________________________________________________

	CPlayerInfo CGameNetPlayersManager::getPlayer(Net::NetID playerNetId) {
		TNetConnectedPlayersTable::const_iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se ha encontrado el id de player buscado");

		return *it->second;
	}

	//______________________________________________________________________________

	std::pair<TEntityID, bool> CGameNetPlayersManager::getPlayerId(Net::NetID playerNetId) {
		TNetConnectedPlayersTable::const_iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se ha encontrado el id de player buscado");

		return it->second->getEntityId();
	}

	//______________________________________________________________________________

	CPlayerInfo CGameNetPlayersManager::getPlayerByEntityId(Logic::TEntityID entityId) {
		TLogicConnectedPlayersTable::const_iterator it = _logicConnectedPlayers.find(entityId);
		assert(it != _logicConnectedPlayers.end() && "No se ha encontrado el id logico buscado");

		return *it->second;
	}

	//______________________________________________________________________________

	TeamFaction::Enum CGameNetPlayersManager::getTeamUsingEntityId(Logic::TEntityID entityId) {
		TLogicConnectedPlayersTable::const_iterator it = _logicConnectedPlayers.find(entityId);
		assert(it != _logicConnectedPlayers.end() && "No se ha encontrado el id logico buscado");

		return it->second->getTeam();
	}

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::getNumberOfPlayersConnected() {
		return _netConnectedPlayers.size();
	}

	//______________________________________________________________________________

	// Esta hecho de forma un poco bestia, pero como son solo 8 jugadores y solo se
	// hace en las conexiones tampoco pasa nada (y me facilita mucho la vida hacerlo
	// asi).
	unsigned int CGameNetPlayersManager::getNumberOfPlayersSpawned() {
		TNetConnectedPlayersTable::const_iterator it = _netConnectedPlayers.begin();

		int playersSpawned = 0;
		for(; it != _netConnectedPlayers.end(); ++it) {
			if( it->second->isSpawned() ) {
				++playersSpawned;
			}
		}

		return playersSpawned;
	}

	//______________________________________________________________________________

	bool CGameNetPlayersManager::existsByNetId(Net::NetID playerNetId) {
		return _netConnectedPlayers.count(playerNetId) > 0;
	}

	//______________________________________________________________________________

	bool CGameNetPlayersManager::existsByLogicId(Logic::TEntityID playerId) {
		return _logicConnectedPlayers.count(playerId) > 0;
	}

	//______________________________________________________________________________

	int CGameNetPlayersManager::getFragsUsingEntityID(Logic::TEntityID playerId) {
		TLogicConnectedPlayersTable::const_iterator it = _logicConnectedPlayers.find(playerId);
		assert(it != _logicConnectedPlayers.end() && "No se ha encontrado el id logico buscado");

		return it->second->getFrags();
	}

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::blueTeamPlayers() {
		auto it = _netConnectedPlayers.begin();

		unsigned int nbPlayers = 0;
		for(; it != _netConnectedPlayers.end(); ++it) {
			if(it->second->getTeam() == TeamFaction::eBLUE_TEAM) {
				++nbPlayers;
			}
		}

		return nbPlayers;
	}

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::redTeamPlayers() {
		auto it = _netConnectedPlayers.begin();

		unsigned int nbPlayers = 0;
		for(; it != _netConnectedPlayers.end(); ++it) {
			if(it->second->getTeam() == TeamFaction::eRED_TEAM) {
				++nbPlayers;
			}
		}

		return nbPlayers;
	}

	//______________________________________________________________________________

	string CGameNetPlayersManager::getPlayerNickname(Net::NetID playerNetId) {
		TNetConnectedPlayersTable::iterator it = _netConnectedPlayers.find(playerNetId);
		assert(it != _netConnectedPlayers.end() && "No se puede poner obtener el nombre del player porque no existe en el Manager");

		return it->second->getName();
	}

	//______________________________________________________________________________

	CGameNetPlayersManager::iterator CGameNetPlayersManager::begin() {
		return CGameNetPlayersManager::iterator( _netConnectedPlayers.begin() );
	}

	//______________________________________________________________________________

	CGameNetPlayersManager::iterator CGameNetPlayersManager::end() {
		return CGameNetPlayersManager::iterator( _netConnectedPlayers.end() );
	}

};