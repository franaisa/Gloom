/**
@file ScreamerShieldDamageNotifier.cpp

Contiene la implementación del componente que 
controla los daños recibidos por el escudo
del Screamer.
 
@see Logic::CScreamerShieldDamageNotifier
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "ScreamerShieldDamageNotifier.h"
#include "Screamer.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "PhysicDynamicEntity.h"

// Mensajes
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageSetReducedDamage.h"

namespace Logic {
	
	IMP_FACTORY(CScreamerShieldDamageNotifier);

	//________________________________________________________________________

	CScreamerShieldDamageNotifier::CScreamerShieldDamageNotifier() :
					 _reducedDamageAbsorption(0),
					 _owner(NULL) {

		// Nada que hacer
	}

	//________________________________________________________________________

	CScreamerShieldDamageNotifier::~CScreamerShieldDamageNotifier() {
		// Nada que borrar
	}
	
	//________________________________________________________________________

	bool CScreamerShieldDamageNotifier::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED || msgType == Message::SET_REDUCED_DAMAGE;
	} // accept
	
	//________________________________________________________________________

	void CScreamerShieldDamageNotifier::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::DAMAGED: {
				std::shared_ptr<CMessageDamaged> dmgMsg = std::static_pointer_cast<CMessageDamaged>(message);
				damaged( dmgMsg->getDamage(), dmgMsg->getEnemy() );
				break;
			}
			case Message::SET_REDUCED_DAMAGE: {
				std::shared_ptr<CMessageSetReducedDamage> reducedDmgMsg = std::static_pointer_cast<CMessageSetReducedDamage>(message);
				reducedDamageAbsorption( reducedDmgMsg->getReducedDamage() );
				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CScreamerShieldDamageNotifier::onActivate() {
		_reducedDamageAbsorption = 0;
	}

	//________________________________________________________________________

	void CScreamerShieldDamageNotifier::damaged(int damage, CEntity* enemy) {
		// Si hay una reduccion de daño activa, reducimos el daño aplicado
		damage -= damage * _reducedDamageAbsorption;

		// Notificamos al componente de la entidad que nos creó (un personaje
		// Screamer) del daño. Dicha entidad gestiona el aguante del escudo.

		// Para no consumir dos ticks, no vamos a reenviar el mensaje de daño
		// al componente CScreamer, sino que sencillamente vamos a llamar al
		// método que corresponda del propio componente.
		CScreamer* screamerComponent = _owner->getComponent<CScreamer>("CScreamer");
		assert(screamerComponent && "Error: La entidad del jugador no tiene ningun componente llamado CScreamer");
		screamerComponent->absorbDamage(damage);
	}// damaged

	//________________________________________________________________________

	void CScreamerShieldDamageNotifier::reducedDamageAbsorption(float percentage) {
		_reducedDamageAbsorption = percentage;
	}

	//________________________________________________________________________

	void CScreamerShieldDamageNotifier::setOwner(Logic::CEntity* owner) { 
		this->_owner = owner; 
	}

} // namespace Logic


