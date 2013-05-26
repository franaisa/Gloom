#include "MessageParticleVisibility.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageParticleVisibility);

	CMessageParticleVisibility::CMessageParticleVisibility() : CMessage(Message::PARTICLE_VISIBILITY) {
		// Nada que hacer
	} //
	//----------------------------------------------------------

	Net::CBuffer CMessageParticleVisibility::serialize() {
		Net::CBuffer buffer(sizeof(int) + sizeof(int) + sizeof(_visibility) );
		buffer.serialize(std::string("CMessageParticleVisibility"), true);
		buffer.serialize(std::string(_nameParticle), false);
		buffer.serialize(_visibility);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageParticleVisibility::deserialize(Net::CBuffer& buffer) {	
		buffer.deserialize(_nameParticle);
		buffer.deserialize(_visibility);
	}

};
