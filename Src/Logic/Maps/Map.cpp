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

#include <cassert>

#include <fstream>

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
				entity = entityFactory->createEntity((*it),map);
			}else{
				info->setAttribute((*it));
				info->setName((*it)->getName());
				entity = entityFactory->createEntity(info,map);
			}
			assert(entity && "No se pudo crear una entidad del mapa");
		}

		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	CMap::CMap(const std::string &name) : _numOfPlayers(0), 
									      _acumTime(0),
										  _fixedTimeStep(16) {
		_name = name;
		_scene = Graphics::CServer::getSingletonPtr()->createScene(name);

	} // CMap

	//--------------------------------------------------------

	CMap::~CMap()
	{
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_scene);

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate()
	{
		Graphics::CServer::getSingletonPtr()->setScene(_scene);

		TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();

		bool correct = true;

		// Activamos todas las entidades registradas en el mapa.
		for(; it != end; it++)
			correct = (*it).second->activate() && correct;

		return correct;

	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate() {
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();

		// Desactivamos todas las entidades activas registradas en el mapa.
		for(; it != end; it++)
			if((*it).second->isActivated())
				(*it).second->deactivate();

		Graphics::CServer::getSingletonPtr()->setScene(0);

	} // getEntity

	//---------------------------------------------------------

	void CMap::start() {
		// Ejecutamos el start de todas nuestras entidades
		TEntityMap::const_iterator it;
		for( it = _entityMap.begin(); it != _entityMap.end(); ++it )
			(*it).second->start();
	}

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) {
		// Comprobamos los timers de las entidades que tienen
		// un tiempo de vida
		checkTimeOuts(msecs);

		// @deprecated deberiamos solo ejecutar el tick y el fixedTick de las entidades
		// que lo hayan solicitado

		// Ejecutamos el tick de las entidades
		executeTick(msecs);
		// Ejecutamos el tick fijo de las entidades
		executeFixedTick(msecs);

	} // tick

	//--------------------------------------------------------

	void CMap::checkTimeOuts(unsigned int msecs) {
		// Si hay entidades con cronometros de autodestruccion
		// comprobamos el cronometro y si hay que destruirlas avisamos
		// a la factoria para que se encargue de ello en diferido.
		if( !_entitiesToBeDeleted.empty() ) {
			std::list< std::pair<CEntity*, unsigned int> >::iterator it = _entitiesToBeDeleted.begin();
			while( it != _entitiesToBeDeleted.end() ) {
				// Dado que estamos tratando con un unsigned int, comprobamos si la resta saldria menor
				// que 0, ya que si hacemos directamente la resta el numero pasa a ser el unsigned int 
				// mas alto y por lo tanto seria un fail.
				it->second = msecs >= it->second ? 0 : (it->second - msecs);

				if(it->second <= 0) {
					// Puesto por defecto a true pero deberia de ser apuntado cuando se llama 
					// al createEntityWithTimeOut y recuperarse al llegar a este caso
					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(it->first, true);
					it = _entitiesToBeDeleted.erase(it);
				}
				else {
					++it;
				}
			}
		}
	}

	//--------------------------------------------------------

	void CMap::executeTick(unsigned int msecs) {
		// Ejecutamos el tick de todas nuestras entidades
		std::set<CEntity*>::const_iterator it = _entitiesWithTick.begin();
		for(; it != _entitiesWithTick.end(); ++it) {
			(*it)->tick(msecs);
		}
	}

	//--------------------------------------------------------

	void CMap::executeFixedTick(unsigned int msecs) {
		_acumTime += msecs;
		
		std::set<CEntity*>::const_iterator it;
		
		// Ejecutamos el tick fijo el máximo número de steps posibles
		// teniendo en cuenta el time step dado.
		while(_acumTime >= _fixedTimeStep) {
			for(it = _entitiesWithFixedTick.begin(); it != _entitiesWithFixedTick.end(); ++it) {
				(*it)->fixedTick(_fixedTimeStep);
			}

			_acumTime -= _fixedTimeStep;
		}
	}

	//--------------------------------------------------------

	typedef std::pair<TEntityID,CEntity*> TEntityPair;

	void CMap::addEntity(CEntity *entity)
	{
		if(_entityMap.count(entity->getEntityID()) == 0)
		{
			TEntityPair elem(entity->getEntityID(),entity);
			_entityMap.insert(elem);
		}

	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity)
	{
		if(_entityMap.count(entity->getEntityID()) != 0)
		{
			if(entity->isActivated())
				entity->deactivate();
			entity->_map = 0;
			_entityMap.erase(entity->getEntityID());
		}

	} // removeEntity

	//--------------------------------------------------------

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		TEntityMap::const_iterator it, end;
		it = _entityMap.begin();
		end = _entityMap.end();

		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		while(it != end)
		{
			CEntity* entity = (*it).second;
			it++;
			entityFactory->deleteEntity(entity);
		}

		_entityMap.clear();

	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID)
	{
		if(_entityMap.count(entityID) == 0)
			return 0;
		return (*_entityMap.find(entityID)).second;

	} // getEntityByID

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
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
			if (!(*it).second->getName().compare(name))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
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

	} // getEntityByType

	//--------------------------------------------------------
	//--------------------------------------------------------
	
	CEntity* CMap::createPlayer(const std::string &name, const std::string &type)
	{

		//cogemos el player necesario
		Map::CEntity *playerInfo = CEntityFactory::getSingletonPtr()->getInfo(type);

		// Asignar nombre
		playerInfo->setName(name);

		// Creamos la entidad y modificamos el resto de parametros que necesitamos
		CEntity* playerCreated = CEntityFactory::getSingletonPtr()->createEntity(playerInfo, this);
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
		_entitiesToBeDeleted.push_back( std::pair<CEntity*, unsigned int>(entity, msecs) );
	}

	void CMap::setFixedTimeStep(unsigned int stepSize) {
		_fixedTimeStep = stepSize;
	}

	void CMap::wantsTick(CEntity* entity, bool tickeable) {
		if(tickeable) {
			// Anotar como tickeable
			_entitiesWithTick.insert(entity);
		}
		else {
			_entitiesWithTick.erase(entity);
		}
	}

	void CMap::wantsFixedTick(CEntity* entity, bool tickeable) {
		if(tickeable) {
			_entitiesWithFixedTick.insert(entity);
		}
		else {
			_entitiesWithFixedTick.erase(entity);
		}
	}

} // namespace Logic
