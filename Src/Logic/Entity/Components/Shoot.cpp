/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#include "Shoot.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"
#include "BaseSubsystems/Math.h"

#include "Graphics/Camera.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageBerserker.h"
#include "Logic/Messages/MessageCreateParticle.h"
#include "Logic/Messages/MessageAudio.h"


// mensaje para debbug
#include "Logic/Messages/MessageHudDebugData.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	//IMP_FACTORY(CShoot);
	
	//__________________________________________________________________

	CShoot::~CShoot() {
		// Nada que destruir
	}
	//__________________________________________________________________

	bool CShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("physic_radius")) {
			_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		}

		if(entityInfo->hasAttribute("heightShoot")) {
			_heightShoot = entityInfo->getIntAttribute("heightShoot");
		}
		
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();
												
		_name = entityInfo->getStringAttribute(weapon+"Name");
		if(entityInfo->hasAttribute(weapon+"Damage")) {
			_damage= entityInfo->getIntAttribute(weapon+"Damage");
		}
		_defaultDamage=_damage;
		_numberShots = entityInfo->getIntAttribute(weapon+"NumberShoots");
		_cooldown = entityInfo->getFloatAttribute(weapon+"ColdDown") * 1000;
		_defaultCooldown=_cooldown;
		_maxAmmo = entityInfo->getIntAttribute(weapon+"MaxAmmo");
		_id = entityInfo->getIntAttribute(weapon+"Id");
		if(entityInfo->hasAttribute(weapon+"ParticlePosition")) {
			_particlePosition = entityInfo->getVector3Attribute(weapon+"ParticlePosition");
		}
		return true;
	} // spawn
	//__________________________________________________________________

	bool CShoot::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTROL;
	} // accept
	//__________________________________________________________________

	void CShoot::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();
				
				if(type == Control::LEFT_CLICK) {
					primaryShoot();
				}
				else if(type == Control::RIGHT_CLICK) {
					secondaryShoot();
				}
				else if(type == Control::UNLEFT_CLICK) {
					stopPrimaryShoot();
				}
				else if(type == Control::UNRIGHT_CLICK) {
					stopSecondaryShoot();
				}
				break;
			}
		}
	} // process
	//__________________________________________________________________

	void CShoot::onTick(unsigned int msecs) {
		if(_cooldownTimer < _cooldown)
			_cooldownTimer += msecs;
		else
			_canShoot = true;

	} // tick
	//__________________________________________________________________

	void CShoot::addAmmo(int weapon, int ammo, bool iAmCatch) {
		if(_currentAmmo + ammo>_maxAmmo)
			_currentAmmo = _maxAmmo;
		else
			_currentAmmo += ammo;

		if(iAmCatch) {
			std::shared_ptr<CMessageHudAmmo> message = std::make_shared<CMessageHudAmmo>();
			message->setWeapon(_id);
			message->setAmmo(_currentAmmo);
			_entity->emitMessage(message);
		}
	} // addAmmo
	//__________________________________________________________________

	void CShoot::decrementAmmo() {
		--_currentAmmo;

		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		std::shared_ptr<CMessageHudAmmo> message = std::make_shared<CMessageHudAmmo>();
		message->setAmmo(_currentAmmo);

		//Cambio sobre uno, hay q cambiarlo ;-)
		message->setWeapon(_id);
		_entity->emitMessage(message);
	}// decrementAmmo
	//__________________________________________________________________

	void CShoot::resetAmmo() {
		//si yo soy el weapon
		_currentAmmo = 0;
	} // resetAmmo
	//__________________________________________________________________


	void CShoot::drawParticle(const std::string &nombreParticula, const std::string &particula){
	
		Vector3 directionWithForce = Math::getDirection(_entity->getOrientation());
		Vector3 positionParticle = (_entity->getPosition()+ Vector3(0,_heightShoot,0)) + ((directionWithForce) * _particlePosition);
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		
		directionWithForce *= 10;

		std::shared_ptr<CMessageCreateParticle> particle = std::make_shared<CMessageCreateParticle>();
		particle->setParticle(particula);
		particle->setPosition(positionParticle);
		particle->setDirectionWithForce(directionWithForce);
		_entity->emitMessage(particle);

	} // drawParticle
	//__________________________________________________________________

	void CShoot::emitSound(const std::string &ruta, const std::string &sound, bool notIfPlay){
		
		std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
			audioMsg->setRuta(ruta);
			audioMsg->setId(sound);
			audioMsg->setPosition(_entity->getPosition());
			audioMsg->setNotIfPlay(notIfPlay);
			audioMsg->setIsPlayer(_entity->isPlayer());
			_entity->emitMessage(audioMsg);
	} // emitSound
	//__________________________________________________________________

	void CShoot::incrementDamage(int percent) {
		//Si es 0 significa que hay que restaurar al que habia por defecto
		if(percent==0){
			_damage=_defaultDamage;
		}
		//Sino aplicamos el porcentaje pasado por parámetro
		else{
			_damage+=(percent*_damage)/100;
		}
	} // incrementDamage
	//__________________________________________________________________

	void CShoot::reduceCooldown(int percent) {
		
		//Si es 0 significa que hay que restaurar al que habia por defecto
		if(percent==0){
			_cooldown=_defaultCooldown;
		}
		//Sino aplicamos el porcentaje pasado por parámetro
		else{
			_cooldown=_cooldown-(percent*_cooldown)/100;
		}
	} // reduceCooldown
	//__________________________________________________________________

	void CShoot::onAvailable() {
		std::shared_ptr<CMessageHudWeapon> message = std::make_shared<CMessageHudWeapon>();
		message->setWeapon(_id);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);
	}
	//__________________________________________________________________

	void CShoot::stopPrimaryShoot(){
	
	} // stopPrimaryShoot
	//__________________________________________________________________

	void CShoot::stopSecondaryShoot(){
	
	} // stopSecondaryShoot
	//__________________________________________________________________

} // namespace Logic

