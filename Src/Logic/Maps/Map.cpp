/**
@file Map.cpp

Contiene la implementación de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/

#include "Map.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "EntityFactory.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "Net/Manager.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "Logic/Messages/MessageHudDebugData.h"

#include <cassert>
#include <fstream>

using namespace std;

// HACK. Debería leerse de algún fichero de configuración
#define MAP_FILE_PATH "./media/maps/"

namespace Logic {
		
	CMap* CMap::createMapFromFile(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(MAP_FILE_PATH);
		completePath = completePath + filename;
		// Parseamos el fichero
		if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath))
		{
			assert(!"No se ha podido parsear el mapa.");
			return false;
		}

		// Si se ha realizado con éxito el parseo creamos el mapa.
		CMap *map = new CMap(filename);

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		Map::CMapParser::TEntityList::const_iterator it, end;
		it = entityList.begin();
		end = entityList.end();
		
		// Creamos todas las entidades lógicas.
		for(; it != end; it++)
		{
			CEntity *entity;
			Map::CEntity * info = entityFactory->getInfo((*it)->getType());
			if(!info){
				entity = entityFactory->createEntity((*it),map, false);
			}else{
				info->setAttribute((*it));
				info->setName((*it)->getName());
				entity = entityFactory->createEntity(info,map, false);
			}
			assert(entity && "No se pudo crear una entidad del mapa");
		}

		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	CMap::CMap(const std::string &name) : _numOfPlayers(0) {
		_name = name;
		_scene = Graphics::CServer::getSingletonPtr()->createScene(name);

	} // CMap

	//--------------------------------------------------------

	CMap::~CMap() {
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_scene);

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate() {
		_acumTime = 0;
		_fixedTimeStep = 16;
		Graphics::CServer::getSingletonPtr()->setScene(_scene);

		/*TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();*/

		list<CEntity*>::const_iterator it = _entityList.begin();
		list<CEntity*>::const_iterator end = _entityList.end();

		bool correct = true;

		// Activamos todas las entidades registradas en el mapa.
		for(; it != end; ++it)
			correct = (*it)->activate() && correct;

		return correct;

	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate() {
		/*TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();*/

		list<CEntity*>::const_iterator it = _entityList.begin();
		list<CEntity*>::const_iterator end = _entityList.end();

		// Desactivamos todas las entidades activas registradas en el mapa.
		for(; it != end; ++it) {
			if((*it)->isActivated()) {
				(*it)->deactivate();
			}
		}

		Graphics::CServer::getSingletonPtr()->setScene(0);

	} // getEntity

	//---------------------------------------------------------

	void CMap::wantsTick(CEntity* entity) {
		_entitiesWithTick.push_back(entity);
	}

	void CMap::wantsFixedTick(CEntity* entity) {
		_entitiesWithFixedTick.push_back(entity);
	}

	void CMap::start() {
		// Ejecutamos el start de todas nuestras entidades
		/*TEntityMap::const_iterator it = _entityMap.begin();
		for(; it != _entityMap.end(); ++it ) {
			// Ejecutamos el start de todas las entidades
			it->second->start();
		}*/

		list<CEntity*>::const_iterator it = _entityList.begin();
		list<CEntity*>::const_iterator end = _entityList.end();

		for(; it != end; ++it) {
			(*it)->start();
		}
	}

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) {
		// Comprobamos los timers de las entidades que tienen
		// un tiempo de vida
		checkTimeouts(msecs);
		
		// Es muuuuuy importante que primero se ejecute esta función
		// si no se hace así, el sleep por ejemplo no funciona bien.
		processComponentMessages();
		
		// Ejecutamos el tick de las entidades
		// Ejecutamos el tick de todas las entidades activadas del mapa
		// La propia entidad se encarga de hacer el process, tick y fixed tick
		// de sus componentes (dependiendo del estado)
		doTick(msecs);

		doFixedTick(msecs);	
	} // tick

	//--------------------------------------------------------

	void CMap::checkTimeouts(unsigned int msecs) {
		// Si hay entidades con cronometros de autodestruccion
		// comprobamos el cronometro y si hay que destruirlas avisamos
		// a la factoria para que se encargue de ello en diferido.
		if( !_entitiesWithTimeout.empty() ) {
			std::list< std::pair<CEntity*, unsigned int> >::iterator it = _entitiesWithTimeout.begin();
			while( it != _entitiesWithTimeout.end() ) {
				// Dado que estamos tratando con un unsigned int, comprobamos si la resta saldria menor
				// que 0, ya que si hacemos directamente la resta el numero pasa a ser el unsigned int 
				// mas alto y por lo tanto seria un fail.
				it->second = msecs >= it->second ? 0 : (it->second - msecs);

				if(it->second <= 0) {
					// Puesto por defecto a true pero deberia de ser apuntado cuando se llama 
					// al createEntityWithTimeOut y recuperarse al llegar a este caso
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(it->first, true);
					it = _entitiesWithTimeout.erase(it);
				}
				else {
					++it;
				}
			}
		}
	}

	//--------------------------------------------------------

	void CMap::processComponentMessages() {
		// Ejecutamos el tick de todas nuestras entidades
		/*TEntityMap::const_iterator it = _entityMap.begin();
		for(; it != _entityMap.end(); ++it) {
			it->second->processComponentMessages();
		}*/

		list<CEntity*>::const_iterator it = _entityList.begin();
		list<CEntity*>::const_iterator end = _entityList.end();

		for(; it != end; ++it) {
			(*it)->processComponentMessages();
		}
	}

	//--------------------------------------------------------
	
	void CMap::doTick(unsigned int msecs) {
		// Ejecutamos el tick de las entidades que lo quieren
		CEntity* entity;
		std::list<CEntity*>::iterator it = _entitiesWithTick.begin();
		while(it != _entitiesWithTick.end()) {
			entity = *it;

			if( !entity->tick(msecs) ) {
				it = _entitiesWithTick.erase(it);
				continue;
			}

			++it;
		}
	}

	//--------------------------------------------------------
	
	void CMap::doFixedTick(unsigned int msecs) {
		_acumTime += msecs;
		unsigned int steps = _acumTime / _fixedTimeStep;
		_acumTime = _acumTime % _fixedTimeStep; 

		// Ejecutamos el fixed tick
		for(int i = 0; i < steps; ++i) {
			CEntity* entity;
			std::list<CEntity*>::iterator it = _entitiesWithFixedTick.begin();
			while( it != _entitiesWithFixedTick.end() ) {
				entity = *it;

				if( !entity->fixedTick(_fixedTimeStep) ) {
					it = _entitiesWithFixedTick.erase(it);
					continue;
				}

				++it;
			}
		}

	}

	//--------------------------------------------------------

	typedef std::pair<TEntityID,CEntity*> TEntityPair;

	void CMap::addEntity(CEntity *entity) {
		_entityList.push_front(entity);
		std::list<CEntity*>::const_iterator listIt = _entityList.begin();

		_entitiesWithTick.push_front(entity);
		std::list<CEntity*>::const_iterator tickIt = _entitiesWithTick.begin();

		_entitiesWithFixedTick.push_front(entity);
		std::list<CEntity*>::const_iterator fixedTickIt = _entitiesWithFixedTick.begin();

		TEntityID entityId = entity->getEntityID();
		// Añadimos la entidad si no existia
		if( _entityInfoTable.find(entityId) == _entityInfoTable.end() ) {
			EntityInfo info;
			info._entityPtr = entity;
			info._processIterator = listIt;
			info._tickIterator = tickIt;
			info._fixedTickIterator = fixedTickIt;
			
			_entityInfoTable[entityId] = info;
		}

		/*if(_entityMap.count(entity->getEntityID()) == 0) {
			TEntityPair elem(entity->getEntityID(), entity);
			_entityMap.insert(elem);
		}*/

		
	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity) {
		// Eliminamos la entidad si existe
		std::unordered_map<TEntityID, EntityInfo>::const_iterator it = _entityInfoTable.find( entity->getEntityID() );
		if( it != _entityInfoTable.end() ) {
			EntityInfo info = it->second;

			if(info._entityPtr->isActivated())
				info._entityPtr->deactivate();

			info._entityPtr->_map = NULL;
			
			// Borramos el elemento de cada lista en la que este
			// presente
			if( info._processIterator != _entityList.end() ) 
				_entityList.erase(info._processIterator);
			if( info._tickIterator != _entitiesWithTick.end() ) 
				_entitiesWithTick.erase(info._tickIterator);
			if( info._fixedTickIterator != _entitiesWithFixedTick.end() ) 
				_entitiesWithFixedTick.erase(info._fixedTickIterator);

			_entityInfoTable.erase(it);
		}
	} // removeEntity

	//--------------------------------------------------------

	// @deprecated WHAT THE FUCK? hay que decirle a la factoria que llame a un metodo nuestro
	// para borrar. Pero es que estamos tontos ya o q? encima al ejecutarlo hay que hacer una
	// busqueda. Esto es ineficiencia por deporte...
	void CMap::destroyAllEntities() {
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		/*TEntityMap::const_iterator it, end;
		it = _entityMap.begin();
		end = _entityMap.end();*/

		std::unordered_map<TEntityID, EntityInfo>::const_iterator it = _entityInfoTable.begin();
		std::unordered_map<TEntityID, EntityInfo>::const_iterator end = _entityInfoTable.end();

		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		for(; it != end; ++it) {	
			entityFactory->deleteEntity(it->second._entityPtr);
		}

		//_entityMap.clear();
		//_entitiesWithTick.clear();
		//_entitiesWithFixedTick.clear();

	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID) {
		std::unordered_map<TEntityID, EntityInfo>::const_iterator it = _entityInfoTable.find(entityID);
		if( it != _entityInfoTable.end() )
			return it->second._entityPtr;
		
		return NULL;

		/*if(_entityMap.count(entityID) == 0)
			return 0;
		return (*_entityMap.find(entityID)).second;*/
	} // getEntityByID

	//--------------------------------------------------------

	// @deprecated esta funcion es un mojon como una casa. Deberiamos tener varias
	// tablas de punteros a informacion del player (tal y como se hace en el gestor de jugadores)
	// es un poco mas complejo de mantener pero infinitamente mas eficiente de ejecutar
	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start) {
		std::unordered_map<TEntityID, EntityInfo>::const_iterator it = _entityInfoTable.begin();
		for(; it != _entityInfoTable.end(); ++it) {
			if( it->second._entityPtr->getName() == name )
				return it->second._entityPtr;
		}

		return NULL;
		
		/*TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start) {
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++) {
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getName().compare(name))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;*/
	} // getEntityByName

	//--------------------------------------------------------

	// @deprecated esta funcion es un mojon como una casa. Deberiamos tener varias
	// tablas de punteros a informacion del player (tal y como se hace en el gestor de jugadores)
	// es un poco mas complejo de mantener pero infinitamente mas eficiente de ejecutar
	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start) {
		std::unordered_map<TEntityID, EntityInfo>::const_iterator it = _entityInfoTable.begin();
		for(; it != _entityInfoTable.end(); ++it) {
			std::cout << it->second._entityPtr->getName() << std::endl;
			if( it->second._entityPtr->getType() == type )
				return it->second._entityPtr;
		}

		return NULL;

		/*TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start)
		{
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++)
		{
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getType().compare(type))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;
		*/
	} // getEntityByType

	//--------------------------------------------------------
	//--------------------------------------------------------
	
	CEntity* CMap::createPlayer(const std::string &name, const std::string &type) {

		//cogemos el player necesario
		Map::CEntity *playerInfo = CEntityFactory::getSingletonPtr()->getInfo(type);

		// Asignar nombre
		playerInfo->setName(name);

		// Creamos la entidad y modificamos el resto de parametros que necesitamos
		CEntity* playerCreated = CEntityFactory::getSingletonPtr()->createEntity(playerInfo, this, false);
		playerCreated->setPosition( playerCreated->getPosition());
		return playerCreated;
	}

	//--------------------------------------------------------
	//--------------------------------------------------------

	CEntity* CMap::createLocalPlayer(const std::string &name, const std::string &type , TEntityID id){

		//cogemos el player necesario
		std::string temp = "Local"+type;
		Map::CEntity *playerInfo = CEntityFactory::getSingletonPtr()->getInfo("Local"+type);

		// Asignar nombre
		playerInfo->setName(name);

		// Creamos la entidad y modificamos el resto de parametros que necesitamos
		CEntity* playerCreated = CEntityFactory::getSingletonPtr()->createEntityById(playerInfo, this, id);
		playerCreated->setPosition( playerCreated->getPosition());

		// Configuramos el jugador como local si lo es
		playerCreated->setIsPlayer(true);
		getEntityByID(playerCreated->getEntityID())->setIsPlayer(true);

		//como es el local, le decimos al juego que esta entidad es el player principal para
		//que le pueda atachar la camara
		CServer::getSingletonPtr()->setPlayer(playerCreated);

		return playerCreated;
	}
	
	//--------------------------------------------------------
	//--------------------------------------------------------

	CEntity* CMap::createPlayer(const std::string &name, const std::string &type, TEntityID id){

		//cogemos el player necesario
		Map::CEntity *playerInfo = CEntityFactory::getSingletonPtr()->getInfo(type);
		// Asignar nombre
		playerInfo->setName(name);

		// Creamos la entidad y modificamos el resto de parametros que necesitamos
		CEntity* playerCreated = CEntityFactory::getSingletonPtr()->createEntityById(playerInfo, this, id);
		playerCreated->setPosition( playerCreated->getPosition());
		return playerCreated;
	}

	//--------------------------------------------------------
	//--------------------------------------------------------


	void CMap::deleteDeferredEntity(CEntity* entity){
		_deleteEntities.push_back(entity);
	}

	void CMap::entityTimeToLive(CEntity* entity, unsigned int msecs) {
		_entitiesWithTimeout.push_back( std::pair<CEntity*, unsigned int>(entity, msecs) );
	}

	void CMap::setFixedTimeStep(unsigned int stepSize) {
		_fixedTimeStep = stepSize;
	}

} // namespace Logic
