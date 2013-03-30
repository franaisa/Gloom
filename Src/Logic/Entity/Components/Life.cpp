/**
@file Life.cpp

Contiene la implementación del componente 
que controla la vida de un personaje.
 
@see Logic::CLife
@see Logic::IComponent

@author Francisco Aisa García
@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "Life.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Para informar por red que se ha acabado el juego
#include "Net/Manager.h"
#include "Net/buffer.h"
#include "Logic/PlayerInfo.h"
#include "Logic/GameNetPlayersManager.h"
#include "Logic/GameNetMsgManager.h"

// Mensajes
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessageCameraToEnemy.h"
#include "Logic/Messages/MessageSetReducedDamage.h"

namespace Logic {
	
	IMP_FACTORY(CLife);

	//________________________________________________________________________

	CLife::CLife() : _damageTimer(0), 
					 _reducedDamageAbsorption(0) {

		// Nada que hacer
	}

	//________________________________________________________________________

	CLife::~CLife() {
		// Nada que borrar
	}
	
	//________________________________________________________________________
	
	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		// ATRIBUTOS DE VIDA Y ARMADURA

		assert( entityInfo->hasAttribute("defaultLife") );
		_defaultLife = entityInfo->getIntAttribute("defaultLife");
		
		assert(entityInfo->hasAttribute("damageOverTime"));
		_damageOverTime = entityInfo->getIntAttribute("damageOverTime");
		
		assert(entityInfo->hasAttribute("damageTimeStep"));
		_damageTimeStep = entityInfo->getIntAttribute("damageTimeStep") * 1000; // Traducimos a milisegundos
		
		// Traducimos el daño al rango 0-1 suponiendo que el daño esté dado entre 0-100
		assert(entityInfo->hasAttribute("shieldDamageAbsorption"));
		_shieldDamageAbsorption = entityInfo->getFloatAttribute("shieldDamageAbsorption") * 0.01f;
		
		assert(entityInfo->hasAttribute("maxLife"));
		_maxLife = entityInfo->getIntAttribute("maxLife");
		
		assert(entityInfo->hasAttribute("maxShield"));
		_maxShield = entityInfo->getIntAttribute("maxShield");
		

		// ATRIBUTOS DE SONIDO

		assert(entityInfo->hasAttribute("audioPain"));
		_audioPain =  entityInfo->getStringAttribute("audioPain");
		
		assert(entityInfo->hasAttribute("audioDeath"));
		_audioDeath =  entityInfo->getStringAttribute("audioDeath");

		return true;
	} // spawn
	
	//________________________________________________________________________
	
	void CLife::activate() {
		IComponent::activate();
		
		// Resteamos los valores de salud y escudo a los valores por defecto
		_currentLife = _defaultLife;
		_currentShield = 0;

		// Actualizamos la info del HUD
		Logic::CMessageHudLife* hudLifeMsg = new Logic::CMessageHudLife();
		hudLifeMsg->setLife(_currentLife);
		_entity->emitMessage(hudLifeMsg);	
		
		Logic::CMessageHudShield* hudShieldMsg = new Logic::CMessageHudShield();
		hudShieldMsg->setShield(_currentShield);
		_entity->emitMessage(hudShieldMsg);	
	} // activate
	
	//________________________________________________________________________

	bool CLife::accept(CMessage* message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED				|| 
			   msgType == Message::ADD_LIFE				||
			   msgType == Message::ADD_SHIELD			||
			   msgType == Message::SET_REDUCED_DAMAGE;
	} // accept
	
	//________________________________________________________________________

	void CLife::process(CMessage* message) {
		switch( message->getMessageType() ) {
			case Message::DAMAGED: {
				CMessageDamaged* dmgMsg = static_cast<CMessageDamaged*>(message);
				damaged( dmgMsg->getDamage(), dmgMsg->getEnemy() );
				break;
			}
			case Message::ADD_LIFE: {
				addLife( static_cast<CMessageAddLife*>(message)->getAddLife() );
				break;
			}	
			case Message::ADD_SHIELD: {
				addShield( static_cast<CMessageAddShield*>(message)->getAddShield() );
				break;
			}
			case Message::SET_REDUCED_DAMAGE: {
				reducedDamageAbsorption( static_cast<CMessageSetReducedDamage*>(message)->getReducedDamage() );
				break;
			}
		}
	} // process
	
	//________________________________________________________________________

	void CLife::tick(unsigned int msecs) {
		// Necesario para que se procesen todos los mensajes aceptados
		IComponent::tick(msecs);

		_damageTimer += msecs;
		if(_damageTimer >= _damageTimeStep && _currentLife != 1) {
			// Reducimos la vida hasta un minimo de un punto de salud
			_currentLife =  _damageOverTime < _currentLife ? (_currentLife - _damageOverTime) : 1;

			// Actualización la información de vida del HUD
			Logic::CMessageHudLife* hudLifeMsg = new Logic::CMessageHudLife();
			hudLifeMsg->setLife(_currentLife);
			_entity->emitMessage(hudLifeMsg);	

			// Resteamos el timer
			_damageTimer = 0;
		}
	} // tick

	//________________________________________________________________________

	void CLife::damaged(int damage, CEntity* enemy) {
		// Actualizamos los puntos de salud y armadura del personaje.
		// En caso de muerte activamos la escena de muerte y disparamos los sonidos
		// correspondientes.
		if( updateLife(damage) ) {
			triggerDeathState(enemy);
			triggerDeathSound();
		}
		// Si el personaje no ha muerto lanzamos los sonidos de daño.
		else {
			triggerHurtSound();
		}
	}// damaged
	
	//________________________________________________________________________
	
	void CLife::addLife(int life) {
		if(_currentLife < _maxLife) {
			if(_currentLife + life <= _maxLife)
				_currentLife += life;
			else
				_currentLife = _maxLife;

			Logic::CMessageHudLife* hudLifeMsg = new Logic::CMessageHudLife();
			hudLifeMsg->setLife(_currentLife);
			_entity->emitMessage(hudLifeMsg);
		}
	}// addLife
	
	//________________________________________________________________________

	void CLife::addShield(int shield) {
		if(_currentShield < _maxShield) {
			if(_currentShield + shield <= _maxShield)
				_currentShield += shield;
			else
				_currentShield = _maxShield;

			Logic::CMessageHudShield* hudShieldMsg = new Logic::CMessageHudShield();
			hudShieldMsg->setShield(_currentShield);
			_entity->emitMessage(hudShieldMsg);
		}// addShield
	}

	//________________________________________________________________________

	void CLife::reducedDamageAbsorption(float percentage) {
		_reducedDamageAbsorption = percentage;
	}

	//________________________________________________________________________

	bool CLife::updateLife(int damage) {
		// Si hay una reduccion de daño activa, reducimos el daño aplicado
		damage -= damage * _reducedDamageAbsorption;

		std::cout << "Me quitan " << damage << std::endl;

		if(_currentShield > 0) {
			int damageAbsorbedByShield = _shieldDamageAbsorption * damage;
			int damageAbsorbedByLife = damage - damageAbsorbedByShield;
			
			if(_currentShield >= damageAbsorbedByShield) {
				_currentShield -= damageAbsorbedByShield;
				_currentLife -= damageAbsorbedByLife;
			}
			else {
				damageAbsorbedByShield -= _currentShield;
				_currentShield = 0;
				_currentLife -= damageAbsorbedByLife + damageAbsorbedByShield;
			}

			// Actualizamos los puntos de armadura mostrados en el HUD
			Logic::CMessageHudShield* hudShieldMsg = new Logic::CMessageHudShield();
			hudShieldMsg->setShield(_currentShield);
			_entity->emitMessage(hudShieldMsg);
		}
		else {
			_currentLife -= damage;
		}

		if(_currentLife < 0)
			_currentLife = 0;

		// Actualizamos los puntos de salud mostrados en el HUD
		Logic::CMessageHudLife* hudLifeMsg = new Logic::CMessageHudLife();
		hudLifeMsg->setLife(_currentLife);
		_entity->emitMessage(hudLifeMsg);

		return _currentLife == 0;
	}

	//________________________________________________________________________

	void CLife::triggerDeathState(CEntity* enemy) {
		// Mensaje de playerDead para tratar el respawn y desactivar los componentes
		// del personaje.
		Logic::CMessagePlayerDead* playerDeadMsg = new Logic::CMessagePlayerDead();
		_entity->emitMessage(playerDeadMsg);

		// Mensaje para que la camara enfoque al jugador que nos ha matado
		// En el caso de la red, hay que enviar un mensaje especial para el cliente
		// Siempre y cuando no haya muerto un remotePlayer/enemigo (debug singlePlayer)
		Logic::CMessageCameraToEnemy* cteMsg;
		CEntity* camera;
		if(_entity->getType().compare("LocalPlayer")==0){
			cteMsg = new Logic::CMessageCameraToEnemy();
			cteMsg->setEnemy(enemy);

			camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
			camera->emitMessage(cteMsg);
		}
		// Enviamos el mensaje por la red
		if( Net::CManager::getSingletonPtr()->imServer() )
			Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(cteMsg, camera->getEntityID(), _entity->getEntityID());
	}

	//________________________________________________________________________

	void CLife::triggerDeathSound() {
		Logic::CMessageAudio* audioMsg = new Logic::CMessageAudio();
		audioMsg->setRuta(_audioDeath);
		audioMsg->setId("death");
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(_entity->isPlayer());
		_entity->emitMessage(audioMsg);
	}

	//________________________________________________________________________

	void CLife::triggerHurtSound() {
		Logic::CMessageAudio* audioMsg = new Logic::CMessageAudio();
		audioMsg->setRuta(_audioPain);
		audioMsg->setId("pain");
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(_entity->isPlayer());
		_entity->emitMessage(audioMsg);
	}

} // namespace Logic


