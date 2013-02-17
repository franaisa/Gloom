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
	
	CGameNetPlayersManager* CGameNetPlayersManager::_instance = 0;

	//______________________________________________________________________________

	CGameNetPlayersManager::CGameNetPlayersManager() {
		_instance = this;
	} // CServer

	//______________________________________________________________________________

	CGameNetPlayersManager::~CGameNetPlayersManager() {
		_instance = 0;
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
		// Vaciamos el map de clientes creados
		_connectedPlayers.clear();
	} // deactivate

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID idPlayer, const std::string& name, const std::string& mesh) {
		return (_connectedPlayers.insert( TConnectedPlayersPair(idPlayer, CPlayerInfo(name, mesh)) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID idPlayer) {
		return (_connectedPlayers.insert( TConnectedPlayersPair(idPlayer, CPlayerInfo(idPlayer)) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	bool CGameNetPlayersManager::removePlayer(Net::NetID idPlayer) {
		// Si el player que desea desconectarse ha sido cargado por algun otro cliente
		// lo eliminamos de su lista de jugadores cargados
		TConnectedPlayersTable::iterator it = _connectedPlayers.begin();
		for(; it != _connectedPlayers.end(); ++it) {
			it->second.unloadPlayer(idPlayer);
		}

		return _connectedPlayers.erase(idPlayer) > 0;
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setPlayerNickname(Net::NetID idPlayer, const std::string& nickname) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede poner nombre al player porque no existe en el Manager");

		it->second.setName(nickname);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setPlayerMesh(Net::NetID idPlayer, const std::string& mesh) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede poner el nombre de la maya al player porque no existe en el Manager");

		it->second.setMesh(mesh);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::setEntityID(Net::NetID idPlayer, Logic::TEntityID entityId) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede asignar el id de entidad al player porque no existe en el Manager");

		it->second.setEntityId(entityId);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::loadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToLoad) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede aumentar el contador de players porque no existe el player en el Manager");

		it->second.loadPlayer(idPlayerToLoad);
	}

	//______________________________________________________________________________

	void CGameNetPlayersManager::unloadPlayer(Net::NetID idPlayer, Net::NetID idPlayerToUnload) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede decrementar el contador de players porque no existe el player en el Manager");

		it->second.unloadPlayer(idPlayerToUnload);
	}

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::getPlayersLoaded(Net::NetID idPlayer) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede devolver cuantos players ha cargado el jugador porque no existe en el Manager");

		return it->second.playersLoaded();
	}

	//______________________________________________________________________________

	CPlayerInfo CGameNetPlayersManager::getPlayer(Net::NetID idPlayer) {
		TConnectedPlayersTable::const_iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se ha encontrado el id de player buscado");

		return it->second;
	} // getPlayer

	//______________________________________________________________________________

	unsigned int CGameNetPlayersManager::getNumberOfPlayersConnected() {
		return _connectedPlayers.size();
	}

	//______________________________________________________________________________

	string CGameNetPlayersManager::getPlayerNickname(Net::NetID idPlayer) {
		TConnectedPlayersTable::iterator it = _connectedPlayers.find(idPlayer);
		assert(it != _connectedPlayers.end() && "No se puede poner obtener el nombre del player porque no existe en el Manager");

		return it->second.getName();
	}

	//______________________________________________________________________________

	CGameNetPlayersManager::iterator CGameNetPlayersManager::begin() {
		return CGameNetPlayersManager::iterator( _connectedPlayers.begin() );
	}

	//______________________________________________________________________________

	CGameNetPlayersManager::iterator CGameNetPlayersManager::end() {
		return CGameNetPlayersManager::iterator( _connectedPlayers.end() );
	}

};