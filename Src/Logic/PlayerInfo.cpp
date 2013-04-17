//---------------------------------------------------------------------------
// PlayerInfo.cpp
//---------------------------------------------------------------------------

/**
@file PlayerInfo.cpp

Contiene la implementación de la clase PlayerInfo para el proyecto de logica.

@see Logic::CPlayerInfo

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "PlayerInfo.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"

namespace Logic {

	CPlayerInfo::CPlayerInfo(Net::NetID netId) : _netId(netId), 
											     _rank(0), 
												 _entityIdInitialized(false),
												 _isPlaying(false) {
		
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(Net::NetID netId, const std::string& nickname) : _netId(netId), 
																			  _rank(0),
																			  _name(nickname),
																			  _entityIdInitialized(false),
																			  _isPlaying(false) {
		
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(const CPlayerInfo& rhs) : _name(rhs._name), 
													   _mesh(rhs._mesh), 
													   _clan(rhs._clan), 
													   _rank(rhs._rank),
													   _entityId(rhs._entityId),
													   _entityIdInitialized(rhs._entityIdInitialized),
													   _netId(rhs._netId),
													   _isPlaying(rhs._isPlaying) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo& CPlayerInfo::operator=(const CPlayerInfo& rhs) {
		if(this != &rhs) {
			_name = rhs._name;
			_mesh = rhs._mesh;
			_clan = rhs._clan;
			_rank = rhs._rank;
			_entityId = rhs._entityId;
			_entityIdInitialized = rhs._entityIdInitialized;
			_netId = rhs._netId;
			_isPlaying = rhs._isPlaying;
		}

		return *this;
	}

	//______________________________________________________________________________

	std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo) {
	   out << "Nombre = " << playerInfo._name << std::endl;
	   out << "Mesh = " << playerInfo._mesh << std::endl;
	   out << "Clan = " << playerInfo._clan << std::endl;
	   out << "Rank = " << playerInfo._rank << std::endl;
	   out << "EntityID = " << playerInfo._entityId << std::endl;
	   out << "EntityIDInitialized = " << (playerInfo._entityIdInitialized ? "true" : "false") << std::endl;
	   out << "NetID = " << playerInfo._netId << std::endl;
	   out << "spawned = " << (playerInfo._isPlaying ? "true" : "false") << std::endl;

	   return out;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getName() {
		return _name;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getPlayerClass() {
		CEntity* entity = CServer::getSingletonPtr()->getMap()->getEntityByID(_entityId);
		assert(entity && "Error: El gestor no puede devolver la clase del jugador porque no existe en el mapa");
		
		return entity->getType();
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getMesh() {
		return _mesh;
	}

	//______________________________________________________________________________

	std::pair<TEntityID, bool> CPlayerInfo::getEntityId() {
		return std::pair<TEntityID, bool>(_entityId, _entityIdInitialized);
	}

	//______________________________________________________________________________

	Net::NetID CPlayerInfo::getNetId() {
		return _netId;
	}

	//______________________________________________________________________________

	bool CPlayerInfo::isSpawned() {
		return _isPlaying;
	}

	//______________________________________________________________________________
	
	void CPlayerInfo::setName(const std::string& name) {
		this->_name = name;
	}

	//______________________________________________________________________________
		
	void CPlayerInfo::setMesh(const std::string& mesh) {
		this->_mesh = mesh;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setEntityId(TEntityID entityId) {
		this->_entityId = entityId;
		_entityIdInitialized = true;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setNetId(Net::NetID netId) {
		this->_netId = netId;
	}

	//______________________________________________________________________________

	void CPlayerInfo::isSpawned(bool playing) {
		_isPlaying = playing;
	}

};