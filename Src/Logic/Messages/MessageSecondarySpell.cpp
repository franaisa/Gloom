#include "MessageSecondarySpell.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSecondarySpell);

	CMessageSecondarySpell::CMessageSecondarySpell() : CMessage(Message::SECONDARY_SPELL) {
		// Nada que hacer
	}

	CMessageSecondarySpell::CMessageSecondarySpell(bool spell) : _spell(spell), CMessage(Message::SECONDARY_SPELL) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageSecondarySpell::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageSecondarySpell"), true );
		buffer.serialize(_spell);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSecondarySpell::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_spell);
	}

};
