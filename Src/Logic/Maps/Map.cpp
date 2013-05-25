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
#include "Logic/Maps/WorldState.h"

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

		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		bool correct = true;

		// Activamos todas las entidades registradas en el mapa.
		for(; it != end; ++it)
			correct = it->second._entityPtr->activate() && correct;

		return correct;

	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate() {
		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		// Desactivamos todas las entidades activas registradas en el mapa.
		CEntity* entity;
		for(; it != end; ++it) {
			entity = it->second._entityPtr;
			if( entity->isActivated() ) {
				entity->deactivate();
			}
		}

		Graphics::CServer::getSingletonPtr()->setScene(0);

	} // getEntity

	//---------------------------------------------------------

	void CMap::wantsTick(CEntity* entity) {
		_entitiesWithTick.push_front(entity);

		auto it = _entityInfoTable.find( entity->getEntityID() );
		it->second._tickIterator = _entitiesWithTick.begin();
	}

	//---------------------------------------------------------

	void CMap::wantsFixedTick(CEntity* entity) {
		_entitiesWithFixedTick.push_front(entity);

		auto it = _entityInfoTable.find( entity->getEntityID() );
		it->second._fixedTickIterator = _entitiesWithFixedTick.begin();
	}

	//---------------------------------------------------------

	void CMap::start() {
		// Ejecutamos el start de todas nuestras entidades
		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		for(; it != end; ++it) {
			it->second._entityPtr->start();
		}
		Logic::CWorldState::getSingletonPtr()->updateEntities();
	}

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) {
		// Comprobamos los timers de las entidades que tienen
		// un tiempo de vida
		checkTimeouts(msecs);

		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();
		
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
		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		for(; it != end; ++it) {
			it->second._entityPtr->processComponentMessages();
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
				auto otherIt = _entityInfoTable.find( entity->getEntityID() );
				otherIt->second._tickIterator = _entitiesWithTick.end();
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
					auto otherIt = _entityInfoTable.find( entity->getEntityID() );
					otherIt->second._fixedTickIterator = _entitiesWithFixedTick.end();
					it = _entitiesWithFixedTick.erase(it);
					continue;
				}

				++it;
			}
		}
	}

	//--------------------------------------------------------

	void CMap::addEntity(CEntity *entity) {
		TEntityID entityId = entity->getEntityID();
		// Añadimos la entidad si no existia
		if( _entityInfoTable.find(entityId) == _entityInfoTable.end() ) {
			// Insertamos las entidades en la listas que vamos a usar para recorrer
			// y nos apuntamos el iterador (que nos hara falta mas tarde para los
			// borrados).
			_entitiesWithTick.push_front(entity);
			std::list<CEntity*>::const_iterator tickIt = _entitiesWithTick.begin();

			_entitiesWithFixedTick.push_front(entity);
			std::list<CEntity*>::const_iterator fixedTickIt = _entitiesWithFixedTick.begin();

			EntityInfo info;
			info._entityPtr = entity;
			info._tickIterator = tickIt;
			info._fixedTickIterator = fixedTickIt;

			_entityInfoTable.insert( pair<TEntityID, EntityInfo>(entityId, info) );
		}
	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity) {
		// Eliminamos la entidad si existe
		auto it = _entityInfoTable.find( entity->getEntityID() );
		if( it != _entityInfoTable.end() ) {
			EntityInfo info = it->second;

			if(info._entityPtr->isActivated())
				info._entityPtr->deactivate();

			info._entityPtr->_map = NULL;

			// Borramos el elemento de cada lista en la que este
			// presente
			if( info._tickIterator != _entitiesWithTick.end() ) 
				_entitiesWithTick.erase(info._tickIterator);
			if( info._fixedTickIterator != _entitiesWithFixedTick.end() ) 
				_entitiesWithFixedTick.erase(info._fixedTickIterator);

			_entityInfoTable.erase(it);
		}
	} // removeEntity

	//--------------------------------------------------------

	void CMap::destroyAllEntities() {
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		// Eliminamos todas las entidades.
		EntityInfo info;
		for(; it != end; ++it) {
			info = it->second;

			info._entityPtr->_map = NULL;

			if( info._entityPtr->isActivated() ) {
				info._entityPtr->deactivate();
			}

			delete info._entityPtr;
		}

		_entityInfoTable.clear();
	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID) {
		auto it = _entityInfoTable.find(entityID);
		return it != _entityInfoTable.end() ? it->second._entityPtr : NULL;
	} // getEntityByID

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start) {
		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start) {
			it = _entityInfoTable.find( start->getEntityID() );
			// si la entidad no existe devolvemos NULL.
			if(it == end) return NULL;
			
			++it;
		}
		else
			it = _entityInfoTable.begin();

		for(; it != end; ++it) {
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!it->second._entityPtr->getName().compare(name))
				return it->second._entityPtr;
		}
		
		// si no se encontró la entidad devolvemos NULL.
		return NULL;

	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start) {
		auto it = _entityInfoTable.begin();
		auto end = _entityInfoTable.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start) {
			it = _entityInfoTable.find( start->getEntityID() );
			// si la entidad no existe devolvemos NULL.
			if(it == end) return NULL;
			
			++it;
		}
		else
			it = _entityInfoTable.begin();

		for(; it != end; ++it) {
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!it->second._entityPtr->getType().compare(type))
				return it->second._entityPtr;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

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
		_entitiesToBeDeleted.push_back(entity);
	}

	void CMap::entityTimeToLive(CEntity* entity, unsigned int msecs) {
		_entitiesWithTimeout.push_back( std::pair<CEntity*, unsigned int>(entity, msecs) );
	}

	void CMap::setFixedTimeStep(unsigned int stepSize) {
		_fixedTimeStep = stepSize;
	}

} // namespace Logic
