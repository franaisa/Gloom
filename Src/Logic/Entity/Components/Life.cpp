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
		std::shared_ptr<CMessageHudLife> hudLifeMsg = std::make_shared<CMessageHudLife>();
		hudLifeMsg->setLife(_currentLife);
		_entity->emitMessage(hudLifeMsg);	
		
		std::shared_ptr<CMessageHudShield> hudShieldMsg = std::make_shared<CMessageHudShield>();
		hudShieldMsg->setShield(_currentShield);
		_entity->emitMessage(hudShieldMsg);	
	} // activate
	
	//________________________________________________________________________

	bool CLife::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED				|| 
			   msgType == Message::ADD_LIFE				||
			   msgType == Message::ADD_SHIELD			||
			   msgType == Message::SET_REDUCED_DAMAGE;
	} // accept
	
	//________________________________________________________________________

	void CLife::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::DAMAGED: {
				std::shared_ptr<CMessageDamaged> dmgMsg = std::static_pointer_cast<CMessageDamaged>(message);
				damaged( dmgMsg->getDamage(), dmgMsg->getEnemy() );
				std::cout << "soy " << _entity->getName() << " y me hace " << dmgMsg->getDamage() << " el enemigo " << dmgMsg->getEnemy()->getName() << std::endl;
				break;
			}
			case Message::ADD_LIFE: {
				std::shared_ptr<CMessageAddLife> addLifeMsg = std::static_pointer_cast<CMessageAddLife>(message);
				addLife( addLifeMsg->getAddLife() );
				break;
			}	
			case Message::ADD_SHIELD: {
				std::shared_ptr<CMessageAddShield> addShieldMsg = std::static_pointer_cast<CMessageAddShield>(message);
				addShield( addShieldMsg->getAddShield() );
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

	void CLife::tick(unsigned int msecs) {
		// Necesario para que se procesen todos los mensajes aceptados
		IComponent::tick(msecs);

		_damageTimer += msecs;
		if(_damageTimer >= _damageTimeStep && _currentLife != 1) {
			// Reducimos la vida hasta un minimo de un punto de salud
			_currentLife =  _damageOverTime < _currentLife ? (_currentLife - _damageOverTime) : 1;

			// Actualización la información de vida del HUD
			std::shared_ptr<CMessageHudLife> hudLifeMsg = std::make_shared<CMessageHudLife>();
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

			std::shared_ptr<CMessageHudLife> hudLifeMsg = std::make_shared<CMessageHudLife>();
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

			std::shared_ptr<CMessageHudShield> hudShieldMsg = std::make_shared<CMessageHudShield>();
			hudShieldMsg->setShield(_currentShield);
			_entity->emitMessage(hudShieldMsg);
		}// addShield
	}

	//________________________________________________________________________

	void CLife::reducedDamageAbsorption(float percentage) {
		std::cout << "reduced damage absortion = " << percentage << std::endl;
		_reducedDamageAbsorption = percentage;
	}

	//________________________________________________________________________

	void CLife::suicide() {
		triggerDeathState(_entity);
		triggerDeathSound();
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
			std::shared_ptr<CMessageHudShield> hudShieldMsg = std::make_shared<CMessageHudShield>();
			hudShieldMsg->setShield(_currentShield);
			_entity->emitMessage(hudShieldMsg);
		}
		else {
			_currentLife -= damage;
		}

		if(_currentLife < 0)
			_currentLife = 0;

		// Actualizamos los puntos de salud mostrados en el HUD
		std::shared_ptr<CMessageHudLife> hudLifeMsg = std::make_shared<CMessageHudLife>();
		hudLifeMsg->setLife(_currentLife);
		_entity->emitMessage(hudLifeMsg);

		std::cout << "Me quedan " << _currentLife << " puntos de salud" << std::endl;

		return _currentLife == 0;
	}

	//________________________________________________________________________

	void CLife::triggerDeathState(CEntity* enemy) {
		// Mensaje de playerDead para tratar el respawn y desactivar los componentes
		// del personaje.
		std::shared_ptr<CMessagePlayerDead> playerDeadMsg = std::make_shared<CMessagePlayerDead>();
		_entity->emitMessage(playerDeadMsg);

		// Mensaje para que la camara enfoque al jugador que nos ha matado
		// En el caso de la red, hay que enviar un mensaje especial para el cliente
		// Siempre y cuando no haya muerto un remotePlayer/enemigo (debug singlePlayer)
		std::shared_ptr<CMessageCameraToEnemy> cteMsg = std::make_shared<CMessageCameraToEnemy>();
		CEntity* camera=camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
		cteMsg->setEnemy(enemy);
		//Solo si soy el jugador local envio mensaje de recolocación de camara (no quiero que enemigos muertos me seteen mi camara)
		if(_entity->getType().compare("LocalPlayer")==0){
			camera->emitMessage(cteMsg);
		}
		// Enviamos el mensaje por la red
		if( Net::CManager::getSingletonPtr()->imServer() )
			Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(cteMsg, camera->getEntityID(), _entity->getEntityID());
	}

	//________________________________________________________________________

	void CLife::triggerDeathSound() {
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
		audioMsg->setRuta(_audioDeath);
		audioMsg->setId("death");
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(_entity->isPlayer());
		_entity->emitMessage(audioMsg);
	}

	//________________________________________________________________________

	void CLife::triggerHurtSound() {
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
		audioMsg->setRuta(_audioPain);
		audioMsg->setId("pain");
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(false);
		audioMsg->setIsPlayer(_entity->isPlayer());
		_entity->emitMessage(audioMsg);
	}

} // namespace Logic


