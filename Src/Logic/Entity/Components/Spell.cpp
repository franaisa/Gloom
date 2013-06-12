/**
@file Spell.cpp

Contiene la implementación de la interfaz común
a todas las armas.

@see Logic::ISpell
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "Spell.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudAmmo.h"
//#include "Logic/Messages/MessageHudSpell.h"
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
	
	ISpell::ISpell(const string& spellName) : _spellName("spell" + spellName),
												 _currentAmmo(0) {

		// Nada que inicializar
	}

	//__________________________________________________________________

	ISpell::~ISpell() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpell::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_spellName + "MaxAmmo") );
		assert( entityInfo->hasAttribute(_spellName + "ID") );
		assert( entityInfo->hasAttribute("physic_radius") );
		assert( entityInfo->hasAttribute("heightShoot") );

		// Leemos los atributos obligatorios de arma
		_spellID = (SpellType::Enum)entityInfo->getIntAttribute(_spellName + "ID");
		_maxAmmo = entityInfo->getIntAttribute(_spellName + "MaxAmmo");
		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");

		return true;
	}

	//__________________________________________________________________

	bool ISpell::accept(const shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		return	message->getMessageType() == Message::PRIMARY_SHOOT ||
				message->getMessageType() == Message::SECONDARY_SHOOT;
	}

	//__________________________________________________________________

	void ISpell::process(const shared_ptr<CMessage>& message) {
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

	void ISpell::onAvailable() {

	}

	//__________________________________________________________________

	void ISpell::emitSound(const string &ruta, const string &sound, bool notIfPlay){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setRuta(ruta);
		audioMsg->setId(sound);
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(notIfPlay);
		audioMsg->setIsPlayer( _entity->isPlayer() );
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________
	
	void ISpell::addAmmo(int spell, int ammo, bool iAmCatch) {
		_currentAmmo += ammo;
		if(_currentAmmo > _maxAmmo) 
			_currentAmmo = _maxAmmo;

		/*
		if(iAmCatch) {
			std::shared_ptr<CMessageHudAmmo> message = std::make_shared<CMessageHudAmmo>();
			message->setSpell(_spellID);
			message->setAmmo(_currentAmmo);
			_entity->emitMessage(message);
		}
		*/
	} // addAmmo

	//__________________________________________________________________

	void ISpell::decrementAmmo(unsigned int ammoSpent) {
		_currentAmmo -= ammoSpent;

		/*
		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		shared_ptr<CMessageHudAmmo> message = make_shared<CMessageHudAmmo>();
		message->setAmmo(_currentAmmo);

		// Cambio sobre uno, hay q cambiarlo ;-)
		message->setSpell(_spellID);
		_entity->emitMessage(message);
		*/
	}

	//__________________________________________________________________

	void ISpell::resetAmmo() {
		_currentAmmo = 0;
	}

	//__________________________________________________________________

	void ISpell::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
		OgreDecal::OgreMesh worldMesh;
 
		/// This method will extract all of the triangles from the mesh to be used later. Only should be called once.
		/// If you scale your mesh at all, pass it in here.

		shared_ptr<CMessageDecal> message = make_shared<CMessageDecal>();
		message->setPosition(vPos);
		_entity->emitMessage(message);
		//std::cout << "emito el mensaje" << std::endl;

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

