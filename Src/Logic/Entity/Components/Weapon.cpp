/**
@file Weapon.cpp

Contiene la implementación de la interfaz común
a todas las armas.

@see Logic::IWeapon
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "Weapon.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageDecal.h"
#include "Logic/Messages/MessageBlockShoot.h"

#include "Logic/Messages/MessagePrimaryShoot.h"
#include "Logic/Messages/MessageSecondaryShoot.h"


// Graficos
// @deprecated Ogre no deberia estar acoplado a la logica
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

#include "Logic/Messages/MessageCreateParticle.h"
#include "Graphics/Particle.h"

using namespace std;

namespace Logic {
	
	IWeapon::IWeapon(const string& weaponName) : _weaponName(weaponName),
												 _ableToShoot(true),
												 _currentAmmo(0) {
		// Nada que inicializar
	}

	//__________________________________________________________________

	IWeapon::~IWeapon() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool IWeapon::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute("MaxAmmo") );
		assert( entityInfo->hasAttribute("ID") );
		

		// Leemos los atributos obligatorios de arma
		_weaponID = (WeaponType::Enum)entityInfo->getIntAttribute("ID");

		
		Map::CEntity* myEntityInfo = CEntityFactory::getSingletonPtr()->getInfo(_entity->getType());

		_maxAmmo = entityInfo->getIntAttribute("MaxAmmo");
		assert( myEntityInfo->hasAttribute("physic_radius") );
		assert( myEntityInfo->hasAttribute("heightShoot") );
		_capsuleRadius = myEntityInfo->getFloatAttribute("physic_radius");
		_heightShoot = myEntityInfo->getFloatAttribute("heightShoot");
		
		
		return true;
	}

	//__________________________________________________________________

	bool IWeapon::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		return msgType == Message::PRIMARY_SHOOT	|| 
			   msgType == Message::SECONDARY_SHOOT	||
			   msgType == Message::BLOCK_SHOOT;
	}

	//__________________________________________________________________

	void IWeapon::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SHOOT: {
				if(_ableToShoot) {
					auto primaryShootMsg = static_pointer_cast<CMessagePrimaryShoot>(message);
					if(primaryShootMsg->getShoot()){
						primaryFire();
						particles();
					}
					else{
						stopPrimaryFire();
					}
				}
				break;
			}
			case Message::SECONDARY_SHOOT: {
				if(_ableToShoot) {
					auto secondaryShootMsg = static_pointer_cast<CMessageSecondaryShoot>(message);
					if(secondaryShootMsg ->getShoot()){
						secondaryFire();
					}
					else{
						stopSecondaryFire();
					}
				}
				break;
			}
			case Message::BLOCK_SHOOT: {
				auto blockShootMsg = static_pointer_cast<CMessageBlockShoot>(message);
				_ableToShoot = blockShootMsg->canShoot();
				break;
			}
		}
	}

	//__________________________________________________________________

	void IWeapon::onDeactivate(){
		stopPrimaryFire();
		stopSecondaryFire();
	} // onDeactivate
	//__________________________________________________________________

	void IWeapon::onAvailable() {

	} // onAvailable
	//__________________________________________________________________

	void IWeapon::addAmmo(int weapon, int ammo, bool iAmCatch) {
		_currentAmmo += ammo;
		if(_currentAmmo > _maxAmmo) 
			_currentAmmo = _maxAmmo;

		/*
		if(iAmCatch) {
			std::shared_ptr<CMessageHudAmmo> message = std::make_shared<CMessageHudAmmo>();
			message->setWeapon(_weaponID);
			message->setAmmo(_currentAmmo);
			_entity->emitMessage(message);
		}
		*/
	} // addAmmo

	//__________________________________________________________________

	void IWeapon::decrementAmmo(unsigned int ammoSpent) {
		_currentAmmo -= ammoSpent;

		/*
		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		shared_ptr<CMessageHudAmmo> message = make_shared<CMessageHudAmmo>();
		message->setAmmo(_currentAmmo);

		// Cambio sobre uno, hay q cambiarlo ;-)
		message->setWeapon(_weaponID);
		_entity->emitMessage(message);
		*/
	}

	//__________________________________________________________________

	void IWeapon::resetAmmo() {
		_currentAmmo = 0;
	}

	//__________________________________________________________________

	void IWeapon::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
		OgreDecal::OgreMesh worldMesh;
 
		/// This method will extract all of the triangles from the mesh to be used later. Only should be called once.
		/// If you scale your mesh at all, pass it in here.

		shared_ptr<CMessageDecal> messageDecal = make_shared<CMessageDecal>();
		messageDecal->setPosition(vPos);
		messageDecal->setTexture("gunshotwall");
		pEntity->emitMessage(messageDecal);
	}

	//__________________________________________________________________

	void IWeapon::emitSound(const string &soundName, bool loopSound, bool play3d, bool streamSound, bool stopSound){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setAudioName(soundName);
		audioMsg->isLoopable(loopSound);
		audioMsg->is3dSound(play3d);
		audioMsg->streamSound(streamSound);
		audioMsg->stopSound(stopSound);
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________


	void IWeapon::particles()
	{
		std::cout << "particula" << std::endl;
		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle("ShootParticle2");

		Vector3 position2 = _entity->getPosition();
		position2.y += _heightShoot;
		//std::cout << "posicionOrig = " << position2 << std::endl;

		
		float fOffset = 8.0f;
		Vector3 orientation= _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
		orientation.normalise();
		orientation *= fOffset;
		//std::cout << "orientacion = " << orientation << std::endl;

		position2 += orientation;
		//std::cout << "posicion NUEVA = " << position2 << std::endl;

		particleMsg->setPosition(position2);
		//Vector3 dir(0,0,0);
		//particleMsg->setDirectionWithForce(dir);
		_entity->emitMessage(particleMsg);
	}

	

} // namespace Logic

