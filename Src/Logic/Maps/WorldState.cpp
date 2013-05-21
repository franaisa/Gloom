#include "WorldState.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"

#include <cassert>

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
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Net::CBuffer CWorldState::serialize(){
		Net::CBuffer worldState;

		return worldState;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::deserialize(Net::CBuffer &worldState){
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWorldState::clearEntities(){
		_entities.clear();
	}

}//namespace Logic



