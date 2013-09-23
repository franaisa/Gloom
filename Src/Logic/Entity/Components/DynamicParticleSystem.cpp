#include "DynamicParticleSystem.h"

#include "Logic/Messages/MessageActivate.h"

namespace Logic {
	
	IMP_FACTORY(CDynamicParticleSystem);

	//______________________________________________________________________________

	CDynamicParticleSystem::CDynamicParticleSystem() : _owner(NULL) { 
		// Nada que hacer
	}

	//______________________________________________________________________________

	CDynamicParticleSystem::~CDynamicParticleSystem() {
		// Nada que hacer
	}

	//______________________________________________________________________________

	bool CDynamicParticleSystem::accept(const std::shared_ptr<CMessage>& message) {
		return CParticleSystem::accept(message)					||
			   message->getMessageType() == Message::ACTIVATE;
	}
	
	//______________________________________________________________________________

	void CDynamicParticleSystem::process(const std::shared_ptr<CMessage>& message) {
		CParticleSystem::process(message);

		switch( message->getMessageType() ) {
			case Message::ACTIVATE: {
				bool activate = std::static_pointer_cast<CMessageActivate>(message)->getActivated();
				
				if(activate)
					_particleSystem->start();
				else
					_particleSystem->stop();

				break;
			}
		}
	}

	//______________________________________________________________________________
	
	void CDynamicParticleSystem::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

	//______________________________________________________________________________

	void CDynamicParticleSystem::setOffset(const Vector3& offset) {
		this->_offset = offset;
	}

	//______________________________________________________________________________

	void CDynamicParticleSystem::onTick(unsigned int msecs) {
		CEntity* followedEntity = (_owner != NULL) ? _owner : _entity;

		_particleSystem->setPosition( followedEntity->getPosition() + _offset );
		_particleSystem->setOrientation( followedEntity->getOrientation() );
	}

} // namespace Logic


