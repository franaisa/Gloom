/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "HudOverlay.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"

#include <OgreSceneManager.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreColourValue.h>
#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>


#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"

namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;


		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		Ogre::Overlay* overlay = overlayManager.create( "HudOverlay" );

		float height = overlayManager.getViewportHeight();
		float width = overlayManager.getViewportWidth();

		float relativeWidth = width/26;
		float relativeHeight = height/31;

		////////////////Todo esto para la mira
         // Create an overlay

         // Create a panel de Mira
         Ogre::OverlayContainer* panelMira = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "Mira" ) );
		 float sizeCrossFire = entityInfo->getFloatAttribute("hudMira");
		 float positionCrossFire = 0.5f-((sizeCrossFire/2)/100) ;
         panelMira->setPosition( positionCrossFire,positionCrossFire);
		 panelMira->setDimensions( sizeCrossFire/100, sizeCrossFire/100 );
         panelMira->setMaterialName("hudMira");

		 overlay->add2D( panelMira );
         // Add the panel to the overlay

		
		 
		 ////// panel PanelWeapon cuadro principal
		Ogre::OverlayContainer* panelWeapon1 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelWeapon1" ) );
		panelWeapon1->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon1->setPosition( 9*relativeWidth, 29*relativeHeight);
		panelWeapon1->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon1->setMaterialName("cuadroArmas");

		overlay->add2D( panelWeapon1 );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[HAMMER][ACTIVE]" ) );
			_weaponsBox[HAMMER][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[HAMMER][ACTIVE]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][ACTIVE]->setMaterialName("katana");

			overlay->add2D( _weaponsBox[0][0] );


		////// panel PanelWeapon2 cuadro principal
		Ogre::OverlayContainer* panelWeapon2 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon2" ) );
		panelWeapon2->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon2->setPosition( 11*relativeWidth, 29*relativeHeight);
		panelWeapon2->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon2->setMaterialName("cuadroArmas");

		overlay->add2D( panelWeapon2 );

			 ////// panel PanelWeapon2 activa
			_weaponsBox[SNIPER][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][ACTIVE]" ) );
			_weaponsBox[SNIPER][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][ACTIVE]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][ACTIVE]->setMaterialName("francotirador");

			overlay->add2D( _weaponsBox[SNIPER][ACTIVE] );

			 ////// panel PanelWeapon2 BN
			_weaponsBox[SNIPER][NO_AMMO] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][NO_AMMO]" ) );
			_weaponsBox[SNIPER][NO_AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][NO_AMMO]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_AMMO]->setMaterialName("francotiradorBN");

			overlay->add2D( _weaponsBox[SNIPER][NO_AMMO] );


			 ////// panel PanelWeapon2 SinBalas
			_weaponsBox[SNIPER][NO_WEAPON] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][NO_WEAPON]" ) );
			_weaponsBox[SNIPER][NO_WEAPON]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][NO_WEAPON]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_WEAPON]->setMaterialName("francotiradorSinBalas");

			overlay->add2D( _weaponsBox[SNIPER][NO_WEAPON] );



		///// panel PanelWeapon3
		Ogre::OverlayContainer* panelWeapon3 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon3" ) );
		panelWeapon3->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon3->setPosition( 13*relativeWidth, 29*relativeHeight);
		panelWeapon3->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon3->setMaterialName("cuadroArmas");
		overlay->add2D( panelWeapon3 );

				///// panel PanelWeapon3 Active
			_weaponsBox[SHOTGUN][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][ACTIVE]" ) );
			_weaponsBox[SHOTGUN][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][ACTIVE]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][ACTIVE]->setMaterialName("escopeta");
			overlay->add2D( _weaponsBox[SHOTGUN][ACTIVE] );

			
				///// panel PanelWeapon3 no ammo
			_weaponsBox[SHOTGUN][NO_AMMO] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][NO_AMMO]" ) );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][NO_AMMO]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMaterialName("escopetaBN");
			overlay->add2D( _weaponsBox[SHOTGUN][NO_AMMO] );
			
				///// panel PanelWeapon3 no weapon
			_weaponsBox[SHOTGUN][NO_WEAPON] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][NO_WEAPON]" ) );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][NO_WEAPON]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMaterialName("escopetaSinBalas");
			overlay->add2D( _weaponsBox[SHOTGUN][NO_WEAPON] );

			

			

		///// panel PanelWeapon1
		Ogre::OverlayContainer* panelWeapon4 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon4" ) );
		panelWeapon4->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon4->setPosition( 15*relativeWidth, 29*relativeHeight);
		panelWeapon4->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon4->setMaterialName("cuadroArmas");

		Ogre::TextAreaOverlayElement* textAreaDummy = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaDummy"));
				
				
				
				textAreaDummy->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaDummy->setPosition(0.5, 0.5);
				textAreaDummy->setDimensions(10, 10);
				textAreaDummy->setCaption("pepe");
				textAreaDummy->setCharHeight(16);
				textAreaDummy->setFontName("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				panelWeapon4->addChild(textAreaDummy);

		overlay->add2D( panelWeapon4 );




		
		///// panel PanelWeapon1
		Ogre::OverlayContainer* panelHealth = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelHealth" ) );
		panelHealth->setMetricsMode(Ogre::GMM_PIXELS);
		panelHealth->setPosition( 3*relativeWidth, 29*relativeHeight);
		panelHealth->setDimensions( relativeWidth*1.5, relativeHeight*1.5 );
        panelHealth->setMaterialName("hudHealth");

				_healthTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelHealth"));
				
				
				
				_healthTextArea->setMetricsMode(Ogre::GMM_PIXELS);
				_healthTextArea->setPosition(-relativeWidth*1.5, 0);
				_healthTextArea->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream sHealth;//create a stringstream
					sHealth << _health;//add number to the stream
				_healthTextArea->setCaption(sHealth.str());
				_healthTextArea->setCharHeight(46);
				_healthTextArea->setFontName("fuenteSimple");
				_healthTextArea->setColour(Ogre::ColourValue::White);

				//panelShield->addChild(textAreaHealth);
				
				panelHealth->addChild(_healthTextArea);

		overlay->add2D( panelHealth );
         // Add the panel to the overlay
        
		
		///// panel PanelWeapon1
		Ogre::OverlayContainer* panelShield = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelShield" ) );
		panelShield->setMetricsMode(Ogre::GMM_PIXELS);
		panelShield->setPosition( 7*relativeWidth, 29*relativeHeight);
		panelShield->setDimensions( relativeWidth*1.5, relativeHeight*1.5 );
        panelShield->setMaterialName("hudShield");


				_shieldTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelShield"));
				
				_shieldTextArea->setMetricsMode(Ogre::GMM_PIXELS);
				_shieldTextArea->setPosition(-relativeWidth*1.5, 0);
				_shieldTextArea->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream sShield;//create a stringstream
					sShield << _shield;//add number to the stream
				_shieldTextArea->setCaption(sShield.str());
				_shieldTextArea->setCharHeight(46);
				_shieldTextArea->setFontName("fuenteSimple");
				_shieldTextArea->setColour(Ogre::ColourValue::White);

				panelShield->addChild(_shieldTextArea);
		overlay->add2D( panelShield );

		 overlay->show();

		 

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(CMessage *message)
	{
		return message->getMessageType() == Message::HUD_LIFE
			|| message->getMessageType() == Message::HUD_SHIELD;

	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::HUD_LIFE: addLife(((CMessageHudLife*)message)->getHudLife());
			break;
		case Message::HUD_SHIELD: addShield(((CMessageHudShield*)message)->getHudShield());
			break;
		}

	} // process

	void CHudOverlay::addLife( int health){
		_health = health;
		std::stringstream sHealth;
		sHealth << _health;
		_healthTextArea->setCaption(sHealth.str());
	}

	void CHudOverlay::addShield( int shield){
		_shield = shield;

		std::stringstream sShield;
		sShield << _shield;
		_shieldTextArea->setCaption(sShield.str());
	}


} // namespace Logic

