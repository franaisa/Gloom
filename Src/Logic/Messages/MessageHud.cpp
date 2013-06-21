#include "MessageHud.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHud);

	CMessageHud::CMessageHud() : CMessage(Message::HUD) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageHud::setType( HudType type){
		_type = type;
	}//
	//----------------------------------------------------------

	CMessageHud::HudType CMessageHud::getType(){
		return _type;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageHud::serialize() {
		
		Net::CBuffer buffer(sizeof(int) + sizeof(_type));
		buffer.serialize(std::string("CMessageHud"),true);
		buffer.write(&_type,sizeof(_type));
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHud::deserialize(Net::CBuffer& buffer) {
		buffer.read(&_type, sizeof(HudType));
	}

};
