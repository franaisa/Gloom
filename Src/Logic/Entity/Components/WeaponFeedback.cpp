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
#include "DynamicParticleSystem.h"
#include "HudWeapons.h"

// Mapa
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Server.h"
// Mensajes
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessagePrimaryShoot.h"
#include "Logic/Messages/MessageSecondaryShoot.h"
#include "Logic/Messages/MessageBlockShoot.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include "Logic/Messages/MessageParticleStart.h"

#include "BaseSubsystems/Euler.h"

// Graficos
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

// @deprecated Ogre no deberia estar acoplado a la logica
#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

using namespace std;

namespace Logic {
	
	IWeaponFeedback::IWeaponFeedback(const string& weaponName) : _weaponName(weaponName),
																 _ableToShoot(true),
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

		
		
		Map::CEntity *tempEntity = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);
		_weaponSound = tempEntity->getStringAttribute("Audio");

		/*
		if(tempEntity->hasAttribute("PrimaryFireParticle"))
			_primaryFireParticle = tempEntity->getStringAttribute("PrimaryFireParticle");
		else
			_primaryFireParticle = "fogonazo";

		if(tempEntity->hasAttribute("SecondaryFireParticle"))
			_secondaryFireParticle = tempEntity->getStringAttribute("SecondaryFireParticle");
		else
			_secondaryFireParticle = "fogonazo";
		*/
		if(tempEntity->hasAttribute("ParticlePosition"))
			_particlePosition = tempEntity->getVector3Attribute("ParticlePosition");
		else
			_particlePosition = Vector3(0, 0, 0);


		_hudWeapon = _entity->getComponent<CHudWeapons>("CHudWeapons");
		assert(_hudWeapon != NULL && "Error: El cliente necesita tener un componente de grafico del arma");

		assert( entityInfo->hasAttribute("heightShoot") );
		_heightShoot = entityInfo->getFloatAttribute("heightShoot");

		return true;
	}

	//__________________________________________________________________

	bool IWeaponFeedback::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PRIMARY_SHOOT	||
			   msgType == Message::SECONDARY_SHOOT	||
			   msgType == Message::BLOCK_SHOOT;
	}

	//__________________________________________________________________

	void IWeaponFeedback::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SHOOT: {
				if(_ableToShoot) {
					shared_ptr<CMessagePrimaryShoot> primaryShootMsg = static_pointer_cast<CMessagePrimaryShoot>(message);

					if( primaryShootMsg->getShoot() ) {
						primaryFire();
						_primaryFireIsActive = true;
						//emitParticle(true);
					}
					else {
						stopPrimaryFire();
						_primaryFireIsActive = false;
					}
				}

				break;
			}
			case Message::SECONDARY_SHOOT: {
				if(_ableToShoot) {
					shared_ptr<CMessageSecondaryShoot> secondaryShootMsg = static_pointer_cast<CMessageSecondaryShoot>(message);

					if( secondaryShootMsg->getShoot() ) {
						secondaryFire();
						_secondaryFireIsActive = true;
						//emitParticle(false);
					}
					else {
						stopSecondaryFire();
						_secondaryFireIsActive = false;
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
	//__________________________________________________________________

	void IWeaponFeedback::emitParticle(bool primaryShoot){
		std::shared_ptr<CMessageCreateParticle> particleMsg = std::make_shared<CMessageCreateParticle>();
		particleMsg->setParticle( primaryShoot?_primaryFireParticle:_secondaryFireParticle );

		Vector3 particlePosition = _entity->getPosition();
		particlePosition.y += _heightShoot;
		//std::cout << "posicionOrig = " << position2 << std::endl;


		Vector3 orientation= _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
		orientation.normalise();
		/*
		float fOffset = 8.0f;
		orientation *= fOffset;
		particlePosition += orientation;
		/*/
		orientation *= _particlePosition;
		particlePosition += orientation;
		/* */

		particleMsg->setPosition(particlePosition);
		_entity->emitMessage(particleMsg);
	} // emitParticle
	//__________________________________________________________________

	void IWeaponFeedback::emitParticle2(bool primaryShoot){
		_currentPaticle = CEntityFactory::getSingletonPtr()->createEntity(
			CEntityFactory::getSingletonPtr()->getInfo(_weaponName+(primaryShoot?"PrimaryShot":"SecondaryShot")),			
			Logic::CServer::getSingletonPtr()->getMap());

		Graphics::CEntity* graphicWeapon = _hudWeapon->getCurrentWeapon();
		CDynamicParticleSystem* particleComp = _currentPaticle->getComponent<CDynamicParticleSystem>("CDynamicParticleSystem");
		
		particleComp->setGraphicParent(graphicWeapon);
		particleComp->setOffset(_particlePosition);

		_currentPaticle->activate();
		_currentPaticle->start();
	} // emitParticle2
	
} // namespace Logic

