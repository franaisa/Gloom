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
#include <set>

namespace Net {
	typedef unsigned int NetID;
};

namespace Logic {

	typedef unsigned int TEntityID;

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
		CPlayerInfo(Net::NetID netId);
		CPlayerInfo(std::string name, std::string mesh);
		CPlayerInfo(const CPlayerInfo& rhs);
		~CPlayerInfo() {};

		CPlayerInfo& operator=(const CPlayerInfo& rhs);

		friend std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo);

		std::string getName();
		std::string getMesh();
		TEntityID getEntityId();
		Net::NetID getNetId();
		unsigned int playersLoaded();

		void setName(const std::string& name);
		void setMesh(const std::string& mesh);
		void setEntityId(TEntityID entityId);
		void setNetId(Net::NetID netId);
		
		bool loadPlayer(Net::NetID playerNetId);
		bool unloadPlayer(Net::NetID playerNetId);
		void clearLoadedPlayers();

	private:
		std::string _name;
		std::string _mesh;
		std::string _clan;
		unsigned short int _rank;

		TEntityID _entityId;
		Net::NetID _netId;

		std::set<Net::NetID> _playersLoaded;
		//CPlayerStats _stats; -- en un futuro la clase que contendra las estadisticas del player
	};

};

#endif