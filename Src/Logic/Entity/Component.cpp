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

namespace Logic 
{
	IComponent::IComponent() : _entity(0), 
							   _isActivated(true), 
							   updater(&IComponent::tick) {

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
	}

	//---------------------------------------------------------

	void IComponent::deactivate() {
		_isActivated = false;
	}

	//---------------------------------------------------------

	void IComponent::onStart(unsigned int msecs) {
		// Seteamos el puntero a funcion como el tick
		updater = &IComponent::tick;
		// Procesamos los mensajes que nos hayan llegado
		processMessages();
	}

	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs) {
		// Procesamos los mensajes que nos hayan llegado
		processMessages();
	} // tick

} // namespace Logic
