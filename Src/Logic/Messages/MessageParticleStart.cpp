#include "MessageParticleStart.h"

#include "Audio/Server.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageParticleStart);

	//----------------------------------------------------------

	CMessageParticleStart::CMessageParticleStart() : CMessage(Message::PARTICLE_START) {
		// Nada que hacer
	}

	//----------------------------------------------------------
	
	CMessageParticleStart::~CMessageParticleStart() {
		// Nada que hacer
	}

	//----------------------------------------------------------

	Net::CBuffer CMessageParticleStart::serialize() {
		Net::CBuffer buffer;
		buffer.serialize( std::string("CMessageParticleStart"), true );

		return buffer;
	}//
	
	//----------------------------------------------------------

	void CMessageParticleStart::deserialize(Net::CBuffer& buffer) {
		// Nada que hacer
	}

};
