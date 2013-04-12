#include "MessageDamaged.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDamaged);

	CMessageDamaged::CMessageDamaged() : CMessage(Message::DAMAGED) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setDamage(float damage){
		_damage = damage;
	}//
	//----------------------------------------------------------

	float CMessageDamaged::getDamage(){
		return _damage;
	}//
	//----------------------------------------------------------

	CEntity* CMessageDamaged::getEnemy(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setEnemy(CEntity* entity){
		_entity=entity;
	}//
	//----------------------------------------------------------
	
	Net::CBuffer* CMessageDamaged::serialize() {
		assert(_tempBuffer == NULL);

		Logic::TEntityID id = _entity->getEntityID();

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_damage) + sizeof(id));
		_tempBuffer->serialize(std::string("CMessageDamaged"), true);
		_tempBuffer->serialize(_damage);
		_tempBuffer->serialize(id);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_damage);

		TEntityID id;
		// Por problemas con enumerados utilizamos directamente
		// el read en vez del deserialize
		buffer.read( &id, sizeof(id) );
		_entity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	}

};
