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

	CGameNetPlayersManager::CGameNetPlayersManager()
	{
		_instance = this;

	} // CServer

	//______________________________________________________________________________

	CGameNetPlayersManager::~CGameNetPlayersManager()
	{
		_instance = 0;

	} // ~CServer
	
	//______________________________________________________________________________

	bool CGameNetPlayersManager::Init() {
		assert(!_instance && "Segunda inicialización de Logic::CGameNetMsgManager no permitida!");

		new CGameNetPlayersManager();

		return true;

	} // Init

	//______________________________________________________________________________

	void CGameNetPlayersManager::Release()
	{
		assert(_instance && "Logic::CGameNetPlayersManager no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//______________________________________________________________________________

	void CGameNetPlayersManager::activate() 
	{
		// De momento no cargamos nada

	} // activate

	//______________________________________________________________________________

	void CGameNetPlayersManager::deactivate() 
	{	
		// Vaciamos el map de clientes creados

	} // deactivate

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID idPlayer, const std::string& name, const std::string& mesh) {
		return (_connectedPlayers.insert( TConnectedPlayersPair(idPlayer, CPlayerInfo(name, mesh)) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	bool CGameNetPlayersManager::addPlayer(Net::NetID idPlayer) {
		return (_connectedPlayers.insert( TConnectedPlayersPair(idPlayer, CPlayerInfo()) ) ).second;
	} // addPlayer

	//______________________________________________________________________________

	CPlayerInfo CGameNetPlayersManager::getPlayer(Net::NetID idPlayer) {
		TConnectedPlayersTable::const_iterator it = _connectedPlayers.find(idPlayer);
		assert(it == _connectedPlayers.end() && "No se ha encontrado el id de player buscado");

		return it->second;
	} // getPlayer

};