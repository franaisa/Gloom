#include "MessageSpellHungry.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSpellHungry);

	CMessageSpellHungry::CMessageSpellHungry() : CMessage(Message::SPELL_HUNGRY) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	CMessageSpellHungry::CMessageSpellHungry(float spellHungry) : CMessage(Message::SPELL_HUNGRY), _spellHungry(spellHungry) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageSpellHungry::setSpellHungry(float spellHungry){
		_spellHungry = spellHungry;
	}//
	//----------------------------------------------------------

	float CMessageSpellHungry::getSpellHungry(){
		return _spellHungry;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageSpellHungry::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(_spellHungry));
		buffer.serialize(std::string("CMessageSpellHungry"),true);
		buffer.serialize(_spellHungry);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSpellHungry::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spellHungry);
	}

};
