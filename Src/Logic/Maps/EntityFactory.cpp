/**
@file EntityFactory.cpp

Contiene la implementación de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García.
@author Marco Antonio Gómez Martín.
@author Ruben Mulero Guerrero.
@author Francisco Aisa García.
@date Agosto, 2010
*/

#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/WorldState.h"
#include "Logic/Server.h"
#include "Logic/GameNetMsgManager.h"
#include "Net/Manager.h"
#include "Map.h"
#include "Map/MapParser.h"
#include "Map/MapEntity.h"
#include "../../Application/BaseApplication.h"
#include "../../Application/ApplicationState.h"

#include <iostream>
#include <fstream>
#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define BLUEPRINTS_FILE_PATH "./media/maps/"

/**
Sobrecargamos el operador >> para la lectura de blueprints.
Cada línea equivaldrá a una entidad donde la primera palabra es el tipo
y las siguientes son los componentes que tiene.

@param is Flujo de entrada.
@param blueprint Tipo de dato blueprint que va a ser leido.
*/
std::istream& operator>>(std::istream& is, Logic::CEntityFactory::TBluePrint& blueprint) {
	is >> blueprint.type;
	std::string aux;
	getline(is,aux,'\n');
	std::istringstream components(aux);

	while( !components.eof() ) {
		aux.clear();
		components >> aux;
		if( !aux.empty() )
			blueprint.components.push_back(aux);
	}

	return is;
}

namespace Logic {

	CEntityFactory *CEntityFactory::_instance = 0;
	
	//________________________________________________________________________

	CEntityFactory::CEntityFactory() : _dynamicCreation(false), _idDispatcher(0) {
		_instance = this;
	} // CEntityFactory
	
	//________________________________________________________________________

	CEntityFactory::~CEntityFactory() {
		_instance = 0;

	} // ~CEntityFactory
	
	//________________________________________________________________________

	bool CEntityFactory::Init() {
		assert(!_instance && "Segunda inicialización de Logic::CEntityFactory no permitida!");
		new CEntityFactory();

		if ( !_instance->open() ) {
			Release();
			return false;
		}

		return true;

	} // Init
	
	//________________________________________________________________________

	void CEntityFactory::Release() {
		assert(_instance && "Logic::CEntityFactory no está inicializado!");

		if(_instance) {
			_instance->close();
			delete _instance;
		}
	} // Release

	//________________________________________________________________________

	CEntityFactory* CEntityFactory::getSingletonPtr() { 
		return _instance; 
	}

	//________________________________________________________________________

	bool CEntityFactory::open() {
		return true;

	} // open

	//________________________________________________________________________

	void CEntityFactory::close() {
		unloadBluePrints();

	} // close
	
	//________________________________________________________________________

	typedef std::pair<std::string,CEntityFactory::TBluePrint> TStringBluePrintPair;

	bool CEntityFactory::loadBluePrints(const std::string &filename) {
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(BLUEPRINTS_FILE_PATH);
		completePath = completePath + filename;
		
		// Abrimos el fichero
		std::ifstream in(completePath.c_str());        
		if(!in)
			return false;

		while( !in.eof() ) {
			// Se lee un TBluePrint del fichero
			TBluePrint b;
			in >> b;
			// Si no era una línea en blanco
			if( !b.type.empty() ) {
				// Si el tipo ya estaba definido lo eliminamos.
				if(_bluePrints.count(b.type))
					_bluePrints.erase(b.type);
				// Añadimos a la tabla
				TStringBluePrintPair elem(b.type,b);
				_bluePrints.insert(elem);
			}
		}
		std::cout << "bluelprints terminado" << std::endl;
		return true;
	} // loadBluePrints

	//________________________________________________________________________

	typedef std::pair<std::string,Map::CEntity*> archetype;

	bool CEntityFactory::loadArchetypes(const std::string &filename){
		std::string completePath(BLUEPRINTS_FILE_PATH);
		completePath = completePath + filename;

		// Parseamos el fichero
		if( !Map::CMapParser::getSingletonPtr()->parseFile(completePath) ) {
			assert(!"No se ha podido parsear el mapa.");
			return false;
		}

		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		Map::CMapParser::TEntityList::iterator it, end;
		it = entityList.begin();
		end = entityList.end();
		
		// Creamos los arquetipos de todo lo que ha leido el parser
		for(; it != end; ++it) {
			Map::CEntity  *clone = (*it)->clone();
			archetype elem((*it)->getType(), clone);
				_archetypes.insert(elem);
		}

		Map::CMapParser::getSingletonPtr()->releaseEntityList();
		std::cout << "archetypes terminado" << std::endl;
		return true;
	} // loadArchetypes
	
