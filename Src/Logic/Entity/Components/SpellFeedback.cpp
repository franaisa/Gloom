/**
@file SpellFeedback.cpp

@see Logic::ISpellFeedback
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SpellFeedback.h"
#include "HudWeapons.h"
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessagePrimarySpell.h"
#include "Logic/Messages/MessageSecondarySpell.h"

// Graficos
// @deprecated Ogre no deberia estar acoplado a la logica
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

using namespace std;

namespace Logic {
	
	ISpellFeedback::ISpellFeedback(const string& spellName) : _spellName("spell" + spellName),
															_spellID(-1),
															_isPrimarySpell(false),
															_isPassive(false) {
	}

	//__________________________________________________________________

	ISpellFeedback::~ISpellFeedback() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpellFeedback::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		_hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		assert(_hudWeapon != NULL && "Error: El cliente necesita tener un componente de grafico del arma");

		_spellID = (SpellType::Enum)entityInfo->getIntAttribute(_spellName + "ID");
		return true;
	}

	//__________________________________________________________________

	bool ISpellFeedback::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PRIMARY_SPELL ||
			   msgType == Message::SECONDARY_SPELL;
	}

	//__________________________________________________________________

	void ISpellFeedback::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SPELL: {
				shared_ptr<CMessagePrimarySpell> primarySpellMsg = static_pointer_cast<CMessagePrimarySpell>(message);
				if(!_isPassive && _isPrimarySpell){
					if( primarySpellMsg->getSpell() )
						spellCast();
					else
						stopSpellCast();
				}
				break;
			}
			case Message::SECONDARY_SPELL: {
				shared_ptr<CMessageSecondarySpell> secondarySpellMsg = static_pointer_cast<CMessageSecondarySpell>(message);
				if(!_isPassive && !_isPrimarySpell){
					if( secondarySpellMsg->getSpell() )
						spellCast();
					else
						stopSpellCast();
				}
				break;
			}
		}
	}

	//__________________________________________________________________

	void ISpellFeedback::emitSound(const string &ruta, const string &sound, bool notIfPlay){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setRuta(ruta);
		audioMsg->setId(sound);
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(notIfPlay);
		audioMsg->setIsPlayer( _entity->isPlayer() );
			
		_entity->emitMessage(audioMsg);
	} // emitSound

	//__________________________________________________________________

	void ISpellFeedback::drawDecal(Logic::CEntity* pEntity, Vector3 vPos) {
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

