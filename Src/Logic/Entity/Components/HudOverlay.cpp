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

		int height = overlayManager.getViewportHeight();
		int width = overlayManager.getViewportWidth();

		int relativeWidth = width/26;
		int relativeHeight = height/31;

		////////////////Todo esto para la mira
         // Create an overlay

         // Create a panel
         Ogre::OverlayContainer* panelMira = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "Mira" ) );
		 float sizeCrossFire = entityInfo->getFloatAttribute("hudMira");
		 float positionCrossFire = 0.5f-((sizeCrossFire/2)/100) ;
         panelMira->setPosition( positionCrossFire,positionCrossFire);
		 panelMira->setDimensions( sizeCrossFire/100, sizeCrossFire/100 );
         panelMira->setMaterialName("hudMira");
         // Add the panel to the overlay

		////// panel PanelWeapon1
		Ogre::OverlayContainer* panelWeapon1 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelWeapon1" ) );
		panelWeapon1->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon1->setPosition( 9*relativeWidth, 29*relativeHeight);
		panelWeapon1->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon1->setMaterialName("cuadroArmas");

		overlay->add2D( panelWeapon1 );

		////// panel PanelWeapon1
		Ogre::OverlayContainer* panelWeapon2 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelWeapon2" ) );
		panelWeapon2->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon2->setPosition( 11*relativeWidth, 29*relativeHeight);
		panelWeapon2->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon2->setMaterialName("cuadroArmas");

		overlay->add2D( panelWeapon2 );

		///// panel PanelWeapon1
		Ogre::OverlayContainer* panelWeapon3 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon3" ) );
		panelWeapon3->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon3->setPosition( 13*relativeWidth, 29*relativeHeight);
		panelWeapon3->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon3->setMaterialName("cuadroArmas");
		overlay->add2D( panelWeapon3 );

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

				Ogre::TextAreaOverlayElement* textAreaPanelHealth = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelHealth"));
				
				
				
				textAreaPanelHealth->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaPanelHealth->setPosition(-relativeWidth*1.5, 0);
				textAreaPanelHealth->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream ss;//create a stringstream
					ss << _health;//add number to the stream
				textAreaPanelHealth->setCaption(ss.str());
				textAreaPanelHealth->setCharHeight(46);
				textAreaPanelHealth->setFontName("fuenteSimple");
				textAreaPanelHealth->setColour(Ogre::ColourValue::White);

				//panelShield->addChild(textAreaHealth);
				
				panelHealth->addChild(textAreaPanelHealth);

		overlay->add2D( panelHealth );
         // Add the panel to the overlay
        
		
		///// panel PanelWeapon1
		Ogre::OverlayContainer* panelShield = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelShield" ) );
		panelShield->setMetricsMode(Ogre::GMM_PIXELS);
		panelShield->setPosition( 7*relativeWidth, 29*relativeHeight);
		panelShield->setDimensions( relativeWidth*1.5, relativeHeight*1.5 );
        panelShield->setMaterialName("hudShield");


				Ogre::TextAreaOverlayElement* textAreaPanelShield = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelShield"));
				
				textAreaPanelShield->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaPanelShield->setPosition(-relativeWidth*1.5, 0);
				textAreaPanelShield->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream ss;//create a stringstream
					ss << _shield;//add number to the stream
				textAreaPanelShield->setCaption(ss.str());
				textAreaPanelShield->setCharHeight(46);
				textAreaPanelShield->setFontName("fuenteSimple");
				textAreaPanelShield->setColour(Ogre::ColourValue::White);

				panelShield->addChild(textAreaPanelShield);
		overlay->add2D( panelShield );
         // Add the panel to the overlay

		/*
			/////////// Texto de salud
		
				Ogre::TextAreaOverlayElement* textAreaShield = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "TextAreaShield"));
				
				
				
				textAreaShield->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaShield->setPosition(0.5, 0.5);
				textAreaShield->setDimensions(10, 10);
				textAreaShield->setCaption("pepe");
				textAreaShield->setCharHeight(16);
				textAreaShield->setFontName("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				panelShield->addChild(textAreaShield);

				Ogre::TextAreaOverlayElement* textAreaShield3 = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "TextAreaShield3"));
				
				
				
				textAreaShield3->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaShield3->setPosition(0.5, 0.5);
				textAreaShield3->setDimensions(10, 10);
				textAreaShield3->setCaption("pepeppepepe 3");
				textAreaShield3->setCharHeight(16);
				textAreaShield3->setFontName("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				panelShield->addChild(textAreaShield3);
				
				

						////// panel Shield2
		Ogre::OverlayContainer* panelShield2 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "Shield2" ) );
		 
		 float hudShield2X = entityInfo->getFloatAttribute("hudShield2X");
		 float hudShield2Y = entityInfo->getFloatAttribute("hudShield2Y");
		 float hudShield2Size = entityInfo->getFloatAttribute("hudShield2Size");
		 
         panelShield2->setPosition( hudShield2X,hudShield2Y);
		 panelShield2->setDimensions( hudShield2Size, hudShield2Size );
         panelShield2->setMaterialName("hudHealth");
         // Add the panel to the overlay
        
		 
			/////////// Texto de salud
		
				Ogre::TextAreaOverlayElement* textAreaShield2 = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "TextAreaShield2"));
				textAreaShield2->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaShield2->setPosition(0.5, 0.5);
				textAreaShield2->setDimensions(10, 10);
				textAreaShield2->setCaption("venga......");
				textAreaShield2->setCharHeight(16);
				textAreaShield2->setFontName("fuenteSimple");

				//panelShield2->addChild(textAreaHealth);
				panelShield2->addChild(textAreaShield2);



		

				
		 
Ogre::OverlayContainer* panelHealth = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "Health" ) );
		 
		 float hudHealthX = entityInfo->getFloatAttribute("hudHealthX");
		 float hudHealthY = entityInfo->getFloatAttribute("hudHealthY");
		 float hudHealthSize = entityInfo->getFloatAttribute("hudHealthSize");
		 
         panelHealth->setPosition( hudHealthX,hudHealthY);
		 panelHealth->setDimensions( hudHealthSize, hudHealthSize );
         panelHealth->setMaterialName("hudHealth");
         // Add the panel to the overlay
        
		 
			/////////// Texto de salud
		
				Ogre::TextAreaOverlayElement* textAreaHealth = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "TextAreaHealth"));
				textAreaHealth->setMetricsMode(Ogre::GMM_PIXELS);
				textAreaHealth->setPosition(0.5, 0.5);
				textAreaHealth->setDimensions(10, 10);
				textAreaHealth->setCaption("juanito");
				textAreaHealth->setCharHeight(16);
				textAreaHealth->setFontName("fuenteSimple");

				//panelHealth->addChild(textAreaHealth);
				panelHealth->addChild(textAreaHealth);
			
			
         // Show the overlay
          
		  overlay->add2D( panelMira );
		
		  overlay->add2D( panelShield );
		   overlay->add2D( panelShield2 );
		    overlay->add2D( panelHealth );
			*/

		 overlay->show();

		 

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(CMessage *message)
	{
		//return message->getMessageType() == Message::DAMAGED;
		return false;

	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{
		switch(message->getMessageType())
		{

		}

	} // process


} // namespace Logic

