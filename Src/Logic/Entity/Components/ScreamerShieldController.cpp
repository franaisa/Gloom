/**
@file ScreamerShieldController.cpp

Contiene la implementación del componente que 
controla el movimiento del escudo del screamer.
 
@see Logic::CScreamerShieldController
@see Logic::IComponent

@author Francisco Aisa García
@date Junio, 2013
*/

#include "ScreamerShieldController.h"
#include "Screamer.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "PhysicDynamicEntity.h"

#include "Logic/Messages/MessageSetOwner.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CScreamerShieldController);

	//________________________________________________________________________

	CScreamerShieldController::CScreamerShieldController() : _owner(NULL) {

		// Nada que hacer
	}

	//________________________________________________________________________

	CScreamerShieldController::~CScreamerShieldController() {
		// Nada que borrar
	}
	
	//________________________________________________________________________

	bool CScreamerShieldController::accept(const shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_OWNER;
	} // accept
	
	//________________________________________________________________________

	void CScreamerShieldController::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::SET_OWNER: {
				setOwner( static_pointer_cast<CMessageSetOwner>(message)->getOwner() );
				setOffset();

				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CScreamerShieldController::onStart() {
		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
	}

	//________________________________________________________________________

	void CScreamerShieldController::onFixedTick(unsigned int msecs) {
		// Aunque este no es el sitio mas idoneo para hacer esto, lo
		// voy a hacer aqui por facilidad
		Vector3 shootPosition = _owner->getPosition() + ( (_owner->getOrientation() * Vector3::NEGATIVE_UNIT_Z) * _capsuleRadius );
		shootPosition.y += _heightShoot;
		
		// Seteamos la posicion fisica del escudo
		if(_physicComponent) {
			_physicComponent->setTransform( shootPosition, _owner->getOrientation(), false );
		}
		else {
			_entity->setPosition( shootPosition );
			_entity->setOrientation( _owner->getOrientation() );
		}
	}

	//________________________________________________________________________

	void CScreamerShieldController::setOwner(Logic::CEntity* owner) { 
		this->_owner = owner;
	}

	//________________________________________________________________________

	void CScreamerShieldController::setOffset() {
		Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Screamer")->clone();
		
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");
	}

} // namespace Logic


