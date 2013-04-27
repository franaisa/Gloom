/**
@file Components/Component.cpp

Contiene la implementación de la clase base de los componentes.
 
@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/

#include "Component.h"
#include "Entity.h"
#include "Logic/Server.h"

namespace Logic 
{
	IComponent::IComponent() : _entity(0), 
							   _isActivated(true) {

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
	
	void IComponent::activateSetup() {
		// Llamamos al activar que los hijos deben redefinir
		activate();
	}

	//---------------------------------------------------------

	void IComponent::activate() {
		if(_isActivated) return;
		
		// Por defecto no forzamos a que se arranque onStart
		// queda abierto para que el cliente lo haga
		// solo tendria que setear updater como onStart
		// al ejecutar onStart se vuelve a setear tick
		
		_isActivated = true;
		clearMessages();
	}

	//---------------------------------------------------------

	void IComponent::deactivate() {
		_isActivated = false;
	}

	//---------------------------------------------------------

	void IComponent::onStart() {
		// Los hijos deben redefinir su comportamiento
	}

	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs) {
		// Procesamos los mensajes que nos hayan llegado
		processMessages();
		onTick(msecs);
	}

	//---------------------------------------------------------

	void IComponent::onTick(unsigned int msecs) {
		// Por defecto el componente indica que no quiere recibir
		// ticks. Si algún hijo redefine este método entonces
		// automáticamente se le llama.
		_entity->wantsTick(this, false);
	} // tick

	//---------------------------------------------------------

	void IComponent::fixedTick(unsigned int msecs) {
		processMessages();
		onFixedTick(msecs);
	}

	//---------------------------------------------------------

	void IComponent::onFixedTick(unsigned int msecs) {
		// Por defecto el componente indica que no quiere 
		// recibir ticks fijos. Si algún hijo redefine este método
		// entonces automáticamente se le llama.
		_entity->wantsFixedTick(this, false);
	}

} // namespace Logic
