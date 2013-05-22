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
	
	Net::CBuffer CMessageDamaged::serialize() {
		Logic::TEntityID id = _entity->getEntityID();

		Net::CBuffer buffer(sizeof(int) + sizeof(_damage) + sizeof(id));
		buffer.serialize(std::string("CMessageDamaged"), true);
		buffer.serialize(_damage);
		buffer.serialize(id);
		
		return buffer;
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
