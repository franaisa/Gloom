#include "MessageDecal.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageDecal);

	CMessageDecal::CMessageDecal() : CMessage(Message::DECAL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageDecal::serialize() {
		Net::CBuffer buffer( sizeof(int) + sizeof(_vPosition) + sizeof(_vTexture) + sizeof(_bRandomSize) );
		buffer.serialize( std::string("CMessageDecal"), true );
		buffer.serialize( _vPosition);
		buffer.serialize( std::string(_vTexture),false);
		buffer.serialize( _bRandomSize);
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageDecal::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_vPosition);
		buffer.deserialize(_vTexture);
		buffer.deserialize(_bRandomSize);
	}

};
