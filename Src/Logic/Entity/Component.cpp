/**
@file Components/Component.cpp

Contiene la implementación de la clase base de los componentes.
 
@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/

#include "Component.h"
#include "Logic/Server.h"

namespace Logic 
{
	IComponent::IComponent() : _entity(0), 
							   _isActivated(true),
							   _state(ComponentState::eAWAKE),
							   _tickMode(TickMode::eBOTH) {

		Logic::CServer::getSingletonPtr()->COMPONENT_CONSTRUCTOR_COUNTER += 1;
	}

	//---------------------------------------------------------

	IComponent::~IComponent() {
		Logic::CServer::getSingletonPtr()->COMPONENT_DESTRUCTOR_COUNTER += 1;
	}

	//---------------------------------------------------------

	bool IComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		_entity = entity;
		return true;

	} // spawn

	//---------------------------------------------------------
	
	void IComponent::activate() {
		if(_isActivated) return;
		
		// Por defecto no forzamos a que se arranque onStart
		// queda abierto para que el cliente lo haga
		// solo tendria que setear updater como onStart
		// al ejecutar onStart se vuelve a setear tick
		
		_isActivated = true;
		clearMessages();

		// Llamamos al activar que los hijos deben redefinir
		onActivate();
	}

	//---------------------------------------------------------

	void IComponent::onActivate() {
		// Redefinir por hijos
	}

	//---------------------------------------------------------

	void IComponent::deactivate() {
		_isActivated = false;

		onDeactivate();
	}

	//---------------------------------------------------------

	void IComponent::onDeactivate() {
		// Redefinir por hijos
	}

	//---------------------------------------------------------

	void IComponent::onStart() {
		// Los hijos deben redefinir su comportamiento
	}

	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs) {
		onTick(msecs);
	}

	//---------------------------------------------------------

	void IComponent::onTick(unsigned int msecs) {
		if(_tickMode == TickMode::eBOTH) {
			_tickMode = TickMode::eFIXED_TICK;
		}
		else if(_tickMode == TickMode::eTICK) {
			_tickMode = TickMode::eNONE;
		}

	} // tick

	//---------------------------------------------------------

	void IComponent::fixedTick(unsigned int msecs) {
		onFixedTick(msecs);
	}

	//---------------------------------------------------------

	void IComponent::onFixedTick(unsigned int msecs) {
		if(_tickMode == TickMode::eBOTH) {
			_tickMode = TickMode::eTICK;
		}
		else if(_tickMode == TickMode::eFIXED_TICK) {
			_tickMode = TickMode::eNONE;
		}
	}

	//---------------------------------------------------------

	void IComponent::putToSleep() {
		_state = ComponentState::eSLEEPING;

		onSleep();
	}

	//---------------------------------------------------------

	void IComponent::onSleep() {
		// Redefinir por hijos
	}

	//---------------------------------------------------------

	void IComponent::wakeUp() {
		_state = ComponentState::eAWAKE;

		onWake();
	}

	//---------------------------------------------------------

	void IComponent::onWake() {
		// Redefinir por hijos
	}

	//---------------------------------------------------------

	void IComponent::stayBusy() {
		_state = ComponentState::eBUSY;

		onBusy();
	}

	//---------------------------------------------------------

	void IComponent::onBusy() {
		// Redefinir por hijos
	}

	//---------------------------------------------------------

	void IComponent::stayAvailable() {
		_state = ComponentState::eAWAKE;

		onAvailable();
	}

	//---------------------------------------------------------

	void IComponent::onAvailable() {
		// Redefinir por hijos
	}

} // namespace Logic
