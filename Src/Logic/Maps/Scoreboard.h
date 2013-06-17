#ifndef __Logic_Scoreboard_H
#define __Logic_Scoreboard_H

//#include "Input/InputManager.h"
#include "Net/Buffer.h"

namespace Logic{
	class CGUIManager;
	class CEntity;
}

namespace Hikari{
	class FlashControl;
}

namespace Logic{
/**
Clase que maneja el scoreboard de una partida online
<p>
Es una clase singleton que contiene un overlay flash que es el que se 
encarga de pintar el scoreboard. El scoreboard solo se debe mostrar cuando
se pulsa la tecla TAB, por lo que esta clase es un observer del teclado.
</p><p>
Las estructuras de datos que contiene esta clase son volcadas a Flash 
cuando se produce un cambio, de manera que la GUI está siempre actualizada
</p><p>
Cuando la tecla TAB es pulsada, la GUI se muestra en la pantalla. Esta GUI
contiene toda la información de estadísticas relevantes de la partida, como
la clase del jugador, las muertes, los frags, los mejores spree, etc.
</p>

*/
	class CScoreboard /*: public Input::CKeyboardListener*/{
	public:

		/**
		Estructura que contiene  toda la información referente a un player.
		Esta información será usada para avisar a la GUI de cómo van las 
		estadísticas que tiene que tener en su estructura de datos para 
		mostrarla por pantalla cuando se pulse la tecla TAB
		*/
		struct PlayerInfo{

			//struct default constructor
			PlayerInfo(	const std::string &name, CEntity* &player, const std::string &pClass, int playerTeam = 0, 
						unsigned int kills = 0, unsigned int death = 0,unsigned int spree = 0, unsigned int lag = 0):
						entityPlayer(player), name(name), playerClass(pClass), team(playerTeam), deaths(death),
						bestSpree(spree), ping(lag), kills(kills){}

			//struct members
			CEntity * entityPlayer;
			std::string name;
			int kills;
			unsigned int deaths;
			unsigned int bestSpree;
			unsigned int ping;
			unsigned int team;
			std::string playerClass;
		};

		CScoreboard();

		~CScoreboard();

		// =======================================================================
		//                          METODOS ESTÁTICOS
		// =======================================================================

		/**
		Devuelve la única instancia de la clase CScoreboard.
		
		@return Única instancia de la clase CScoreboard.
		*/
		static CScoreboard* getSingletonPtr(){return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static void Init();

		/**
		Libera la instancia de GUIKillersMessage. Debe llamarse en
		la destrucción de las estructuras de la lógica
		*/
		static void Release();

		// =======================================================================
		//                            METODOS PÚBLICOS
		// =======================================================================

		/**
		Método que añade un nuevo jugador a la estructura de jugadores de la partida.
		Inicializa todas las estructuras necesarias para almacenar toda la información
		de la partida.

		@param name Nombre del jugador que se quiere meter.
		@param entidad asociada al jugador.
		@param playerClass clase del jugador que se está creando.
		*/
		void addPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass);

		/**
		Método que añade al jugador local a la estructura de jugadores de la partida.
		Inicializa todas las estructuras necesarias para almacenar toda la información
		de la partida.

		@param name Nombre del jugador que se quiere meter.
		@param entidad asociada al jugador.
		@param playerClass clase del jugador que se está creando.
		*/
		void addLocalPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass);

		/**
		Método que cambia la entidad asociada a un jugador. Sirve para cuando el jugador
		cambia de clase que se pueda cambiar la entidad asociada y consultarla.

		@param name Nombre de la entidad que se está actualizando.
		@param newPlayerEntity entidad nueva del jugador.
		@param newClass clase nueva a la que pertenece la entidad del jugador.
		*/
		void changePlayerEntity(const std::string &name, CEntity * newPlayerEntity, const std::string &newClass);

