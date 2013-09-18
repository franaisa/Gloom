#include "DynamicParticleSystem.h"

#include "Logic/Messages/MessageActivate.h"

namespace Logic {
	
	IMP_FACTORY(CDynamicParticleSystem);

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
	
	void CDynamicParticleSystem::onTick(unsigned int msecs) {
		_particleSystem->setPosition( _entity->getPosition() + _offset );
		_particleSystem->setOrientation( _entity->getOrientation() );
	}

} // namespace Logic


