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
							   _isActivated(true), 
							   updater(&IComponent::onStartSetup) {

		Logic::CServer::getSingletonPtr()->COMPONENT_CONSTRUCTOR_COUNTER += 1;
	}

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
		// Seteamos el puntero para que se ejecute onStart al comenzar
		updater = &IComponent::onStartSetup;
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

	void IComponent::onStartSetup(unsigned int msecs) {
		// Fijamos el puntero a funcion al tick
		updater = &IComponent::tickSetup;
		// Procesamos los mensajes recibidos
		processMessages();
		// Ejecutamos el onStart definido por el componente hijo
		onStart(msecs);
	}

	//---------------------------------------------------------

	void IComponent::onStart(unsigned int msecs) {
		// Los hijos deben redefinir su comportamiento
	}

	//---------------------------------------------------------

	void IComponent::tickSetup(unsigned int msecs) {
		// Procesamos los mensajes que nos hayan llegado
		processMessages();
		tick(msecs);
	}

	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs) {
		// Los hijos deben redefinir su comportamiento
	} // tick

} // namespace Logic
