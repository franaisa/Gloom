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

namespace Net {

	CPlayerInfo::CPlayerInfo(std::string name, std::string mesh) : _name(name), _mesh(mesh) {
		// No hay memoria dinamica que inicializar
	}

	//______________________________________________________________________________

	CPlayerInfo::CPlayerInfo(const CPlayerInfo& rhs) : _name(rhs._name), _mesh(rhs._mesh), _clan(rhs._clan), _rank(rhs._rank) {
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

};