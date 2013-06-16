//---------------------------------------------------------------------------
// Manager.h
//---------------------------------------------------------------------------

/**
@file Manager.h

Contiene la declaración de la clase CManager, Singleton que se encarga de
la gestión de la red del juego.

@see Net::CManager

@author David Llansó
@author Francisco Aisa García
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

#ifndef __Net_Manager_H
#define __Net_Manager_H

#include <vector>
#include <set>
#include <queue>
#include <map>

// Predeclaracion de clases
namespace Net {
	class CBuffer;
	class CServidor;
	class CCliente;
	class CConexion;
	class CFactoriaRed;
	class CFactoriaRedEnet;
	class CPaquete;
	class CNetIdDispatcher;
};

/**
Namespace que engloba lo relacionado con la parte de red.
(para más información ver @ref NetGroup).

@author David Llansó
@author Francisco Aisa García
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

namespace Net {

	/** ID de identificación en la red. */
	typedef unsigned int NetID;

	/** Tipos de mensajes que pueden mandarse por la red. */
	enum NetMessageType {
		SEND_CLIENT_INFO,
		CLIENT_INFO,
		UPDATE_CLIENT,
		CLIENT_UPDATED,
		SEND_PLAYER_INFO,
		PLAYER_INFO,
		LOAD_MAP,
		MAP_LOADED,
		GAME_SETTINGS,
		GAME_SETTINGS_LOADED,
		LOAD_PLAYERS,
		//PLAYERS_LOADED,
		LOAD_WORLD_STATE,
		WORLD_STATE_LOADED,
		PLAYER_DISCONNECTED,
		START_GAME,
		END_GAME,
		DISCONNECT,

		CLASS_SELECTED,
		LOAD_LOCAL_PLAYER,
		LOCAL_PLAYER_LOADED,

		SPECTATE_REQUEST,
		LOAD_LOCAL_SPECTATOR,
		LOAD_SPECTATORS,

		COMMAND,
		PING,
		ENTITY_MSG,
		ASSIGNED_ID,

		CREATE_ENTITY,
		CREATE_CUSTOM_ENTITY,
		DESTROY_ENTITY,
		DEACTIVATE_ENTITY,
		ACTIVATE_ENTITY
	};

	/**
	Gestor de la red. Sirve como interfaz para que el resto de los
	proyectos interactúen con la red y no tengan que preocuparse de 
	la tecnología específica usada para su implementación.
	<p>
	Esta clase debe ser específicamente inicializada o configurada
	como cliente <em>o</em> como servidor. Sin embargo, desde fuera
	<em>puede usarse sin conocer su configuración</em> y así invocar 
	métodos como CManager::tick() para que compruebe si se ha recibido 
	algún mensaje o invocar a CManager::send() cuando se quiera enviar 
	un mensaje al otro extremo de la conexión.

	@ingroup NetGroup

	@author David Llansó
	@author Francisco Aisa García
	@author Ruben Mulero Guerrero
	@date Febrero, 2013
	*/
	class CManager {
	public:


		// =======================================================================
		//                            CLASS IOBSERVER
		// =======================================================================


		/**
		Interfaz que deben implementar las clases que quieran ser 
		notificadas de eventos de red.
		*/
		class IObserver {
		public:

			/**
			Se dispara cuando se recibe un paquete de datos.

			@param packet Paquete de datos recibido.
			*/
			virtual void dataPacketReceived(Net::CPaquete* packet) = 0;

			//________________________________________________________________________

			/**
			Se dispara cuando se recibe un paquete de conexión.

			@param packet Paquete recibido.
			*/
			virtual void connectionPacketReceived(Net::CPaquete* packet) = 0;

			//________________________________________________________________________

			/**
			Se dispara cuando se recibe un paquete de desconexión.

			@param packet Paquete recibido.
			*/
			virtual void disconnectionPacketReceived(Net::CPaquete* packet) = 0;
		};


		// =======================================================================
		//                  METODOS DE INICIALIZACION Y ACTIVACION
		// =======================================================================


		/**
		Devuelve la única instancia de la clase CManager.
		
		@return Única instancia de la clase CManager.
		*/
		static CManager* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera la instancia de CManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();


		// =======================================================================
		//            MÉTODOS DE ENVÍO Y RECEPCIÓN DE DATOS POR LA RED
		// =======================================================================


		/**
		Realiza una conexión a un servidor dada una ip y un puerto. Éste método solo
		debe ser usado por los clientes.

		@param address Dirección IP o nombre de la máquina remota a conectar
		@param puerto Puerto remoto al que conectar.
		@param channels Número de canales lógicos a crear sobre la conexión
		@param timeout Tiempo máximo de espera para conectar con la máquina remota.
		@return true si la conexión se realizó con éxito.
		*/
		bool connectTo(char* address, int port, int channels = 1, unsigned int timeout = 5);

		//________________________________________________________________________

		/**
		Desactiva la conexión (tanto para cliente como para servidor).
		*/
		void deactivateNetwork();

		//________________________________________________________________________

		/**
		Función que sirve para enviar datos al otro lado de la conexión.
		Si se está en modo cliente los datos se enviarán al servidor
		mientras que si se encuentra en modo servidor la información se
		enviará a todos los clientes registrados.

		@param data Datos a enviar.
		@param longdata Tamaño de los datos a enviar.
		*/
		void broadcast(void* data, size_t longdata);

		//________________________________________________________________________

		/**
		Manda el mensaje a todos los clientes exceptuando al cliente con el id de
		red dado.

		@param id Id de red del cliente que queremos que NO reciba el mensaje.
		@param data Datos a enviar.
		@param longdata Tamaño de los datos a enviar.
		*/
		void broadcastIgnoring(Net::NetID id, void* data, size_t longdata);

		//________________________________________________________________________

		/**
		Manda el mensaje al cliente con el id de red dado.
		
		@param id Id de red del cliente al que queremos mandar el mensaje.
		@param data Datos a enviar.
		@param longdata Tamaño de los datos a enviar.
		*/
		void sendTo(Net::NetID id, void* data, size_t longdata);


		// =======================================================================
		//            MÉTODOS DE PROCESADO, CONSULTA Y CONFIGURACIÓN
		// =======================================================================


		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Será aquí donde consultaremos si han llegado mensajes y de ser así 
		los procesaremos o delegaremos su proceso al módulo correspondiente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Construye un servidor con la configuración y el puerto dado.

		@param port Puerto que el servidor usará para escuchar.
		@param clients Número de clientes que soporta el servidor.
		@param maxinbw Ancho de banda entrante.
		@param maxoutbw Ancho de banda saliente.
		*/
		void activateAsServer(int port, int clients = 16, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		//________________________________________________________________________

		/**
		Construye un cliente con la configuración y el puerto dado.

		@param maxConnections Numero de conexiones máximas (debería ser uno siempre).
		@param maxinbw Ancho de banda entrante.
		@param maxoutbw Ancho de banda saliente.
		*/
		void activateAsClient(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		//________________________________________________________________________

		/**
		Devuelve true si el manager se está ejecutando como servidor.

		@return true si el manager se está ejecutando como servidor.
		*/
		bool imServer() { return _servidorRed != NULL; }

		//________________________________________________________________________

		/**
		Devuelve true si el manager se está ejecutando como cliente.

		@return true si el manager se está ejecutando como cliente.
		*/
		bool imClient() { return _clienteRed != NULL; }

		//________________________________________________________________________

		/**
		Devuelve el ID de red.
		
		@return El ID de red.
		*/
		NetID getID() { return _id; }


		// =======================================================================
		//                       MÉTODOS PARA OBSERVERS
		// =======================================================================


		/**
		Añade un nuevo observador a la lista de observadores.

		@param listener Objeto que vamos añadir a la lista de observadores.
		*/
		void addObserver(IObserver* listener);

		//________________________________________________________________________

		/**
		Elimina un observador de la lista de observadores.

		@param listener Objeto que vamos a eliminar de la lista de observadores.
		*/
		void removeObserver(IObserver* listener);

		/**
		Dada una "conexión" realiza la desconexión.

		@param connection Objeto que representa la conexión.
		*/
		void disconnect(CConexion* connection);
		//________________________________________________________________________

		/**
		Devuelve el número de clientes conectados. En el caso de ser un cliente
		devuelve 1.

		@return Número de clientes conectados.
		*/
		unsigned int nbClients() {
			return _connections.size();
		}


	protected:


		// =======================================================================
		//             MÉTODOS PROTEGIDOS DE CONSTRUCCIÓN DEL SINGLETON
		// =======================================================================


		/** Constructor por defecto. */
		CManager ();

		//________________________________________________________________________

		/** Destructor. */
		~CManager();

		//________________________________________________________________________

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		//________________________________________________________________________

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();


		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================


		/**
		Realiza el procesado de los paquetes recibidos.

		@param _paquetes Vector que contiene todos los paquetes recibidos.
		*/
		void getPackets(std::vector<Net::CPaquete*>& _paquetes);

		//________________________________________________________________________

		/**
		Escribe en un buffer las cabeceras de red que corresponden en función del
		paquete recibido.

		@param packet Paquete recibido.
		@return true Si se asigna id.
		*/
		bool internalData(Net::CPaquete* packet);

		//________________________________________________________________________

		/**
		Dada una "conexión" realiza la conexión.

		@param connection Objeto que representa la conexión.
		*/
		void connect(CConexion* connection);

		//________________________________________________________________________




	private:


		// =======================================================================
		//                          MÉTODOS PRIVADOS
		// =======================================================================


		/**
		Devuelve la conexión asociada a un id de red. Útil para el servidor.

		@param id Id de red de la conexión que buscamos.
		@return Conexión asociada al id de red dado.
		*/
		CConexion* getConnection(NetID id) { return _connections.find(id)->second; }

		//________________________________________________________________________

		/**
		Añade una conexión dado un id de red.

		@param id Id de red al que queremos asociar una conexión.
		@param connection Conexión que queremos asociar al id de red dado.
		@return true si la asociación se ha realizado con éxito.
		*/
		bool addConnection(NetID id, CConexion* connection);

		//________________________________________________________________________

		/**
		Elimina una conexíon de la lista de conexiones dado un id de red.

		@param id Id de red que queremos eliminar de la lista de conexiones.
		@return true si la conexión ha podido eliminarse de forma satisfactoria.
		*/
		bool removeConnection(NetID id);


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Única instancia de la clase. */
		static CManager* _instance;

		/** Factoría de objetos de red. */
		Net::CFactoriaRed* _factoriaRed;

		/** Servidor de red. */
		Net::CServidor* _servidorRed;

		/** Cliente de red. */
		Net::CCliente* _clienteRed;

		// Typedefs para el manejo de tablas de conexión.
		typedef std::pair<NetID, CConexion*> TConnectionPair;
		typedef std::map<NetID, CConexion*> TConnectionTable;

		/**
		Conexiones de red. Es decir, el servidor visto desde el cliente
		o los clientes vistos desde el servidor. En el cliente solo se 
		usará una y en el servidor tantas como clientes haya.
		*/
		TConnectionTable _connections;

		/** Vector de observadores. */
		std::set<IObserver*> _observers;

		std::queue<IObserver*> _observersToBeDeleted;

		/** Vector de paquetes recibidos. */
		std::vector<Net::CPaquete*> _paquetes;

		/** ID de red una vez conectado. */
		NetID _id;

		/** Asigna ids de red */
		CNetIdDispatcher* _idDispatcher;

	}; // class CManager

} // namespace Net

#endif // __Net_Manager_H