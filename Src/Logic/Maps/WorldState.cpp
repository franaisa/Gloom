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

	void CWorldState::deleteEntity(CEntity* entity){
		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if we don't have the entity changed, we do nothing
		if (entityFound== _entities.end())
			return;

		_entities.erase(entityFound);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::addChange(CEntity* entity, std::shared_ptr<CMessage> message){
		TEntityID id = entity->getEntityID();

		auto entityFound = _entities.find(id);

		//if we don't have the entity changed, we do nothing
		if (entityFound== _entities.end())
			return;

		unsigned int type = message->getMessageType();

		auto messageFound = entityFound->second.messages.find(type);

		if(messageFound!=entityFound->second.messages.end())
			entityFound->second.messages.erase(messageFound);

		TInfo newMessage(type,message);
		entityFound->second.messages.insert(newMessage);

		bool notified;
		for(int i = 0; i < _observers.size(); ++i) {
			// @deprecated
			// En el futuro esto sera una sola comprobacion de máscara
			// ahora mismo lo hago un poco a lo bestia
			notified = false;
			for(int k = 0; k < _observers[i].second.size() && !notified; ++k) {
				if(_observers[i].second[k] == type) {
					_observers[i].first->gameEventOcurred(message);
					notified = true;
				}
			}
		}
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
			worldState.serialize(entities->second.id);
			worldState.serialize(entities->second.entity->getType(), false);
			worldState.serialize(entities->second.entity->getName(), false);
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

			unsigned int messageSize;

			worldState.deserialize(messageSize);

			addEntity(entity);
			//deserialize entity messages
			for(int j = 0; j < messageSize ; ++j){
				int typeMessage;
				//read message type
				worldState.read(&typeMessage, sizeof(int));

				//deserialize message
				std::shared_ptr<CMessage> messageReceived( Logic::CMessageFactory::getSingletonPtr()->create(typeMessage) );
				messageReceived->deserialize(worldState);

				//send message to entity
				addChange(entity, messageReceived);
			}

			//if the entity is a player, we must add it to the scoreboard
			if(entityType == "Hound" || entityType == "Archangel" || entityType == "Shadow" || entityType == "Screamer"){
				if(!Logic::CScoreboard::getSingletonPtr()->getPlayer(entityName))
					Logic::CScoreboard::getSingletonPtr()->addPlayer(entityName, entity, entityType);
				else
					Logic::CScoreboard::getSingletonPtr()->changePlayerEntity(entityName, entity, entityType);
			}
		}

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



