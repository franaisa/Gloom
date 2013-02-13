//---------------------------------------------------------------------------
// PlayerInfo.h
//---------------------------------------------------------------------------

/**
@file PlayerInfo.h

Contiene la declaracion de la clase PlayerInfo para el proyecto de logica.

@see Logic::CPlayerInfo

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_PlayerInfo_H
#define __Logic_PlayerInfo_H

#include <string>

namespace Logic {

	/**
	Información sobre los clientes. El manager de red debe mantener una
	lista de clientes conectados con información asociada a ellos. Esta
	clase sirve de abstracción para introducir todos aquellos detalles
	de los clientes que consideremos relevantes.

	@ingroup LogicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	class CPlayerInfo {
	public:
		CPlayerInfo() : _rank(0) {}
		CPlayerInfo(std::string name, std::string mesh);
		CPlayerInfo(const CPlayerInfo& rhs);
		~CPlayerInfo();

		CPlayerInfo& operator=(const CPlayerInfo& rhs);

		friend std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo);

		std::string getName();
		std::string getMesh();

		void setName(const std::string& name);
		void setMesh(const std::string& mesh);

	private:
		std::string _name;
		std::string _mesh;
		std::string _clan;
		unsigned short int _rank;
		//CPlayerStats _stats; -- en un futuro la clase que contendra las estadisticas del player
	};

};

#endif