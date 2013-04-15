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
							   _isStartingUp(true) {

	}

	//---------------------------------------------------------

	bool IComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		_entity = entity;
		return true;

	} // spawn

	//---------------------------------------------------------
	
	void IComponent::activate() {
		if(_isActivated) 
			return;

		_isActivated = _isStartingUp = true;
		clearMessages();
	}

	//---------------------------------------------------------

	void IComponent::deactivate() {
		_isActivated = false;
	}

	//---------------------------------------------------------

	void IComponent::onStart(unsigned int msecs) {
		_isStartingUp = false;
		processMessages();
	}

	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs) {
		processMessages();

	} // tick

} // namespace Logic
