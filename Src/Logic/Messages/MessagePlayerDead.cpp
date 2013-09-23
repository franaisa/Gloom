#include "MessagePlayerDead.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePlayerDead);

	
	CMessagePlayerDead::CMessagePlayerDead() : CMessage(Message::PLAYER_DEAD),
											   _killer(NULL) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessagePlayerDead::serialize() {
		Net::CBuffer buffer( sizeof(int) * 2);
		buffer.serialize(std::string("CMessagePlayerDead"), true);

		int killerId = _killer == NULL ? -1 : _killer->getEntityID();
		buffer.write( &killerId, sizeof(killerId) );

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessagePlayerDead::deserialize(Net::CBuffer& buffer) {
		int killerId;

		buffer.read( &killerId, sizeof(killerId) );
		
		if(killerId == -1)
			_killer = NULL;
		else
			_killer = CServer::getSingletonPtr()->getMap()->getEntityByID(killerId);
	}

};
