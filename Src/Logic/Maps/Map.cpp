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
		
	CMap* CMap::createMapFromFile(const std::string &filename) {
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(MAP_FILE_PATH);
		completePath = completePath + filename;
		// Parseamos el fichero
		if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath)) {
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
		for(; it != end; it++) {
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

	//---------------------------------------------------------

	void CMap::wantsFixedTick(CEntity* entity) {
		_entitiesWithFixedTick.push_back(entity);
	}

	//---------------------------------------------------------

	void CMap::start() {
		// Ejecutamos el start de todas nuestras entidades
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
		doTick(msecs);

		// Ejecutamos el fixed tick de las entidades
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
		// Insertamos las entidades en la listas que vamos a usar para recorrer
		// y nos apuntamos el iterador (que nos hara falta mas tarde para los
		// borrados).
		_entityList.push_front(entity);
		std::list<CEntity*>::const_iterator listIt = _entityList.begin();

		_entitiesWithTick.push_front(entity);
		std::list<CEntity*>::const_iterator tickIt = _entitiesWithTick.begin();

		_entitiesWithFixedTick.push_front(entity);
		std::list<CEntity*>::const_iterator fixedTickIt = _entitiesWithFixedTick.begin();

		TEntityID entityId = entity->getEntityID();
		// Añadimos la entidad si no existia
		if( _entityIdLookupTable.find(entityId) == _entityIdLookupTable.end() ) {
			EntityInfo* info = new(std::nothrow) EntityInfo;
			assert(info != NULL && "Error: En la reserva de memoria");

			info->_entityPtr = entity;
			info->_processIterator = listIt;
			info->_tickIterator = tickIt;
			info->_fixedTickIterator = fixedTickIt;
			
			_entityIdLookupTable[entityId] = info;
			// @todo añadir a las listas que toque
			//_entityNameLookupTable[info->_entityPtr->getName()] = info;
			//_entityTypeLookupTable[info->_entityPtr->getType()] = info;
		}
	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity) {
		// Eliminamos la entidad si existe
		std::unordered_map<TEntityID, EntityInfo*>::const_iterator it = _entityIdLookupTable.find( entity->getEntityID() );
		if( it != _entityIdLookupTable.end() ) {
			EntityInfo* info = it->second;

			if(info->_entityPtr->isActivated())
				info->_entityPtr->deactivate();

			info->_entityPtr->_map = NULL;
			
			// Borramos el elemento de cada lista en la que este
			// presente
			if( info->_processIterator != _entityList.end() ) 
				_entityList.erase(info->_processIterator);
			if( info->_tickIterator != _entitiesWithTick.end() ) 
				_entitiesWithTick.erase(info->_tickIterator);
			if( info->_fixedTickIterator != _entitiesWithFixedTick.end() ) 
				_entitiesWithFixedTick.erase(info->_fixedTickIterator);

			_entityIdLookupTable.erase(it);
			//@todo borrar en la tabla de nombre y tipo

			// Borramos el puntero a la estructura que contenia
			// la informacion relativa a la estructura
			delete info;
		}
	} // removeEntity

	//--------------------------------------------------------

	// @deprecated WHAT THE FUCK? hay que decirle a la factoria que llame a un metodo nuestro
	// para borrar. Pero es que estamos tontos ya o q? encima al ejecutarlo hay que hacer una
	// busqueda. Esto es ineficiencia por deporte...
	void CMap::destroyAllEntities() {
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		std::unordered_map<TEntityID, EntityInfo*>::const_iterator it = _entityIdLookupTable.begin();
		std::unordered_map<TEntityID, EntityInfo*>::const_iterator end = _entityIdLookupTable.end();

		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		for(; it != end; ++it) {	
			entityFactory->deleteEntity(it->second->_entityPtr);
		}
	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID) {
		std::unordered_map<TEntityID, EntityInfo*>::const_iterator it = _entityIdLookupTable.find(entityID);
		if( it != _entityIdLookupTable.end() )
			return it->second->_entityPtr;
		
		return NULL;
	} // getEntityByID

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name) {
		std::unordered_map<std::string, EntityInfo*>::const_iterator it = _entityNameLookupTable.find(name);
		if( it != _entityNameLookupTable.end() )
			return it->second->_entityPtr;

		return NULL;
	} // getEntityByName

	//--------------------------------------------------------

	std::list<CEntity*> CMap::getEntitiesByType(const std::string &type) {
		std::unordered_multimap<std::string, EntityInfo*>::const_iterator it;
		std::list<CEntity*> entities;

		auto range = _entityTypeLookupTable.equal_range(type);
		for(it = range.first; it != range.second; ++it) {
			entities.push_back(it->second->_entityPtr);
		}

		return entities;
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
