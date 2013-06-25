/**
@file WeaponFeedback.cpp

@see Logic::IWeaponFeedback
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "WeaponFeedback.h"
#include "HudWeapons.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudWeapon.h"
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
	
	IWeaponFeedback::IWeaponFeedback(const string& weaponName) : _weaponName("weapon" + weaponName),
																 _primaryFireIsActive(false),
																 _secondaryFireIsActive(false) {
		// Nada que inicializar
	}

	//__________________________________________________________________

	IWeaponFeedback::~IWeaponFeedback() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool IWeaponFeedback::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		_weaponSound = entityInfo->getStringAttribute(_weaponName+"Audio");

		_hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		assert(_hudWeapon != NULL && "Error: El cliente necesita tener un componente de grafico del arma");

		return true;
	}

	//__________________________________________________________________

	bool IWeaponFeedback::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PRIMARY_SHOOT ||
			   msgType == Message::SECONDARY_SHOOT;
	}

	//__________________________________________________________________

	void IWeaponFeedback::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SHOOT: {
				shared_ptr<CMessagePrimaryShoot> primaryShootMsg = static_pointer_cast<CMessagePrimaryShoot>(message);

				if( primaryShootMsg->getShoot() ) {
					primaryFire();
					_primaryFireIsActive = true;
				}
				else {
					stopPrimaryFire();
					_primaryFireIsActive = false;
				}

				break;
			}
			case Message::SECONDARY_SHOOT: {
				shared_ptr<CMessageSecondaryShoot> secondaryShootMsg = static_pointer_cast<CMessageSecondaryShoot>(message);

				if( secondaryShootMsg->getShoot() ) {
					secondaryFire();
					_secondaryFireIsActive = true;
				}
				else {
					stopSecondaryFire();
					_secondaryFireIsActive = false;
				}

				break;
			}
		}
	} // process
	//__________________________________________________________________

	void IWeaponFeedback::onDeactivate(){
		if(_primaryFireIsActive) {
			stopPrimaryFire();
			_primaryFireIsActive = false;
		}
		if(_secondaryFireIsActive) {
			stopSecondaryFire();
			_secondaryFireIsActive = false;
		}
	} // onDeactivate
	//__________________________________________________________________

	void IWeaponFeedback::emitSound(const string &soundName, bool loopSound, bool play3d, bool streamSound, bool stopSound){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setAudioName(soundName);
		audioMsg->isLoopable(loopSound);
		audioMsg->is3dSound(play3d);
		audioMsg->streamSound(streamSound);
		audioMsg->stopSound(stopSound);
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________

	void IWeaponFeedback::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
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

