#include "DynamicParticleSystem.h"
#include "Graphics.h"

#include "Logic/Messages/MessageActivate.h"

#include "Graphics/Entity.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CDynamicParticleSystem);

	//______________________________________________________________________________

	CDynamicParticleSystem::CDynamicParticleSystem() : _graphicParent(NULL) { 
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

	void CDynamicParticleSystem::setOffset(const Vector3& offset) {
		this->_offset = offset;
	}

	//______________________________________________________________________________

	void CDynamicParticleSystem::setGraphicParent(Graphics::CEntity* parent) {
		this->_graphicParent = parent;
	}

	//______________________________________________________________________________

	void CDynamicParticleSystem::onStart() {	
		if(_graphicParent != NULL) {
			_particleSystem = new(nothrow) Graphics::PUParticle(_scriptName, _graphicParent);
			_particleSystem->setPosition(_offset);
		}
		else {
			_particleSystem = new(nothrow) Graphics::PUParticle(_scriptName);
			_particleSystem->setPosition( _entity->getPosition() + _offset );
			_particleSystem->setOrientation( _entity->getOrientation() );
		}

		_particleSystem->addObserver(this); // No es necesario hacer el remove
		if(_emitting)
			_particleSystem->start();
	} // onStart

	//______________________________________________________________________________

	void CDynamicParticleSystem::onTick(unsigned int msecs) {
		if(_graphicParent == NULL) {
			_particleSystem->setPosition( _entity->getPosition() + _offset );
			_particleSystem->setOrientation( _entity->getOrientation() );
		}
	}

} // namespace Logic


