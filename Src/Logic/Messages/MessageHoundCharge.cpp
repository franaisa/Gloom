#include "MessageHoundCharge.h"

#include "Audio/Server.h"
#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageHoundCharge);

	//----------------------------------------------------------

	CMessageHoundCharge::CMessageHoundCharge() : CMessage(Message::HOUND_CHARGE), _isActive(true) {
		// Nada que hacer
	}

	//----------------------------------------------------------
	
	CMessageHoundCharge::~CMessageHoundCharge() {
		// Nada que hacer
	}

	//----------------------------------------------------------

	Net::CBuffer CMessageHoundCharge::serialize() {
		Net::CBuffer buffer;
		buffer.serialize( std::string("CMessageHoundCharge"), true );

		buffer.write( &_isActive, sizeof(_isActive) );
		// El resto de los atributos nos interesan solo si se
		// activa la habilidad
		if(_isActive) {
			buffer.write( &_filterMask, sizeof(_filterMask) );
			buffer.write( &_force, sizeof(_force) );
		}

		return buffer;
	}//
	
	//----------------------------------------------------------

	void CMessageHoundCharge::deserialize(Net::CBuffer& buffer) {
		buffer.read( &_isActive, sizeof(_isActive) );

		// Solo si la habilidad esta activa se mandan el resto 
		// de los atributos
		if(_isActive) {
			buffer.read( &_filterMask, sizeof(_filterMask) );
			buffer.read( &_force, sizeof(_force) );
		}	
	}

};
