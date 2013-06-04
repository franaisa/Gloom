#include "MessageSpawnIsLive.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSpawnIsLive);

	CMessageSpawnIsLive::CMessageSpawnIsLive() : CMessage(Message::SPAWN_IS_LIVE) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageSpawnIsLive::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageSpawnIsLive"), true );

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSpawnIsLive::deserialize(Net::CBuffer& buffer) {
		// Nada que deserializar
	}

};
