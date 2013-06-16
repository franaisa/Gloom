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

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageDecal.h"


#include "Logic/Messages/MessagePrimaryShoot.h"
#include "Logic/Messages/MessageSecondaryShoot.h"


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
_bDecalsCreated = false;
_primerDecal = true;
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

	bool IWeapon::accept(const shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		return	message->getMessageType() == Message::PRIMARY_SHOOT ||
				message->getMessageType() == Message::SECONDARY_SHOOT;
	}

	//__________________________________________________________________

	void IWeapon::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SHOOT: {
				auto primaryShootMsg = static_pointer_cast<CMessagePrimaryShoot>(message);
				if(primaryShootMsg->getShoot())
					primaryFire();
				else
					stopPrimaryFire();
				break;
			}
			case Message::SECONDARY_SHOOT: {
				auto secondaryShootMsg = static_pointer_cast<CMessageSecondaryShoot>(message);
				if(secondaryShootMsg ->getShoot())
					secondaryFire();
				else
					stopSecondaryFire();
				break;
			}
		}
	}

	//__________________________________________________________________

	void IWeapon::onAvailable() {

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

		std::cout << "emito el mensaje" << std::endl;

		/*
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
			if (!_bDecalsCreated)
			{
				decalObject = cGraph->getSceneManager()->createManualObject();
				_bDecalsCreated = true;
			}
			else
			{
				int a;
			}
			OgreDecal::Decal decal = generator.createDecal( &worldMesh, pos, width, height, textureName, true, decalObject );
			std::cout << "ha creado el decal" << std::endl;
			/// Render the decal object. Always verify the returned object - it will be NULL if no decal could be created.
			if ((decal.object) && (_primerDecal)) {
				//sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
				cGraph->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject( decal.object );
				//decalObject = NULL;
				std::cout << "entraaaaaa" << std::endl;
				_primerDecal = false;
			}
			std::cout << "pinta" << std::endl;*/
	}
} // namespace Logic

