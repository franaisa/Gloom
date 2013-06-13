#include "MessageAddSpell.h"

#include "Logic/Entity/MessageFactory.h"

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAddSpell);

	CMessageAddSpell::CMessageAddSpell() : CMessage(Message::ADD_SPELL) {
		// Nada que hacer
	}//
	//----------------------------------------------------------

	void CMessageAddSpell::setSpell( int spell){
		_spell = spell;
	}//
	//----------------------------------------------------------

	int CMessageAddSpell::getSpell(){
		return _spell;
	}//
	//----------------------------------------------------------
		
	Net::CBuffer CMessageAddSpell::serialize() {
		
		Net::CBuffer buffer(sizeof(int) + sizeof(_spell));
		buffer.serialize(std::string("CMessageAddSpell"),true);
		buffer.serialize(_spell);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAddSpell::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spell);
	}

};
