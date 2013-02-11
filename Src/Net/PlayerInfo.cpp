//---------------------------------------------------------------------------
// PlayerInfo.cpp
//---------------------------------------------------------------------------

/**
@file PlayerInfo.cpp

Contiene la implementación de la clase PlayerInfo para el proyecto de red.

@see Net::CPlayerInfo

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "PlayerInfo.h"
#include "conexion.h"

namespace Net {

	CPlayerInfo::CPlayerInfo(std::string name, std::string mesh, CConexion* connection) : 
																   _name(name), 
																   _mesh(mesh),
																   _connection(connection) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(CConexion* connection) : _connection(connection) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(const CPlayerInfo& rhs) : _name(rhs._name), 
													   _mesh(rhs._mesh), 
													   _clan(rhs._clan), 
													   _rank(rhs._rank),
													   _connection(rhs._connection) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::~CPlayerInfo() {
		// No hay memoria dinamica que borrar
	}

	//______________________________________________________________________________

	CPlayerInfo& CPlayerInfo::operator=(const CPlayerInfo& rhs) {
		if(this != &rhs) {
			_name = rhs._name;
			_mesh = rhs._mesh;
			_clan = rhs._clan;
			_rank = rhs._rank;
			_connection = rhs._connection;
		}

		return *this;
	}

	//______________________________________________________________________________

	std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo) {
	   out << "Nombre = " << playerInfo._name << std::endl;
	   out << "Mesh = " << playerInfo._mesh << std::endl;
	   out << "Clan = " << playerInfo._clan << std::endl;
	   out << "Rank = " << playerInfo._rank;

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

	CConexion* CPlayerInfo::getConnection() {
		return _connection;
	}

	//______________________________________________________________________________

	void CPlayerInfo::setName(const std::string& name) {
		this->_name = name;
	}

	//______________________________________________________________________________
		
	void CPlayerInfo::setMesh(const std::string& mesh) {
		this->_mesh = mesh;
	}

};