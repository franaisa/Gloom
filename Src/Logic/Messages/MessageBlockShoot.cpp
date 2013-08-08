#include "MessageBlockShoot.h"

#include "Logic/Entity/MessageFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageBlockShoot);

	CMessageBlockShoot::CMessageBlockShoot() : CMessage(Message::BLOCK_SHOOT),
											   _canShoot(true) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageBlockShoot::~CMessageBlockShoot() {
		// Nada que hacer
	}
	//----------------------------------------------------------

	bool CMessageBlockShoot::canShoot() {
		return _canShoot;
	}
	//----------------------------------------------------------
		
	void CMessageBlockShoot::canShoot(bool can) {
		_canShoot = can;
	}
	//----------------------------------------------------------
	
	Net::CBuffer CMessageBlockShoot::serialize() {
		Net::CBuffer buffer( sizeof(int) + sizeof(bool) );
		buffer.serialize(std::string("CMessageBlockShoot"), true);
		buffer.write( &_canShoot, sizeof(bool) );
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageBlockShoot::deserialize(Net::CBuffer& buffer) {
		buffer.read( &_canShoot, sizeof(bool) );
	}

};
