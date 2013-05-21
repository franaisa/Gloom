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

	void CMessageHudDebugData::setValue(float value){
		std::stringstream aux;
		aux << value;
		_value = aux.str();

	}//
	//----------------------------------------------------------

	void CMessageHudDebugData::setValue(Vector3 value){
		std::stringstream aux;
		aux << value;
		_value = aux.str();

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

		
	Net::CBuffer CMessageHudDebugData::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_key) + sizeof(_value));
		buffer.serialize(std::string("CMessageHudDebugData"),true);

		buffer.serialize(_key, false);
		buffer.serialize(_value, false);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageHudDebugData::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_key);
		buffer.deserialize(_value);
	}

};
