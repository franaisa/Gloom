/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la lógica del juego.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/GameNetMsgManager.h"
#include "Logic/GameSpawnManager.h"
#include "Logic/GameNetPlayersManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/GUIManager.h"

#include "Map/MapParser.h"

#include <cassert>

namespace Logic {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _map(0), 
						 _player(0), 
						 COMPONENT_CONSTRUCTOR_COUNTER(0), 
						 COMPONENT_DESTRUCTOR_COUNTER(0),
						 MESSAGE_CONSTRUCTOR_COUNTER(0),
						 MESSAGE_DESTRUCTOR_COUNTER(0)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

		std::cout << "COMPONENT_CONSTRUCTOR_COUNTER = " << COMPONENT_CONSTRUCTOR_COUNTER << std::endl;
		std::cout << "COMPONENT_DESTRUCTOR_COUNTER = " << COMPONENT_DESTRUCTOR_COUNTER << std::endl;

		std::cout << "MESSAGE_CONSTRUCTOR_COUNTER = " << MESSAGE_CONSTRUCTOR_COUNTER << std::endl;
		std::cout << "MESSAGE_DESTRUCTOR_COUNTER = " << MESSAGE_DESTRUCTOR_COUNTER << std::endl;
	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		// Inicializamos el parser de mapas.
		if (!Map::CMapParser::Init())
			return false;

		// Inicializamos la factoría de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;
		// Inicializamos el gestor de los mensajes de red durante
		// el estado de juego
		if (!Logic::CGameNetMsgManager::Init())
			return false;
		_gameNetMsgManager = Logic::CGameNetMsgManager::getSingletonPtr();

		//Inicializamos el gestor de spawn/respawn
		if (!Logic::CGameSpawnManager::Init())
			return false;
		_gameSpawnManager = Logic::CGameSpawnManager::getSingletonPtr();

		// Inicializamos el gestor de jugadores en red
		if(!Logic::CGameNetPlayersManager::Init())
			return false;

		// Inicializamos el manager de eventos de GUI
		if(!Logic::CGUIManager::Init())
			return false;
		_guiManager = Logic::CGUIManager::getSingletonPtr(); 

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		unLoadLevel();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();

		Logic::CGameNetMsgManager::Release();

		Logic::CGameSpawnManager::Release();

		Logic::CGUIManager::Release();

	} // close

	//--------------------------------------------------------

	bool CServer::loadLevel(const std::string &filename, const std::string &ambit)
	{
		// solo admitimos un mapa cargado, si iniciamos un nuevo nivel 
		// se borra el mapa anterior.

		if(_map = CMap::createMapFromFile(filename,ambit))
		{
			return true;
		}

		return false;

	} // loadLevel

	//--------------------------------------------------------

	void CServer::unLoadLevel()
	{
		if(_map)
		{
			_map->deactivate();
			_gameSpawnManager->deactivate();
			_gameNetMsgManager->deactivate();
			delete _map;
			_map = 0;
		}
		_player = 0;
		_guiManager->deactivate();
	} // unLoadLevel

	//---------------------------------------------------------

	bool CServer::activateMap() {
		_gameSpawnManager->activate();
		_gameNetMsgManager->activate();
		_guiManager->activate();

		// Activamos el mapa
		bool success = _map->activate();
		// Si no ha habido problemas, ejecutamos
		// el start de todas las entidades
		if(success)
			_map->start();

		return success;

	} // activateMap

	//---------------------------------------------------------

	void CServer::deactivateMap() 
	{
		_gameSpawnManager->deactivate();
		_gameNetMsgManager->deactivate();
		
		_map->deactivate();

	} // deactivateMap

	//---------------------------------------------------------

	void CServer::tick(unsigned int msecs) {
		// Hacemos el tick al gestor del mapa.
		_map->tick(msecs);

		//_guiManager->tick(msecs);
		//tick de GUI

		// Eliminamos las entidades que se han marcado para ser eliminadas.
		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();
	} // tick

	//---------------------------------------------------------

	void CServer::setFixedTimeStep(unsigned int stepSize) {
		_map->setFixedTimeStep(stepSize);
	}

} // namespace Logic
