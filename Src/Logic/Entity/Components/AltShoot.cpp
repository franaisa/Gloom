/**
@file AltShoot.cpp


@see Logic::CAltShoot
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#include "AltShoot.h"

#include "Logic/Messages/MessageControl.h"

using namespace std;

namespace Logic {
	
	CAltShoot::CAltShoot(const string& weaponName) : _weaponName(weaponName),
													 _primaryFireCooldown(0), 
													 _secondaryFireCooldown(0) {

		
	}

	CAltShoot::~CAltShoot() {

	}

	//__________________________________________________________________

	bool CAltShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Leer los atributos que toque
	}

	bool CAltShoot::accept(const shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		if(message->getMessageType() == Message::CONTROL) {
			shared_ptr<CMessageControl> controlMsg = static_pointer_cast<CMessageControl>(message);
			
			ControlType type = controlMsg->getType();
			
			return type == Control::RIGHT_CLICK		||
				   type == Control::LEFT_CLICK		||
				   type == Control::UNLEFT_CLICK	||
				   type == Control::UNRIGHT_CLICK;
		}

		return false;
	}

	void CAltShoot::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

				if(type == Control::RIGHT_CLICK) {
					if( canUsePrimaryFire() ) {
						//shootPrimary(); // Ejecutar patron
						//primaryFire();
						//_primarySkillTimer = _primarySkillCooldown;
					}
				}
				else if(type == Control::LEFT_CLICK) {
					if( canUseSecondaryFire() ) {
						//shootSecondary(); // Ejecutar patron
						//secondaryFire();
						//_secondarySkillTimer = _secondarySkillCooldown;
					}
					else {
						// Lanzar mensaje de sin balas y tal
					}
				}
				else if(type == Control::UNLEFT_CLICK) {
					stopPrimaryFire();
				}
				else if(type == Control::UNRIGHT_CLICK) {
					stopSecondaryFire();
				}

				break;
			}
		}
	}

	//__________________________________________________________________

	void CAltShoot::onTick(unsigned int msecs) {
		// Controlar los timers de los cooldowns
	}

	void CAltShoot::onAvailable() {

	}

	bool CAltShoot::canUsePrimaryFire() {
		return false;
	}

	bool CAltShoot::canUseSecondaryFire() {
		return false;
	}

} // namespace Logic

