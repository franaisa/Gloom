/**
@file Ammo.cpp

@see Logic::IWeaponAmmo
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "WeaponAmmo.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"

#include "Logic/Messages/MessagePrimaryShoot.h"
#include "Logic/Messages/MessageSecondaryShoot.h"
#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageBlockShoot.h"

// Graficos
// @deprecated Ogre no deberia estar acoplado a la logica
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

using namespace std;

namespace Logic {
	
	IWeaponAmmo::IWeaponAmmo(const string& weaponName) : _weaponName("weapon" + weaponName),
											 _currentAmmo(0),
											 _ableToShoot(true),
											 _primaryFireIsActive(false),
											 _secondaryFireIsActive(false),
											 _friends(0){

		// Nada que inicializar
	}

	//__________________________________________________________________

	IWeaponAmmo::~IWeaponAmmo() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool IWeaponAmmo::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_weaponName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_weaponName + "ID") );

		// Creo q esto no es necesario
		assert( entityInfo->hasAttribute("physic_radius") );
		assert( entityInfo->hasAttribute("heightShoot") );


		// Leemos los atributos obligatorios de arma
		_weaponID = (WeaponType::Enum)entityInfo->getIntAttribute(_weaponName + "ID");
		_maxAmmo = entityInfo->getIntAttribute(_weaponName + "MaxAmmo");
		_noAmmoSound = entityInfo->getStringAttribute("audioNoAmmo");

		// Creo q esto no es necesario
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");


		return true;
	}

	//__________________________________________________________________

	bool IWeaponAmmo::accept(const shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		bool result = false;
		if(message->getMessageType() == Message::CONTROL) {
			shared_ptr<CMessageControl> controlMsg = static_pointer_cast<CMessageControl>(message);
			
			ControlType type = controlMsg->getType();
			
			result =	type == Control::RIGHT_CLICK	||
						type == Control::LEFT_CLICK		||
						type == Control::UNLEFT_CLICK	||
						type == Control::UNRIGHT_CLICK;
		}else{
			result = message->getMessageType() == Message::REDUCED_COOLDOWN	||
					 message->getMessageType() == Message::BLOCK_SHOOT;
		}

		return result;
	}

	//__________________________________________________________________

	void IWeaponAmmo::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				if(_ableToShoot) {
					ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

					if(type == Control::LEFT_CLICK) {
						if( canUsePrimaryFire() )
							primaryFire();
					}
					else if(type == Control::RIGHT_CLICK) {
						if( canUseSecondaryFire() )
							secondaryFire();
					}
					else if(type == Control::UNLEFT_CLICK) {
							stopPrimaryFire();
					}
					else if(type == Control::UNRIGHT_CLICK) {
							stopSecondaryFire();
					}
				}

				break;
			}
			case Message::REDUCED_COOLDOWN: {
				reduceCooldown(std::static_pointer_cast<CMessageReducedCooldown>(message)->getPercentCooldown());
				break;								
			}
			case Message::BLOCK_SHOOT: {
				auto blockShootMsg = static_pointer_cast<CMessageBlockShoot>(message);
				_ableToShoot = blockShootMsg->canShoot();
				break;
			}
		}
	} // process
	//__________________________________________________________________

	void IWeaponAmmo::onDeactivate(){
		stopPrimaryFire();
		stopSecondaryFire();
	} // onDeactivate
	//__________________________________________________________________

	void IWeaponAmmo::primaryFire() {
		_primaryFireIsActive = true;
		// Mandar el mensaje primaryFire(true)
		auto m = make_shared<CMessagePrimaryShoot>(true);
		_entity->emitMessage(m);
	}
	//__________________________________________________________________

	void IWeaponAmmo::secondaryFire() {
		_secondaryFireIsActive = true;
		// Mandar el mensaje seondaryFire(true)
		auto m = make_shared<CMessageSecondaryShoot>(true);
		_entity->emitMessage(m);
	}

	//__________________________________________________________________

	void IWeaponAmmo::stopPrimaryFire() {
		_primaryFireIsActive = false;
		// Mandar el mensaje primaryFire(false)
		auto m = make_shared<CMessagePrimaryShoot>(false);
		_entity->emitMessage(m);
	} // stopPrimaryFire
	//__________________________________________________________________

	void IWeaponAmmo::stopSecondaryFire() {
		_secondaryFireIsActive = false;
		// Mandar el mensaje secondaryFire(false)
		auto m = make_shared<CMessageSecondaryShoot>(false);
		_entity->emitMessage(m);
	} // stopSecondaryFire
	//__________________________________________________________________

	void IWeaponAmmo::onAvailable() {
		shared_ptr<CMessageHudWeapon> message = make_shared<CMessageHudWeapon>();
		message->setWeapon(_weaponID);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);

		for(unsigned int i = 0; i < _friends; ++i){
			_friend[i]->stayAvailable();
		}
		
			
	} // onAvailable
	//__________________________________________________________________

	void IWeaponAmmo::onBusy() {
		for(unsigned int i = 0; i < _friends; ++i){
			_friend[i]->stayBusy();
		}
	} // onBusy
	//__________________________________________________________________

	void IWeaponAmmo::emitSound(const string &soundName, bool loopSound, bool play3d, bool streamSound, bool stopSound){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setAudioName(soundName);
		audioMsg->isLoopable(loopSound);
		audioMsg->is3dSound(play3d);
		audioMsg->streamSound(streamSound);
		audioMsg->stopSound(stopSound);
			
		_entity->emitMessage(audioMsg);
	} // emitSound
	//__________________________________________________________________
	
	void IWeaponAmmo::addAmmo(int weapon, int ammo, bool iAmCatch) {
		_currentAmmo += ammo;
		if(_currentAmmo > _maxAmmo)
			_currentAmmo = _maxAmmo;
		if(iAmCatch) {
			std::shared_ptr<CMessageHudAmmo> message = std::make_shared<CMessageHudAmmo>();
			message->setWeapon(_weaponID);
			message->setAmmo(_currentAmmo);
			_entity->emitMessage(message);
		}
	} // addAmmo
	//__________________________________________________________________

	void IWeaponAmmo::decrementAmmo(unsigned int ammoSpent) {
		_currentAmmo -= ammoSpent;

		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		shared_ptr<CMessageHudAmmo> message = make_shared<CMessageHudAmmo>();
		message->setAmmo(_currentAmmo);

		// Cambio sobre uno, hay q cambiarlo ;-)
		message->setWeapon(_weaponID);
		_entity->emitMessage(message);
	} // decrementAmmo
	//__________________________________________________________________

	void IWeaponAmmo::resetAmmo() {
		_currentAmmo = 0;
	}

	//__________________________________________________________________

	void IWeaponAmmo::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
		OgreDecal::OgreMesh worldMesh;
 
		/// This method will extract all of the triangles from the mesh to be used later. Only should be called once.
		/// If you scale your mesh at all, pass it in here.

		CGraphics* cGraph;
		cGraph = pEntity->getComponent<CGraphics>("CGraphics");
		if (cGraph) {
			worldMesh.initialize( cGraph->getOgreMesh()->getMesh(), Vector3(1,1,1));
 
			/// Get the DecalGenerator singleton and initialize it
			OgreDecal::DecalGenerator& generator = OgreDecal::DecalGenerator::getSingleton();
			//generator.initialize( sceneMgr );
			generator.initialize(cGraph->getSceneManager());
 
			/// Set Decal parameters:
			Ogre::Vector3 pos = vPos; /// Send a ray into the mesh
			float width = 1.0f;
			float height = 1.0f;
			std::string textureName = "gunshotwall"; /// Make sure your texture has a depth_bias greater than 1 to avoid z-fighting

			/// We have everything ready to go. Now it's time to actually generate the decal:
			OgreDecal::Decal decal = generator.createDecal( &worldMesh, pos, width, height, textureName );
 
			/// Render the decal object. Always verify the returned object - it will be NULL if no decal could be created.
			if (decal.object) {
				//sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
				cGraph->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
			}
		}
	} // decals
	//__________________________________________________________________
		

	void IWeaponAmmo::amplifyDamage(unsigned int percentage){
		
	}



} // namespace Logic
