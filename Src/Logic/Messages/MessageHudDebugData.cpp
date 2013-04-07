#include "MessageHudDebugData.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHudDebugData);

	CMessageHudDebugData::CMessageHudDebugData() : CMessage(Message::HUD_DEBUG_DATA) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
	
	void CMessageHudDebugData::setKey(std::string key){
		_key = key;

	}//
	//----------------------------------------------------------

	void CMessageHudDebugData::setValue(std::string value){
		_value = value;

	}//
	//----------------------------------------------------------

	std::string CMessageHudDebugData::getKey(){
		return _key;

	}//
	//----------------------------------------------------------

	std::string CMessageHudDebugData::getValue(){
		return _value;

	}//
	//----------------------------------------------------------

		
	Net::CBuffer* CMessageHudDebugData::serialize() {
		assert(_tempBuffer == NULL);

		_tempBuffer = new Net::CBuffer(sizeof(int) + sizeof(_key) + sizeof(_value));
		_tempBuffer->serialize(std::string("CMessageHudDebugData"),true);

		_tempBuffer->serialize(_key, false);
		_tempBuffer->serialize(_value, false);
		
		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDebugData::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_key);
		buffer.deserialize(_value);
	}

};
