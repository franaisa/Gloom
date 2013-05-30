/**
@file Weapon.cpp


@see Logic::IWeapon
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Febrero, 2013
*/

#include "Weapon.h"
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
	
	IWeapon::IWeapon(const string& weaponName) : _weaponName("weapon" + weaponName),
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

		readMustAttributes(entityInfo);
		readOptionalAttributes(entityInfo);

		return true;
	}

	//__________________________________________________________________

	bool IWeapon::accept(const shared_ptr<CMessage>& message) {
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

	void IWeapon::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

				if(type == Control::LEFT_CLICK) {
					if( canUsePrimaryFire() ) {
						primaryFire();

						//_primaryFireTimer = _primaryFireCooldown;
					}
				}
				else if(type == Control::RIGHT_CLICK) {
					if( canUseSecondaryFire() ) {
						secondaryFire();

						//_secondaryFireTimer = _secondaryFireCooldown;
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

	//void IWeapon::onTick(unsigned int msecs) {
		// Controlamos el cooldown del disparo primario y secundario
		/*if(_primaryFireTimer > 0) {
			_primaryFireTimer -= msecs;
			
			if(_primaryFireTimer < 0)
				_primaryFireTimer = 0;
		}

		if(_secondaryFireTimer > 0) {
			_secondaryFireTimer -= msecs;

			if(_secondaryFireTimer < 0)
				_secondaryFireTimer = 0;
		}*/
	//}

	//__________________________________________________________________

	void IWeapon::onAvailable() {
		shared_ptr<CMessageHudWeapon> message = make_shared<CMessageHudWeapon>();
		message->setWeapon(_weaponID);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);
	}

	//__________________________________________________________________

	void IWeapon::emitSound(const string &ruta, const string &sound, bool notIfPlay){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setRuta(ruta);
		audioMsg->setId(sound);
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(notIfPlay);
		audioMsg->setIsPlayer( _entity->isPlayer() );
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________
	
	void IWeapon::addAmmo(int weapon, int ammo, bool iAmCatch) {
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

	void IWeapon::decrementAmmo(unsigned int ammoSpent) {
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

	bool IWeapon::canUsePrimaryFire() {
		//return _primaryFireTimer == 0 && ( (_ammoSpentPerPrimaryShot * _shotsPerPrimaryFire) <= _currentAmmo);
		return true;
	}

	//__________________________________________________________________

	bool IWeapon::canUseSecondaryFire() {
		//return _secondaryFireTimer == 0 && ( (_ammoSpentPerSecondaryShot * _shotsPerSecondaryFire) <= _currentAmmo);
		return true;
	}

	//__________________________________________________________________

	void IWeapon::resetAmmo() {
		_currentAmmo = 0;
	}

	//__________________________________________________________________

	void IWeapon::amplifyDamage(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto
		/*if(percentage == 0) {
			_primaryFireDamage = _defaultPrimaryFireDamage;
			_secondaryFireDamage = _defaultSecondaryFireDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireDamage += percentage * _primaryFireDamage * 0.01f;
			_secondaryFireDamage += percentage * _secondaryFireDamage * 0.01f;
		}*/
	}

	//__________________________________________________________________

	void IWeapon::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto
		/*if(percentage == 0) {
			_primaryFireCooldown = _defaultPrimaryFireCooldown;
			_secondaryFireCooldown = _defaultSecondaryFireCooldown;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireCooldown -= percentage * _primaryFireCooldown * 0.01f;
			_secondaryFireCooldown -= percentage * _secondaryFireCooldown * 0.01f;
		}*/
	}

	//__________________________________________________________________

	void IWeapon::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
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

	void IWeapon::readMustAttributes(const Map::CEntity* entityInfo) {
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
	}
	
	//__________________________________________________________________

	void IWeapon::readOptionalAttributes(const Map::CEntity* entityInfo) {
		// Cooldown, por defecto es 0
		/*if( entityInfo->hasAttribute(_weaponName + "PrimaryFireCooldown") )
			_defaultPrimaryFireCooldown =  _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireCooldown")  * 1000;
		if( entityInfo->hasAttribute(_weaponName + "SecondaryFireCooldown") )
			_defaultSecondaryFireCooldown = _secondaryFireCooldown = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireCooldown") * 1000;*/
		
		// Daño. No todos los modos de disparo tienen porque hacer daño. Por defecto es 0.
		/*if( entityInfo->hasAttribute(_weaponName + "PrimaryFireDamage") )
			_defaultPrimaryFireDamage = _primaryFireDamage = entityInfo->getIntAttribute(_weaponName + "PrimaryFireDamage");
		if( entityInfo->hasAttribute(_weaponName + "SecondaryFireDamage") )
			_defaultSecondaryFireDamage = _secondaryFireDamage = entityInfo->getIntAttribute(_weaponName + "SecondaryFireDamage");*/

		// Munición por disparo
		/*if( entityInfo->hasAttribute(_weaponName + "AmmoSpentPerPrimaryShot") )
			_ammoSpentPerPrimaryShot = entityInfo->getIntAttribute(_weaponName + "AmmoSpentPerPrimaryShot");
		if( entityInfo->hasAttribute(_weaponName + "AmmoSpentPerSecondaryShot") )
			_ammoSpentPerSecondaryShot = entityInfo->getIntAttribute(_weaponName + "AmmoSpentPerSecondaryShot");*/

		// Disparos por click
		/*if( entityInfo->hasAttribute(_weaponName + "ShotsPerPrimaryFire") )
			_shotsPerPrimaryFire = entityInfo->getIntAttribute(_weaponName + "ShotsPerPrimaryFire");
		if( entityInfo->hasAttribute(_weaponName + "ShotsPerSecondaryFire") )
			_shotsPerSecondaryFire = entityInfo->getIntAttribute(_weaponName + "ShotsPerSecondaryFire");*/

		/*if( entityInfo->hasAttribute(_weaponName + "ShotsDistance") )
			_shotsDistance = entityInfo->getFloatAttribute(_weaponName + "ShotsDistance");*/
	}

} // namespace Logic

