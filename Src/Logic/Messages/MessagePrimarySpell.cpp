#include "MessagePrimarySpell.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessagePrimarySpell);

	CMessagePrimarySpell::CMessagePrimarySpell() : CMessage(Message::PRIMARY_SPELL) {
		// Nada que hacer
	}

	CMessagePrimarySpell::CMessagePrimarySpell(bool spell) : _spell(spell), CMessage(Message::PRIMARY_SPELL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessagePrimarySpell::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessagePrimarySpell"), true );
		buffer.serialize(_spell);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessagePrimarySpell::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spell);
	}

};
