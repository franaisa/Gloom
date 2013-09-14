//---------------------------------------------------------------------------
// WorldState.cpp
//---------------------------------------------------------------------------

/**
@file WorldState.cpp

Contiene la implementación del estado del mundo.

@author Rubén Mulero Guerrero
@date May, 2013
*/

#include "WorldState.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Scoreboard.h"
#include "Logic/Entity/Entity.h"
#include "Logic/GameNetPlayersManager.h"

#include "Map/MapEntity.h"

#include <cassert>

using namespace std;

namespace Logic {

	CWorldState* CWorldState::_instance = 0;

	CWorldState::CWorldState(){
		_instance = this;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CWorldState::Init(){
		assert(!_instance && "Segunda inicialización de Logic::CWorldState no permitida!");

		new CWorldState();

		if (!_instance->open())
		{
			Release();
			return false;
		}
		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::Release()
	{
		assert(_instance && "Logic::CWorldState no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}
	} // Release

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CWorldState::open(){
		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::close(){
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::addObserver(IObserver* listener, const vector<TMessageType>& eventsMask) {
		_observers.push_back( pair<IObserver*, vector<TMessageType> >(listener, eventsMask) );
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::removeObserver(IObserver* listener) {
		if( _observers.empty() ) return;

		// Debido a que normalmente tendremos un observador registrado
		// nos podemos permitir el lujo de hacer borrados secuenciales
		for(auto it = _observers.begin(); it != _observers.end(); ++it) {
			if(it->first == listener) {
				_observers.erase(it);
				break;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::addEntity(CEntity* entity){
		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if the entity was previously added, we don't need to add it again
		if (entityFound!= _entities.end())
			return;

		EntityInfo info;
		info.id = entity->getEntityID();
		info.entity = entity;

		TEntityInfo newEntity(id,info);

		_entities.insert(newEntity);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::deleteEntity(CEntity* entity) {
		for(auto it = _observers.begin(); it != _observers.end(); ++it)
			it->first->entityDestroyed(entity);

		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if we don't have the entity changed, we do nothing
		if (entityFound== _entities.end())
			return;

		_entities.erase(entityFound);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::addChange(CEntity* entity, std::shared_ptr<CMessage> message){
		if(message->getMessageType() == Message::PLAYER_DEAD)
			cout << entity->getName() << " ha palmado - WorldState" << endl;

		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if we don't have the entity changed, we do nothing
		if (entityFound== _entities.end())
			return;

		// Si el mensaje ya esta anotado lo actualizamos, si no,
		// lo registramos
		unsigned int type = message->getMessageType();
		entityFound->second.messages[type] = message;

		bool notified;
		for(int i = 0; i < _observers.size(); ++i) {
			// @deprecated
			// En el futuro esto sera una sola comprobacion de máscara
			// ahora mismo lo hago un poco a lo bestia
			notified = false;
			for(int k = 0; k < _observers[i].second.size() && !notified; ++k) {
				if(_observers[i].second[k] == type) {
					_observers[i].first->gameEventOcurred(entity, message);
					notified = true;
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::deleteChange(CEntity* entity, unsigned int messageType){
		if(messageType == Message::PLAYER_DEAD)
			cout << entity->getName() << " ha resucitado - WorldState" << endl;

		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if we don't have the entity changed, we do nothing
		if (entityFound == _entities.end())
			return;

		auto msgIt = entityFound->second.messages.find(messageType);
 
		if(msgIt!=entityFound->second.messages.end())
			entityFound->second.messages.erase(msgIt);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Net::CBuffer CWorldState::serialize(){
		Net::CBuffer worldState;

		//first of all, serialize the length of the struct
		unsigned int tableSize = _entities.size();
		worldState.serialize(tableSize);
		auto entities = _entities.begin();
		auto end = _entities.end();

		//serialize all entities
		for(;entities!=end;++entities){
			std::string entityType = entities->second.entity->getType();
			
			worldState.serialize(entities->second.id);
			worldState.serialize(entityType, false);
			worldState.serialize(entities->second.entity->getName(), false);

			// Si se trata de un player, serializamos el equipo al que pertenece y su id de red
			if(entityType == "Hound" || entityType == "Archangel" || entityType == "Shadow" || entityType == "Screamer") {
				Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
				Logic::CPlayerInfo playerInfo = playersMgr->getPlayerByEntityId(entities->second.id);
				Net::NetID netId = playerInfo.getNetId();
				Logic::TeamFaction::Enum team = playerInfo.getTeam();
				int frags = playerInfo.getFrags();
				int deaths = playerInfo.getDeaths();

				worldState.write( &netId, sizeof(netId) );
				worldState.write( &team, sizeof(team) );
				worldState.write( &frags, sizeof(frags) );
				worldState.write( &deaths, sizeof(deaths) );
			}

			if(entityType == "Spectator") {
				Logic::CPlayerInfo playerInfo = Logic::CGameNetPlayersManager::getSingletonPtr()->getPlayerByEntityId(entities->second.id);
				Net::NetID netId = playerInfo.getNetId();

				worldState.write( &netId, sizeof(netId) );
			}

			if(entities->second.messages.empty()){
				worldState.serialize(0);
				continue;
			}

			//serialize entity messages
			worldState.serialize(entities->second.messages.size());
			auto messages = entities->second.messages.begin();
			auto mEnd = entities->second.messages.end();
			for(;messages!=mEnd;++messages){
				Net::CBuffer bufferAux = messages->second->serialize(); // @deprecated (creo que esto no lo puedes hacer asi)
				worldState.write(bufferAux.getbuffer(), bufferAux.getSize());
			}
		}

		// @deprecated, aqui va la deserializacion del playersmanager siguiendo la 
		// estructura del scoreboard
		Net::CBuffer scoreboard = Logic::CScoreboard::getSingletonPtr()->serialize();
		worldState.write(scoreboard.getbuffer(), scoreboard.getSize());
		return worldState;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::deserialize(Net::CBuffer &worldState){

		//first of all, deserialize the length of the struct
		unsigned int tableSize;

		worldState.deserialize(tableSize);

		//serialize all entities
		for(int i = 0; i < tableSize ; ++i){
			TEntityID id;
			worldState.deserialize(id);
			std::string entityType, entityName;
			worldState.deserialize(entityType);
			worldState.deserialize(entityName);
			//get the Entity we are updating
			CEntity* entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);

			if(!entity){
				Map::CEntity* info = Logic::CEntityFactory::getSingletonPtr()->getInfo(entityType);
				info->setName(entityName);
				entity = Logic::CEntityFactory::getSingletonPtr()->createEntityById(info,Logic::CServer::getSingletonPtr()->getMap(), id);
			}
			else {
				std::cerr << "Warning: Habia una entidad con el id que se ha pasado, se va a liar parda!!!" << std::endl;
			}

			//if the entity is a player, we must add it to the scoreboard and read its team faction
			if(entityType == "Hound" || entityType == "Archangel" || entityType == "Shadow" || entityType == "Screamer") {
				Net::NetID netId;
				Logic::TeamFaction::Enum team;
				int frags, deaths;

				worldState.read( &netId, sizeof(netId) );
				worldState.read( &team, sizeof(team) );
				worldState.read( &frags, sizeof(frags) );
				worldState.read( &deaths, sizeof(deaths) );

				Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
				playersMgr->addPlayer(netId, entityName);
				playersMgr->setEntityID(netId, id);
				playersMgr->setPlayerTeam(netId, team);
				playersMgr->setPlayerState(netId, true);
				playersMgr->setFrags(netId, frags);
				playersMgr->setDeaths(netId, deaths);

				if(!Logic::CScoreboard::getSingletonPtr()->getPlayer(entityName))
					Logic::CScoreboard::getSingletonPtr()->addPlayer(entityName, entity, entityType, team);
				else
					Logic::CScoreboard::getSingletonPtr()->changePlayerEntity(entityName, entity, entityType);
			}

			if(entityType == "Spectator") {
				Net::NetID netId;
				worldState.read( &netId, sizeof(netId) );

				Logic::CGameNetPlayersManager* playersMgr = Logic::CGameNetPlayersManager::getSingletonPtr();
				playersMgr->addPlayer(netId, entityName);
				playersMgr->setEntityID(netId, id);
				playersMgr->setPlayerTeam(netId, Logic::TeamFaction::eNONE);
				playersMgr->setPlayerState(netId, false);
				playersMgr->setFrags(netId, 0);
				playersMgr->setDeaths(netId, 0);

				continue;
			}

			unsigned int messageSize;

			worldState.deserialize(messageSize);

			addEntity(entity);
			//deserialize entity messages
			for(int j = 0; j < messageSize ; ++j){
				int typeMessage;
				//read message type
				worldState.read( &typeMessage, sizeof(int) );

				//deserialize message
				std::shared_ptr<CMessage> messageReceived( Logic::CMessageFactory::getSingletonPtr()->create(typeMessage) );
				messageReceived->deserialize(worldState);

				//send message to entity
				addChange(entity, messageReceived);
			}
		}

		//updating the scoreboard
		Logic::CScoreboard::getSingletonPtr()->deserialize(worldState);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::clearEntities(){
		_entities.clear();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::updateEntities(){
		auto entities = _entities.begin();
		auto end = _entities.end();

		//serialize all entities
		for(;entities!=end;++entities){
			if(entities->second.messages.empty()){
				continue;
			}

			//serialize entity messages
			auto messages = entities->second.messages.begin();
			auto mEnd = entities->second.messages.end();
			for(;messages!=mEnd;++messages){
				entities->second.entity->emitMessage(messages->second);
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////


}//namespace Logic



