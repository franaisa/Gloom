//---------------------------------------------------------------------------
// PlayerInfo.h
//---------------------------------------------------------------------------

/**
@file PlayerInfo.h

Contiene la declaracion de la clase PlayerInfo para el proyecto de red.

@see Net::CPlayerInfo

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Net_PlayerInfo_H
#define __Net_PlayerInfo_H

#include <string>

// Predeclaracion de clases
namespace Net {
	class CConexion;
};

namespace Net {

	/**
	Información sobre los clientes. El manager de red debe mantener una
	lista de clientes conectados con información asociada a ellos. Esta
	clase sirve de abstracción para introducir todos aquellos detalles
	de los clientes que consideremos relevantes.

	@ingroup NetGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	class CPlayerInfo {
	public:
		CPlayerInfo(std::string name, std::string mesh, CConexion* connection);
		CPlayerInfo(CConexion* connection);
		CPlayerInfo(const CPlayerInfo& rhs);
		~CPlayerInfo();

		CPlayerInfo& operator=(const CPlayerInfo& rhs);

		friend std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo);

		std::string getName();
		std::string getMesh();
		CConexion* getConnection();

	private:
		std::string _name;
		std::string _mesh;
		std::string _clan;
		unsigned short int _rank;

		// Esta clase no es responsable de liberar los recursos alocatados por _connection
		// tan solo se encarga de almacenar el puntero dado
		CConexion* _connection;
		//CPlayerStats _stats;
	};

};

#endif