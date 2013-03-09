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

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	//IMP_FACTORY(CShoot);
	
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
		_damage= (unsigned char) entityInfo->getIntAttribute(weapon+"Damage");
		_numberShots = (unsigned char) entityInfo->getIntAttribute(weapon+"NumberShoots");
		_cooldown = (unsigned char) entityInfo->getIntAttribute(weapon+"ColdDown") * 1000;
		_maxAmmo = entityInfo->getIntAttribute(weapon+"MaxAmmo");
		_id = entityInfo->getIntAttribute(weapon+"Id");
		if(entityInfo->hasAttribute(weapon+"ParticlePosition")) {
			_particlePosition = entityInfo->getVector3Attribute(weapon+"ParticlePosition");
		}
		return true;
	} // spawn
	
	//__________________________________________________________________

	bool CShoot::accept(CMessage *message) {
		return message->getMessageType() == Message::CONTROL;
	} // accept
	
	//__________________________________________________________________

	void CShoot::process(CMessage *message) {
		switch(message->getMessageType()) {
			case Message::CONTROL:
				if(((CMessageControl*)message)->getType()==Control::LEFT_CLICK) {
					shoot();
				}
				if(((CMessageControl*)message)->getType()==Control::RIGHT_CLICK) {
					printf("\nx: %f, y: %f, z: %f",_entity->getPosition().x, _entity->getPosition().y, _entity->getPosition().z);
				}
				break;
			break;
		}
		
	} // process
	//__________________________________________________________________

	void CShoot::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		
		if(_cooldownTimer < _cooldown)
			_cooldownTimer += msecs;
		else
			_canShoot = true;

	} // tick

	//__________________________________________________________________

	// Cuando activo una entidad de disparo, actulio la hub indicandole cuanta municion tengo
	
	void CShoot::activate() {
		IComponent::activate();

		Logic::CMessageHudWeapon *message = new Logic::CMessageHudWeapon();
		message->setWeapon(_id);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);
	} // activate

	//__________________________________________________________________

	void CShoot::addAmmo(int weapon, int ammo, bool iAmCatch) {
		if(_currentAmmo + ammo>_maxAmmo)
			_currentAmmo = _maxAmmo;
		else
			_currentAmmo += ammo;

		if(iAmCatch) {
			Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
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
		Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
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

	void CShoot::drawParticle(const std::string &nombreParticula, const std::string &particula){


		
		Vector3 positionParticle = _entity->getPosition() + (Math::getDirection(_entity->getOrientation()) * _particlePosition);
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		
		_scen->createParticle(_entity->getName(),particula, &positionParticle);
		
	}
} // namespace Logic

