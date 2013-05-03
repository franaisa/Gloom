/**
@file Manager.cpp

Contiene la implementación de la clase CManager, Singleton que se encarga de
la gestión de la red del juego.

@see Net::CManager

@author Francisco Aisa García
@author Ruben Mulero Guerrero
@date Febrero, 2013
*/

#include "Manager.h"

#include "buffer.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Messages/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "factoriaredenet.h"
#include "factoriared.h"
#include "NetIdDispatcher.h"
#include <cassert>

namespace Net {

	CManager* CManager::_instance = 0;

	//--------------------------------------------------------

	CManager::CManager(): _factoriaRed(0), 
						  _servidorRed(0), 
						  _clienteRed(0),
						  _idDispatcher(0) {
		
		_instance = this;
	} // CManager

	//--------------------------------------------------------

	CManager::~CManager() {
		if(_idDispatcher != NULL) {
			delete _idDispatcher;
			_idDispatcher = NULL;
		}

		_instance = 0;
	} // ~CManager
	
	//--------------------------------------------------------

	bool CManager::Init() {
		assert(!_instance && "Segunda inicialización de Net::CManager no permitida!");

		new CManager();

		if ( !_instance->open() ) {
			Release();
			return false;
		}

		return true;
	} // Init

	//--------------------------------------------------------

	void CManager::Release() {
		assert(_instance && "Net::CManager no está inicializado!");

		if(_instance) {
			_instance->close();
			delete _instance;
			_instance = 0;
		}

	} // Release

	//--------------------------------------------------------

	bool CManager::open() {
		 // Inicializamos la factoria de objetos de red
		_factoriaRed = new Net::CFactoriaRedEnet();

		return true;
	} // open

	//--------------------------------------------------------

	void CManager::close() {
		deactivateNetwork();
		if(_factoriaRed)
			delete _factoriaRed;
	} // close

	//---------------------------------------------------------

	void CManager::broadcast(void* data, size_t longdata) {
		// Si hay jugadores conectados
		if(!_connections.empty()) {
			// Si somos el servidor realizar un broadcast a todos los clientes
			if(_servidorRed)
				_servidorRed->sendAll(data, longdata, 0, 1);

			// Si somos el cliente enviamos la informacion al servidor
			if(_clienteRed)
				_clienteRed->sendData(getConnection(_idDispatcher->getServerId()), data, longdata, 0, 1);
		}
	} // send

	//---------------------------------------------------------

	void CManager::sendTo(Net::NetID id, void* data, size_t longdata) {
		if(!_connections.empty()) {
			// Si somos el servidor mandamos el mensaje al cliente que nos han indicado
			// por parametro
			if(_servidorRed)
				_servidorRed->sendData(_connections.find(id)->second, data, longdata, 0, 1);

			// Si somos el cliente enviamos la información al servidor
			if(_clienteRed)
				_clienteRed->sendData(getConnection(_idDispatcher->getServerId()), data, longdata, 0, 1);
		}
	} // send

	//---------------------------------------------------------

	void CManager::broadcastIgnoring(Net::NetID id, void* data, size_t longdata) {
		// Si hay jugadores conectados
		if(!_connections.empty()) {
			// Si somos el servidor realizar un broadcast a todos los clientes
			if(_servidorRed) {
				TConnectionTable::iterator it = _connections.find(id);
				assert(it != _connections.end() && "broadcastIgnoring no puede ejecutarse porque no existe ninguna conexion con el id dado");

				_servidorRed->sendAllExcept(data, longdata, 0, 1, it->second);
			}
		}
	} // sendAllExcept

	//---------------------------------------------------------

	void CManager::tick(unsigned int msecs) 
	{
		_paquetes.clear();
		Net::CManager::getSingletonPtr()->getPackets(_paquetes);
		
		for(std::vector<Net::CPaquete*>::iterator iterp = _paquetes.begin();iterp != _paquetes.end();++iterp)
		{
			Net::CPaquete* paquete = *iterp;
			// El mensaje debe ser de tipo CONEXION
			
			switch (paquete->getTipo())
			{
				case Net::CONEXION:
					connect(paquete->getConexion());
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->connectionPacketReceived(paquete);
					break;
				case Net::DATOS:
					
					if(!internalData(paquete)){ // Analiza si trae contenido -> TODO: ver funcion
						//std::cout << "mensaje recibido:  " <<  _observers.size() << std::endl;
						for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
							(*iter)->dataPacketReceived(paquete);
					}
					break;
				case Net::DESCONEXION:
					for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
						(*iter)->disconnectionPacketReceived(paquete);
					disconnect(paquete->getConexion());
					//retornar id
					break;
			}
			delete paquete;
		}

	} // tick

	//---------------------------------------------------------

	void CManager::activateAsServer(int port, int clients, unsigned int maxinbw, unsigned int maxoutbw) {
		_idDispatcher = new CNetIdDispatcher(clients);

		//Creamos el servidor de red
		_servidorRed = _factoriaRed->buildServidor();

		_servidorRed->init(port,clients, maxinbw, maxoutbw);

		_id = _idDispatcher->getServerId();

	} // activateAsServer

