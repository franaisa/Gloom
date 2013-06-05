#include "MessageDecal.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDecal);

	CMessageDecal::CMessageDecal() : CMessage(Message::DECAL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageDecal::serialize() {
		Net::CBuffer buffer( sizeof(int) + sizeof(_vPosition));
		buffer.serialize( std::string("CMessageDecal"), true );
		buffer.serialize(_vPosition);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageDecal::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_vPosition);
	}

};
