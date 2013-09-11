#include "MessageSecondarySkillActive.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageSecondarySkillActive);

	CMessageSecondarySkillActive::CMessageSecondarySkillActive() : CMessage(Message::SECONDARY_SKILL_ACTIVE) {
		// Nada que hacer
	}

	
	//----------------------------------------------------------

	Net::CBuffer CMessageSecondarySkillActive::serialize() {
		Net::CBuffer buffer( sizeof(int) );
		buffer.serialize( std::string("CMessageSecondarySkillActive"), true );
		buffer.serialize(_active);
		
		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageSecondarySkillActive::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_active);
	}

	//----------------------------------------------------------

	bool CMessageSecondarySkillActive::canActive() {
		return _active;
	}
	//----------------------------------------------------------
		
	void CMessageSecondarySkillActive::canActive(bool active) {
		_active = active;
	}
};