	//---------------------------------------------------------

	void CManager::activateAsClient(unsigned int maxConnections, unsigned int maxinbw, unsigned int maxoutbw)
	{
		//Creamos el servidor de red
		_clienteRed = _factoriaRed->buildCliente();

		_clienteRed->init(maxConnections, maxinbw, maxoutbw);

	} // activateAsClient

	//---------------------------------------------------------

	bool CManager::connectTo(char* address, int port, int channels, unsigned int timeout)
	{
		assert(_clienteRed && "Cliente Red es null"); // Solo se puede ejecutar el connectTo si somos cliente
		assert(_connections.empty() && "Ya hay una conexion"); // Capamos al cliente a 1 conexión max: la de con el server

		CConexion* connection = _clienteRed->connect(address, port, channels, timeout); // CONNECT
		
		if(!connection)
			return false;
		
		// Almacenamos esa conexión y le otorgamos un ID de red
		NetID serverId = _idDispatcher->getServerId();
		connection->setId(serverId); // Un cliente sólo se conecta al SERVER
		addConnection(serverId, connection); // Guardamos en la tabla

		return true;
	} // connectTo

	//---------------------------------------------------------

	bool CManager::internalData(Net::CPaquete* packet)
	{
		Net::CBuffer data;
		data.write(packet->getData(),packet->getDataLength());
		data.reset();
		
		Net::NetMessageType msg;
		data.read(&msg,sizeof(msg));
		switch (msg)
		{
		case Net::ASSIGNED_ID: 
			// Escribimos el id de red que corresponde a este cliente
			data.read(&_id,sizeof(_id));
			return true;
		default:
			return false;
		}

	} // internalData

	//---------------------------------------------------------

	void CManager::connect(CConexion* connection)// Una nueva conexión de un cliente al sevidor
	{
		unsigned int nextId = _idDispatcher->getNextClientId();

		// Almacenamos esa conexión y le otorgamos un ID de red
		connection->setId(nextId);
		addConnection(nextId,connection);

		NetMessageType type = Net::ASSIGNED_ID;// Escribimos el tipo de mensaje de red a enviar
		CBuffer buf;// Avisamos al cliente de cual es su nuevo ID	
			buf.write(&type,sizeof(type));		
			buf.write(&nextId,sizeof(nextId));// Escribimos el id del cliente
		_servidorRed->sendData(connection, buf.getbuffer(),buf.getSize(),0,1);
	} // connect

	//---------------------------------------------------------

	void CManager::disconnect(CConexion* connection) {
		if(_servidorRed) {
			unsigned int clientId = connection->getId();
			
			_servidorRed->disconnect(connection);
			removeConnection(clientId);

			_idDispatcher->returnClientId(clientId);
		}
		else if(_clienteRed) {
			NetID serverId = _idDispatcher->getServerId();
			_clienteRed->disconnect(getConnection(serverId));
			removeConnection(serverId);
		}
	} // disconnect
		
	//---------------------------------------------------------

	bool CManager::addConnection(NetID id, CConexion* connection) {
		if(_connections.count(id))
			return false;

		// Insertamos una nueva conexion con tan solo la informacion de conexion
		TConnectionPair elem(id, connection);
		// Insertamos par id - playerInfo con la conexion correspondiente
		_connections.insert(elem);
		
		return true;
	} // addConection
		
	//---------------------------------------------------------

	bool CManager::removeConnection(NetID id) {
		if(_connections.count(id)) {
			CConexion* connection = getConnection(id);
			_connections.erase(id);
			delete connection;
			
			return true;
		}
		
		return false;
	} // removeConection

	//---------------------------------------------------------

	void CManager::deactivateNetwork() {
		if(_servidorRed) {
			_servidorRed->deInit(); // TODO analizar que haga disconnect de todo; antes if(_conexion) disconnect (OJO estaba comentado)
			delete _servidorRed;
			_servidorRed = 0;
		}
		if(_clienteRed) {
			_clienteRed->deInit();
			delete _clienteRed;
			_clienteRed = 0;
		}
		if(!_connections.empty()) {
			for(TConnectionTable::const_iterator it = _connections.begin(); it != _connections.end(); it++)
				delete it->second;
			
			_connections.clear(); // Quien hace el disconnect
		}
	} // deactivateNetwork

	//---------------------------------------------------------

	void CManager::getPackets(std::vector<Net::CPaquete*>& _paquetes) {
		if(_servidorRed)
			_servidorRed->service(_paquetes);
		if(_clienteRed)
			_clienteRed->service(_paquetes);

	} // getPackets

	//---------------------------------------------------------

	void CManager::addObserver(IObserver* listener) {
		_observers.push_back(listener);
	} // addObserver

	//---------------------------------------------------------

	void CManager::removeObserver(IObserver* listener)
	{
		for(std::vector<IObserver*>::iterator iter = _observers.begin();iter != _observers.end();++iter)
			if((*iter)==listener)
			{
				_observers.erase(iter);
				break;
			}
	} // removeObserver

} // namespace Net
