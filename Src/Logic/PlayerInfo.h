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

// Predeclaracion del typedef NetID
namespace Net {
	typedef unsigned int NetID;
};

namespace Logic {

	// Predeclaracion del typedef TEntityID
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


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CPlayerInfo() : _rank(0), _entityIdInitialized(false) {}
		
		//________________________________________________________________________

		/**
		Constructor por parametro
		
		@param netId Identificador de red del player que se va a crear.
		*/
		CPlayerInfo(Net::NetID netId);

		//________________________________________________________________________

		/**
		Constructor por parametro
		
		@param netId Identificador de red del player que se va a crear.
		@param nickname Nickname del player que se va a crear.
		*/
		CPlayerInfo(Net::NetID netId, const std::string& nickname);
		
		//________________________________________________________________________

		/** 
		Constructor de copias.

		@param rhs objeto que se a partir del cual vamos a inicializar el nuestro.
		*/
		CPlayerInfo(const CPlayerInfo& rhs);
		
		//________________________________________________________________________

		/** Destructor */
		~CPlayerInfo() { /* No hay memoria dinamica que liberar */ };


		// =======================================================================
		//                              OPERADORES
		// =======================================================================


		/**
		Operador de asignacion.

		@param Objeto que vamos a asignar a nuestra clase.
		@return Referencia a nuestro objeto.
		*/
		CPlayerInfo& operator=(const CPlayerInfo& rhs);

		//________________________________________________________________________
		
		/**
		Operador de flujo de salida.

		@param out Flujo de salida.
		@param playerInfo Informacion del player.
		@return Referencia al flujo de salida modificado.
		*/
		friend std::ostream& operator<<(std::ostream& out, const CPlayerInfo& playerInfo);


		// =======================================================================
		//                               GETTERS
		// =======================================================================


		/**
		Devuelve el nickname del player.

		@return El nickname del player.
		*/
		std::string getName();

		//________________________________________________________________________

		/**
		Devuelve la clase que esta usando el player.

		@return La clase del player.
		*/
		std::string getPlayerClass();

		//________________________________________________________________________

		/**
		Devuelve el mesh del player.

		@return El mesh del player.
		*/
		std::string getMesh();

		//________________________________________________________________________

		/**
		Devuelve el identificador de la entidad logica que corresponde al player que
		describe este CPlayerInfo.

		@return El identificador de la entidad y un booleano que indica si el id de
		entidad ha sido inicializado.
		*/
		std::pair<TEntityID, bool> getEntityId();

		//________________________________________________________________________

		/**
		Devuelve el identificador de red de la entidad que corresponde al player que
		describe este CPlayerInfo.

		@return El identificador de red de la entidad.
		*/
		Net::NetID getNetId();

		//________________________________________________________________________

		/**
		Devuelve la cantidad de clientes que este player ha cargado.

		@return Numero de clientes que este player ha cargado.
		*/
		unsigned int playersLoaded();

		//________________________________________________________________________

		/**
		Indica si el player ha sido spawneado en la partida online o no.

		@return True si la entidad ha sido spawneada.
		*/
		bool isSpawned();


		// =======================================================================
		//                               SETTERS
		// =======================================================================


		/**
		Asigna un nickname al player.

		@param name Nombre que se le desea asignar al player.
		*/
		void setName(const std::string& name);

		//________________________________________________________________________

		/**
		Asigna un mesh al player.

		@param mesh Nombre del mesh que se le va a asignar al player.
		*/
		void setMesh(const std::string& mesh);

		//________________________________________________________________________

		/**
		Asigna un identificador de entidad al player.

		@param entityId Identificador de entidad que se le va a asignar al player.
		*/
		void setEntityId(TEntityID entityId);

		//________________________________________________________________________

		/**
		Asigna un identificador de red al player.

		@param Identificador de red que se le va a asignar al player.
		*/
		void setNetId(Net::NetID netId);

		//________________________________________________________________________
		
		/**
		Setea el estado del jugador, si jugando o espectando/conectandose.

		@param playing True si el jugador esta spawneado.
		*/
		void isSpawned(bool playing);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Nickname del player. */
		std::string _name;

		/** Nombre del mesh que esta usando el player. */
		std::string _mesh;

		/** Clan al que pertenece este player. */
		std::string _clan;

		/** Nivel de este player. */
		unsigned short int _rank;

		/** Identificador de la entidad logica */
		TEntityID _entityId;

		/** True si el identificador de entidad ha sido inicializado */
		bool _entityIdInitialized;

		/** Identificador de la entidad de red */
		Net::NetID _netId;

		/** 
		True si el jugador esta dentro de la partida, false si se esta conectando
		o está en modo espectador. */
		bool _isPlaying;
	};

};

#endif