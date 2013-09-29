#include "MessageParticleStop.h"

#include "Audio/Server.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageParticleStop);

	//----------------------------------------------------------

	CMessageParticleStop::CMessageParticleStop() : CMessage(Message::PARTICLE_STOP) {
		// Nada que hacer
	}

	//----------------------------------------------------------
	
	CMessageParticleStop::~CMessageParticleStop() {
		// Nada que hacer
	}

	//----------------------------------------------------------

	Net::CBuffer CMessageParticleStop::serialize() {
		Net::CBuffer buffer;
		buffer.serialize( std::string("CMessageParticleStop"), true );

		return buffer;
	}//
	
	//----------------------------------------------------------

	void CMessageParticleStop::deserialize(Net::CBuffer& buffer) {
		// Nada que hacer
	}

};