		/**
		Método que elimina un jugador de la estructura de datos de players. Este 
		método debe ser llamado siempre que un jugador abandona la partida.

		@param name Nombre de la entidad que se está borrando.
		*/
		void deletePlayer(const std::string &name);

		/**
		Método que añade un frag al jugador indicado.

		@param name Nombre de la entidad que se está actualizando.
		*/
		void addKill(const std::string &name);

		/**
		Método que resta un frag al jugador indicado.

		@param name Nombre de la entidad que se está actualizando.
		*/
		void subKill(const std::string &name);

		/**
		Método que añade una muerte al jugador indicado.

		@param name Nombre de la entidad que se está actualizando.
		*/
		void addDeath(const std::string &name);

		/**
		Método que cambia el nombre de un jugador. Siempre se debe llamar a
		este método cuando el jugador cambia de nombre o la información del
		jugador dejará de actualizarse

		@param oldName Nombre que tenía la entidad antes.
		@param newName Nombre que tendrá a partir de ahora.
		*/
		void changeName(const std::string &oldName, const std::string &newName);

		/**
		Método que actualiza el ping que tiene el jugador indicado con respecto
		al servidor

		@param name Nombre de la entidad que se está actualizando.
		@param ping ping que tiene el jugador indicado.
		*/
		void changePing(const std::string &name, unsigned int &ping);

		/**
		Método que cambia la entidad asociada a un jugador. Sirve para cuando el jugador
		cambia de clase que se pueda cambiar la entidad asociada y consultarla.

		@param name Nombre de la entidad que se está actualizando.
		@param newSpree Killing spree actual del jugador
		*/
		void changeSpree(const std::string &name, unsigned int &newSpree);

		/**
		Método que consulta el mejor spree de un jugador concreto

		@param name El nombre del player que estamos consultando

		@return El mejor killing spree del jugador consultado
		*/
		unsigned int getSpree(const std::string &name);

		/**
		Método que limpia la lista de players
		*/
		void clearPlayers();

		/**
		Método que activa el scoreboard para la lógica
		*/
		void activate();

		/**
		Método que desactiva el scoreboard para la lógica
		*/
		void deactivate();

		/**
		Método que te devuelve un jugador de la estructura dado un nombre

		@param name el nombre del player que estamos consultando

		@return la entidad con ese nombre
		*/
		CEntity* getPlayer(const std::string &name);

		/**
		Método que desactiva el scoreboard para la lógica
		*/
		void loadScoreboardDM();

		/**
		Método que carga el GUI en memoria y lo prepara para mostrarlo
		*/
		void loadScoreboardTDM();

		/**
		Método que quita el GUI de memoria
		*/
		void unLoadScoreboard();

		/**
		Método que te devuelve una lista con todos los players y sus estadísticas

		@return Una lista con todos los players y sus estadísticas
		*/
		std::vector<PlayerInfo> getAllPlayers();

		Net::CBuffer serialize();
		void deserialize(Net::CBuffer &players);
		// =======================================================================
		//                 METODOS HEREDADOS DE CKEYBOARDLISTENER
		// =======================================================================

		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		//bool keyPressed(Input::TKey key);

		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		//bool keyReleased(Input::TKey key);

	private:
		/**
		Única instancia de la clase.
		*/
		static CScoreboard* _instance;

		/**
		Instancia del GUIManager
		*/
		CGUIManager* _guiManager;

		/**
		Tabla donde tenemos almacenados todos los jugadores de la partida
		junto con sus estadísticas.
		*/
		std::map<std::string, PlayerInfo> _players;

		/**
		typedef para comodidad
		*/
		typedef std::pair<std::string, PlayerInfo> TPlayerInfo;

		/**
		Variable que guarda el control de flash. Lo guardamos porque lo vamos
		a utilizar mucho y llamar muchas veces al manager es muy costoso
		*/
		Hikari::FlashControl * _scoreboard;
	};

}

#endif