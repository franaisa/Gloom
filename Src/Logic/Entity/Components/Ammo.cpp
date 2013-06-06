/**
@file Ammo.cpp

@see Logic::IAmmo
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "Ammo.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"

// Graficos
// @deprecated Ogre no deberia estar acoplado a la logica
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

using namespace std;

namespace Logic {
	
	IAmmo::IAmmo(const string& weaponName) : _weaponName("weapon" + weaponName),
											 _currentAmmo(0),
											 _primaryFireIsActive(false),
											 _secondaryFireIsActive(false) {

		// Nada que inicializar
	}

	//__________________________________________________________________

	IAmmo::~IAmmo() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool IAmmo::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_weaponName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_weaponName + "ID") );
		assert( entityInfo->hasAttribute("physic_radius") );
		assert( entityInfo->hasAttribute("heightShoot") );

		// Leemos los atributos obligatorios de arma
		_weaponID = (WeaponType::Enum)entityInfo->getIntAttribute(_weaponName + "ID");
		_maxAmmo = entityInfo->getIntAttribute(_weaponName + "MaxAmmo");
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");

		return true;
	}

	//__________________________________________________________________

	bool IAmmo::accept(const shared_ptr<CMessage>& message) {
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

	//__________________________________________________________________

	void IAmmo::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
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

				break;
			}
		}
	}

	//__________________________________________________________________

	void IAmmo::primaryFire() {
		_primaryFireIsActive = true;
		// Mandar el mensaje primaryFire(true)
	}

	//__________________________________________________________________

	void IAmmo::secondaryFire() {
		_secondaryFireIsActive = true;
		// Mandar el mensaje seondaryFire(true)
	}

	//__________________________________________________________________

	void IAmmo::stopPrimaryFire() {
		_primaryFireIsActive = true;
		// Mandar el mensaje primaryFire(false)
	}

	//__________________________________________________________________

	void IAmmo::stopSecondaryFire() {
		_secondaryFireIsActive = false;
		// Mandar el mensaje secondaryFire(false)
	}

	//__________________________________________________________________

	void IAmmo::onAvailable() {
		shared_ptr<CMessageHudWeapon> message = make_shared<CMessageHudWeapon>();
		message->setWeapon(_weaponID);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);
	}

	//__________________________________________________________________

	void IAmmo::emitSound(const string &ruta, const string &sound, bool notIfPlay){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setRuta(ruta);
		audioMsg->setId(sound);
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(notIfPlay);
		audioMsg->setIsPlayer( _entity->isPlayer() );
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________
	
	void IAmmo::addAmmo(int weapon, int ammo, bool iAmCatch) {
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

	void IAmmo::decrementAmmo(unsigned int ammoSpent) {
		_currentAmmo -= ammoSpent;

		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		shared_ptr<CMessageHudAmmo> message = make_shared<CMessageHudAmmo>();
		message->setAmmo(_currentAmmo);

		// Cambio sobre uno, hay q cambiarlo ;-)
		message->setWeapon(_weaponID);
		_entity->emitMessage(message);
	}

	//__________________________________________________________________

	void IAmmo::resetAmmo() {
		_currentAmmo = 0;
	}

	//__________________________________________________________________

	void IAmmo::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
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

} // namespace Logic