	//________________________________________________________________________

	void CEntityFactory::unloadBluePrints() {
		_bluePrints.clear();
	} // unloadBluePrints
	
	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type) {
		TBluePrintMap::const_iterator it;
		it = _bluePrints.find(type);

		// si el tipo se encuentra registrado.
		if ( it != _bluePrints.end() ) {
			CEntity* ent = new CEntity( _idDispatcher->getNextId() );
			std::list<std::string>::const_iterator itc;
			
			// Añadimos todos sus componentes.
			IComponent* comp;
			for(itc = (*it).second.components.begin(); itc !=(*it).second.components.end(); ++itc) {
				if( CComponentFactory::getSingletonPtr()->has(*itc) ) {
					comp = CComponentFactory::getSingletonPtr()->create(*itc);
				}
				else {
					std::cerr << *itc << std::endl;;
					assert(!"Nombre erroneo de un componente, Mira a ver si están todos bien escritos en el fichero de blueprints");
					delete ent;
					return 0;
				}

				if(comp)
					ent->addComponent(comp, *itc);
			}
			return ent;
		}

		return 0;
	} // assembleEntity


	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type, const TEntityID id) {
		TBluePrintMap::const_iterator it;
		it = _bluePrints.find(type);

		// si el tipo se encuentra registrado.
		if ( it != _bluePrints.end() ) {
			CEntity* ent = new CEntity(id);
			std::list<std::string>::const_iterator itc;
			
			// Añadimos todos sus componentes.
			IComponent* comp;
			for(itc = (*it).second.components.begin(); itc !=(*it).second.components.end(); ++itc) {
				if( CComponentFactory::getSingletonPtr()->has(*itc) ) {
					comp = CComponentFactory::getSingletonPtr()->create((*itc));
				}
				else {
					assert(!"Nombre erroneo de un componente, Mira a ver si están todos bien escritos en el fichero de blueprints");
					delete ent;
					return 0;
				}

				if(comp)
					ent->addComponent(comp, *itc);
			}
			return ent;
		}

		return 0;
	} // assembleEntity
	
	//________________________________________________________________________

	Logic::CEntity* CEntityFactory::initEntity(Logic::CEntity* entity, Map::CEntity* entityInfo, CMap *map, bool replicate, Map::CEntity* customInfoForClient) {
		if(!entity) return NULL;

		if ( _dynamicCreation ? entity->dynamicSpawn(map, entityInfo) : entity->spawn(map, entityInfo) ) {
			// Añadimos la nueva entidad en el mapa antes de inicializarla.
			map->addEntity(entity);
			if(_dynamicCreation)
				CWorldState::getSingletonPtr()->addEntity(entity);
			if(replicate) {
				Logic::CGameNetMsgManager::getSingletonPtr()->sendCreateEntity( entity->getEntityID(), customInfoForClient );
			}

			return entity;
		} 
		else {
			delete entity;
			return NULL;
		}
	}

	//________________________________________________________________________

	Logic::CEntity* CEntityFactory::initEntity(Logic::CEntity* entity, Map::CEntity* entityInfo, Logic::CMap *map, const Vector3& position, const Quaternion& orientation, bool replicate, Map::CEntity* customInfoForClient) {
		if(!entity) return NULL;

		// Seteamos la posición de la entidad a ser creada
		entity->setPosition(position);
		entity->setOrientation(orientation);
		if( _dynamicCreation ? entity->dynamicSpawn(map, entityInfo) : entity->spawn(map, entityInfo) ) {
			// Si la entidad ha sido inicializada con éxito la añadimos al mapa.
			map->addEntity(entity);

			if(replicate) {
				Logic::CGameNetMsgManager::getSingletonPtr()->sendCreateEntity( entity->getEntityID(), customInfoForClient );
			}

			return entity;
		} 
		else {
			delete entity;
			return NULL;
		}
	}

	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::createEntity(Map::CEntity *entityInfo, Logic::CMap *map, bool replicate) {
		return initEntity(assembleEntity( entityInfo->getType() ), entityInfo, map, replicate);
	} // createEntity

	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::createEntity(Map::CEntity *entityInfo, Logic::CMap *map, const Vector3& position, const Quaternion& orientation, bool replicate) {
		return initEntity(assembleEntity( entityInfo->getType() ), entityInfo, map, position, orientation, replicate);
	} // createEntity

	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::createEntityById(Map::CEntity *entityInfo, Logic::CMap *map, TEntityID id, bool replicate) {
		return initEntity(assembleEntity(entityInfo->getType(), id), entityInfo, map, replicate);
	} // createEntity

	//________________________________________________________________________

	Logic::CEntity *CEntityFactory::createEntityById(Map::CEntity *entityInfo, Logic::CMap *map, TEntityID id, const Vector3& position, const Quaternion& orientation, bool replicate) {
		return initEntity(assembleEntity( entityInfo->getType(), id ), entityInfo, map, position, orientation, replicate);
	} // createEntity

	//________________________________________________________________________

	CEntity* CEntityFactory::createCustomClientEntity(Map::CEntity *entityInfo, Map::CEntity* customClientInfo, Logic::CMap *map, const Vector3& position, const Quaternion& orientation) {
		return initEntity( assembleEntity( entityInfo->getType() ), entityInfo, map, position, orientation, true, customClientInfo);
	}

	//________________________________________________________________________

	//Al metodo le falta un control de si quieres que el server envie o no a los clientes el borrado
	Logic::CEntity* CEntityFactory::createEntityWithTimeOut(Map::CEntity *entityInfo, CMap *map, unsigned int msecs, bool replicate) {
		CEntity* createdEntity = createEntity(entityInfo, map, replicate);
		deferredDeleteEntity(createdEntity, msecs);
		return createdEntity;
	}

	//________________________________________________________________________

	void CEntityFactory::deleteEntity(Logic::CEntity *entity, bool toClients) {
		assert(entity);
		// Si la entidad estaba activada se desactiva al sacarla
		// del mapa.
		entity->getMap()->removeEntity(entity);
		CWorldState::getSingletonPtr()->deleteEntity(entity);
		//Comprobamos si debe enviarse a los clientes, porque hay casos en los que no deberia
		if( Net::CManager::getSingletonPtr()->imServer() && toClients )
			Logic::CGameNetMsgManager::getSingletonPtr()->sendDestroyEntity( entity->getEntityID() );

		delete entity;

	} // deleteEntity
	
	//________________________________________________________________________

	void CEntityFactory::deferredDeleteEntity(Logic::CEntity *entity, bool toClients) {
		assert(entity);
		_pendingEntities.insert(entity);

		//la borramos del worldstate para evitar problemas y crashes
		CWorldState::getSingletonPtr()->deleteEntity(entity);

		//Comprobamos si debe enviarse a los clientes, porque hay casos en los que no deberia
		if( Net::CManager::getSingletonPtr()->imServer() && toClients )
			Logic::CGameNetMsgManager::getSingletonPtr()->sendDestroyEntity( entity->getEntityID() );
	} // deferredDeleteEntity

	//________________________________________________________________________

	void CEntityFactory::deferredDeleteEntity(CEntity *entity, unsigned int msecs) {
		assert(entity);
		Logic::CServer::getSingletonPtr()->getMap()->entityTimeToLive(entity, msecs);
	}
	
	//________________________________________________________________________

	void CEntityFactory::deleteDefferedEntities() {
		TEntitySet::const_iterator it(_pendingEntities.begin());
		TEntitySet::const_iterator end(_pendingEntities.end());
		
		while(it != end) {
			CEntity *entity = *it;
			++it;
			deleteEntity(entity);
		}

		if ( !_pendingEntities.empty() )
			_pendingEntities.clear();
	} // deleteDefferedObjects

	//________________________________________________________________________

	// @deprecated Deberia devolver un puntero constante para asegurarnos de que
	// nadie nos va a hacer la jugada fuera
	Map::CEntity* CEntityFactory::getInfo(std::string type) {
		std::map<std::string,Map::CEntity *>::const_iterator it = _archetypes.find(type);

		if( it!=_archetypes.end() ){
			return (*it).second;
		}

		return NULL;
	}

} // namespace Logic
