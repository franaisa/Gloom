/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "FireBallController.h"
#include "IronHellGoat.h"
#include "PhysicDynamicEntity.h"

#include "Logic/Maps/EntityFactory.h"

#include "Logic/Messages/MessageContactEnter.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CFireBallController);

	//________________________________________________________________________

	CFireBallController::CFireBallController() { 
		// Nada que hacer
	}

	//________________________________________________________________________

	CFireBallController::~CFireBallController() {
		// Le indicamos al componente dueño que nos vamos a destruir
		// y que por lo tanto ya no vamos a poder recibir mas instrucciones
		// de move
		if(_owner != NULL) {
			_owner->removeFireBall(this);
			_owner = NULL;
		}
	}

	//________________________________________________________________________

	void CFireBallController::onStart() {
		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
		assert(_physicComponent != NULL && "Error: La bola de fuego no tiene collider");
	}

	//________________________________________________________________________

	bool CFireBallController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("speed") );
		assert( entityInfo->hasAttribute("damage") );
		assert( entityInfo->hasAttribute("explotionRadius") );
		assert( entityInfo->hasAttribute("direction") );

		_speed = entityInfo->getFloatAttribute("speed");
		_damage = entityInfo->getFloatAttribute("damage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");
		_direction = entityInfo->getVector3Attribute("direction");

		return true;
	}

	//________________________________________________________________________

	bool CFireBallController::accept(const shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTACT_ENTER;
	}

	//________________________________________________________________________

	void CFireBallController::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTACT_ENTER: {
				createExplotion();
			}
		}
	}

	//________________________________________________________________________

	void CFireBallController::onFixedTick(unsigned int msecs) {
		// Muevo a la bola
		
		// @todo llamar al move del kinematico con la velocidad * direccion * tiempo
		_physicComponent->move( _direction * _speed * msecs );
	}
	
	//________________________________________________________________________

	void CFireBallController::setOwner(CIronHellGoat* owner) {
		this->_owner = owner;
	}

	//________________________________________________________________________

	void CFireBallController::createExplotion() {
		// Generar un overlap y mandar los mensajes de daño a las entidades
		// con las que se impacte

		// Destruir en diferido esta entidad
	}

} // namespace Logic

