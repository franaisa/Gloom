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

namespace Logic {

	CPlayerInfo::CPlayerInfo(Net::NetID netId) : _netId(netId), _rank(0) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(std::string name, std::string mesh) : _name(name), 
																   _mesh(mesh) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(const CPlayerInfo& rhs) : _name(rhs._name), 
													   _mesh(rhs._mesh), 
													   _clan(rhs._clan), 
													   _rank(rhs._rank),
													   _entityId(rhs._entityId),
													   _netId(rhs._netId) {
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
			_netId = rhs._netId;
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
	   out << "NetID = " << playerInfo._netId;

	   return out;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getName() {
		return _name;
	}

	//______________________________________________________________________________

	std::string CPlayerInfo::getMesh() {
		return _mesh;
	}

	//______________________________________________________________________________

	TEntityID CPlayerInfo::getEntityId() {
		return _entityId;
	}

	//______________________________________________________________________________

	Net::NetID CPlayerInfo::getNetId() {
		return _netId;
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
	}

	//______________________________________________________________________________

	void CPlayerInfo::setNetId(Net::NetID netId) {
		this->_netId = netId;
	}

};