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


#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudSpawn.h"

namespace Logic 
{
	IMP_FACTORY(CHudOverlay);
	
	//---------------------------------------------------------

	bool CHudOverlay::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		
		_numWeapons = entityInfo->getIntAttribute("numWeapons");

		int _actualWeapon;

		

		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		_overlayPlay = overlayManager.create( "_overlayPlay" );

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

		 _overlayPlay->add2D( panelMira );
         // Add the panel to the overlay

		
		 
		 ////// panel PanelWeapon cuadro principal
		Ogre::OverlayContainer* panelWeapon1 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "PanelWeapon1" ) );
		panelWeapon1->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon1->setPosition( 9*relativeWidth, 29*relativeHeight);
		panelWeapon1->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon1->setMaterialName("cuadroArmas");

		_overlayPlay->add2D( panelWeapon1 );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[HAMMER][ACTIVE]" ) );
			_weaponsBox[HAMMER][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[HAMMER][ACTIVE]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][ACTIVE]->setMaterialName("katana");

			_overlayPlay->add2D( _weaponsBox[HAMMER][ACTIVE] );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][NO_AMMO] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[HAMMER][NO_AMMO]" ) );
			_weaponsBox[HAMMER][NO_AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[HAMMER][NO_AMMO]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][NO_AMMO]->setMaterialName("katana");

			_overlayPlay->add2D( _weaponsBox[HAMMER][NO_AMMO] );

				 ////// panel PanelWeapon1 con arma activa
			_weaponsBox[HAMMER][NO_WEAPON] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[HAMMER][NO_WEAPON]" ) );
			_weaponsBox[HAMMER][NO_WEAPON]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[HAMMER][NO_WEAPON]->setPosition( 9*relativeWidth, 29*relativeHeight);
			_weaponsBox[HAMMER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[HAMMER][NO_WEAPON]->setMaterialName("katana");

			_overlayPlay->add2D( _weaponsBox[HAMMER][NO_WEAPON] );


		////// panel PanelWeapon2 cuadro principal
		Ogre::OverlayContainer* panelWeapon2 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon2" ) );
		panelWeapon2->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon2->setPosition( 11*relativeWidth, 29*relativeHeight);
		panelWeapon2->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon2->setMaterialName("cuadroArmas");

		_overlayPlay->add2D( panelWeapon2 );

			 ////// panel PanelWeapon2 activa
			_weaponsBox[SNIPER][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][ACTIVE]" ) );
			_weaponsBox[SNIPER][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][ACTIVE]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][ACTIVE]->setMaterialName("francotirador");

			_overlayPlay->add2D( _weaponsBox[SNIPER][ACTIVE] );

			 ////// panel PanelWeapon2 BN
			_weaponsBox[SNIPER][NO_AMMO] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][NO_AMMO]" ) );
			_weaponsBox[SNIPER][NO_AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][NO_AMMO]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_AMMO]->setMaterialName("francotiradorBN");

			_overlayPlay->add2D( _weaponsBox[SNIPER][NO_AMMO] );


			 ////// panel PanelWeapon2 SinBalas
			_weaponsBox[SNIPER][NO_WEAPON] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SNIPER][NO_WEAPON]" ) );
			_weaponsBox[SNIPER][NO_WEAPON]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SNIPER][NO_WEAPON]->setPosition( 11*relativeWidth, 29*relativeHeight);
			_weaponsBox[SNIPER][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SNIPER][NO_WEAPON]->setMaterialName("francotiradorSinBalas");

			_overlayPlay->add2D( _weaponsBox[SNIPER][NO_WEAPON] );



		///// panel PanelWeapon3
		Ogre::OverlayContainer* panelWeapon3 = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelWeapon3" ) );
		panelWeapon3->setMetricsMode(Ogre::GMM_PIXELS);
		panelWeapon3->setPosition( 13*relativeWidth, 29*relativeHeight);
		panelWeapon3->setDimensions( relativeWidth*2, relativeHeight*2 );
        panelWeapon3->setMaterialName("cuadroArmas");
		_overlayPlay->add2D( panelWeapon3 );

				///// panel PanelWeapon3 Active
			_weaponsBox[SHOTGUN][ACTIVE] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][ACTIVE]" ) );
			_weaponsBox[SHOTGUN][ACTIVE]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][ACTIVE]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][ACTIVE]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][ACTIVE]->setMaterialName("escopeta");
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][ACTIVE] );

			
				///// panel PanelWeapon3 no ammo
			_weaponsBox[SHOTGUN][NO_AMMO] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][NO_AMMO]" ) );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][NO_AMMO]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_AMMO]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_AMMO]->setMaterialName("escopetaBN");
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][NO_AMMO] );
			
				///// panel PanelWeapon3 no weapon
			_weaponsBox[SHOTGUN][NO_WEAPON] = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "_weaponsBox[SHOTGUN][NO_WEAPON]" ) );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMetricsMode(Ogre::GMM_PIXELS);
			_weaponsBox[SHOTGUN][NO_WEAPON]->setPosition( 13*relativeWidth, 29*relativeHeight);
			_weaponsBox[SHOTGUN][NO_WEAPON]->setDimensions( relativeWidth*2, relativeHeight*2 );
			_weaponsBox[SHOTGUN][NO_WEAPON]->setMaterialName("escopetaSinBalas");
			_overlayPlay->add2D( _weaponsBox[SHOTGUN][NO_WEAPON] );

			

			

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

		_overlayPlay->add2D( panelWeapon4 );




		
		///// panel health
		Ogre::OverlayContainer* panelHealth = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelHealth" ) );
		panelHealth->setMetricsMode(Ogre::GMM_PIXELS);
		panelHealth->setPosition( 3*relativeWidth, 29*relativeHeight);
		panelHealth->setDimensions( relativeWidth*1.5f, relativeHeight*1.5f );
        panelHealth->setMaterialName("hudHealth");

		_textBoxArea[HEALTH] = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelHealth"));
				
		
				_textBoxArea[HEALTH]->setMetricsMode(Ogre::GMM_PIXELS);
				_textBoxArea[HEALTH]->setPosition(-relativeWidth*1.5, 0);
				_textBoxArea[HEALTH]->setDimensions(relativeWidth*2, relativeHeight*2);

					std::stringstream sHealth;//create a stringstream
					sHealth << _health;//add number to the stream
				_textBoxArea[HEALTH]->setCaption(sHealth.str());
				_textBoxArea[HEALTH]->setCharHeight(46);
				_textBoxArea[HEALTH]->setFontName("fuenteSimple");
				_textBoxArea[HEALTH]->setColour(Ogre::ColourValue::White);

				//panelShield->addChild(textAreaHealth);
				
				panelHealth->addChild(_textBoxArea[HEALTH]);

		_overlayPlay->add2D( panelHealth );
         // Add the panel to the overlay
        
		
		///// panel shield
		Ogre::OverlayContainer* panelShield = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelShield" ) );
		panelShield->setMetricsMode(Ogre::GMM_PIXELS);
		panelShield->setPosition( 7*relativeWidth, 29*relativeHeight);
		panelShield->setDimensions( relativeWidth*1.5f, relativeHeight*1.5f );
        panelShield->setMaterialName("hudShield");


				_textBoxArea[SHIELD] = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelShield"));
				
				_textBoxArea[SHIELD]->setMetricsMode(Ogre::GMM_PIXELS);
				_textBoxArea[SHIELD]->setPosition(-relativeWidth*1.5, 0);
				_textBoxArea[SHIELD]->setDimensions(relativeWidth*2, relativeHeight*2);

					std::stringstream sShield;//create a stringstream
					sShield << _shield;//add number to the stream
				_textBoxArea[SHIELD]->setCaption(sShield.str());
				_textBoxArea[SHIELD]->setCharHeight(46);
				_textBoxArea[SHIELD]->setFontName("fuenteSimple");
				_textBoxArea[SHIELD]->setColour(Ogre::ColourValue::White);

				panelShield->addChild(_textBoxArea[SHIELD]);
		_overlayPlay->add2D( panelShield );



		///// panel AMMO
		Ogre::OverlayContainer* panelAmmo = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelAmmo" ) );
		panelAmmo->setMetricsMode(Ogre::GMM_PIXELS);
		panelAmmo->setPosition( 24*relativeWidth, 29*relativeHeight);
		panelAmmo->setDimensions( relativeWidth*0.5, relativeHeight*1.5 );
        panelAmmo->setMaterialName("hudAmmo");


				_textBoxArea[AMMO] = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "textAreaPanelAmmo"));
				
				_textBoxArea[AMMO]->setMetricsMode(Ogre::GMM_PIXELS);
				_textBoxArea[AMMO]->setPosition(-relativeWidth*2, 0);
				_textBoxArea[AMMO]->setDimensions(relativeWidth*2, relativeHeight*2);
					std::stringstream sAmmo;
					sAmmo << _ammo;
				_textBoxArea[AMMO]->setCaption(sAmmo.str());
				_textBoxArea[AMMO]->setCharHeight(46);
				_textBoxArea[AMMO]->setFontName("fuenteSimple");
				_textBoxArea[AMMO]->setColour(Ogre::ColourValue::White);

				panelAmmo->addChild(_textBoxArea[AMMO]);
		_overlayPlay->add2D( panelAmmo );

		for(int i = 1; i < _numWeapons; ++i){
			_weaponsBox[i][NO_AMMO]->hide();
			_weaponsBox[i][ACTIVE]->hide();
		}
		


		 _overlayPlay->show();




		 //////////////////////////////////////AQUI ME CREO EL OVERLAY PARA CUANDO SE MUERA




		 _overlayDie = overlayManager.create( "_overlayDie" );



		Ogre::OverlayContainer* panelDie = static_cast<Ogre::OverlayContainer*>( overlayManager.createOverlayElement( "Panel", "panelDie" ) );
		panelDie->setMetricsMode(Ogre::GMM_PIXELS);
		panelDie->setPosition( 5*relativeWidth, 12*relativeHeight);
		panelDie->setDimensions( relativeWidth*10, relativeHeight*10 );
        //panelDie->setMaterialName("cuadroArmas");

		_textAreaDie = static_cast<Ogre::TextAreaOverlayElement*>(
				overlayManager.createOverlayElement("TextArea", "_textAreaDie"));
				
				_textAreaDie->setMetricsMode(Ogre::GMM_PIXELS);
				_textAreaDie->setPosition(0.5, 0.5);
				_textAreaDie->setDimensions(10, 10);

				_textAreaDie->setCaption("MUERE");
				_textAreaDie->setCharHeight(46);
				_textAreaDie->setFontName("fuenteSimple");

				//panelShield->addChild(textAreaHealth);
				
				panelDie->addChild(_textAreaDie);

		_overlayDie->add2D( panelDie );

		//_overlayDie->show();

		 

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CHudOverlay::accept(CMessage *message)
	{
		return message->getMessageType() == Message::HUD_LIFE
			|| message->getMessageType() == Message::HUD_SHIELD
			|| message->getMessageType() == Message::HUD_AMMO
			|| message->getMessageType() == Message::HUD_WEAPON
			|| message->getMessageType() == Message::HUD_SPAWN;

	} // accept
	
	//---------------------------------------------------------

	void CHudOverlay::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::HUD_LIFE: hudLife(((CMessageHudLife*)message)->getLife());
			break;
		case Message::HUD_SHIELD: hudShield(((CMessageHudShield*)message)->getShield());
			break;
		case Message::HUD_AMMO: hudAmmo( ((CMessageHudAmmo*)message)->getAmmo(), ((CMessageHudAmmo*)message)->getWeapon());
			break;
		case Message::HUD_WEAPON: hudWeapon(((CMessageHudWeapon*)message)->getAmmo(), ((CMessageHudWeapon*)message)->getWeapon());
			break;
		case Message::HUD_SPAWN: hudSpawn(((CMessageHudSpawn*)message)->getTime());
			break;
		}

	} // process

	void CHudOverlay::hudLife(int health){
		_health = health;
		std::stringstream sHealth;
		sHealth << _health;
		_textBoxArea[HEALTH]->setCaption(sHealth.str());
	}

	void CHudOverlay::hudShield(int shield){
		_shield = shield;

		std::stringstream sShield;
		sShield << _shield;
		_textBoxArea[SHIELD]->setCaption(sShield.str());
	}

	void CHudOverlay::hudAmmo(int ammo, int weapon){

		if(weapon == _actualWeapon)
		{
			_ammo = ammo;
			if(_ammo != 0){
				if(!_weaponsBox[_actualWeapon][ACTIVE]->isVisible())
				{
					_weaponsBox[_actualWeapon][NO_AMMO]->hide();
					_weaponsBox[_actualWeapon][ACTIVE]->show();
				}
			}else{
				if(_weaponsBox[_actualWeapon][ACTIVE]->isVisible())
				{
					_weaponsBox[_actualWeapon][ACTIVE]->hide();
					_weaponsBox[_actualWeapon][NO_AMMO]->show();
				}
			}//fin else	ammo!=0
			std::stringstream sAmmo;
			sAmmo << _ammo;
			_textBoxArea[AMMO]->setCaption(sAmmo.str());
		}//fin weapon == _actualweapon
		else{
			_weaponsBox[weapon][ACTIVE]->show();
			_weaponsBox[weapon][NO_WEAPON]->hide();
			_weaponsBox[weapon][NO_AMMO]->hide();
		}
	}

	void CHudOverlay::hudWeapon(int ammo, int weapon){

		//if(weapon != _actualWeapon && _actualWeapon != 0)
		if(weapon != _actualWeapon)
		{
			_actualWeapon = weapon;
			_weaponsBox[_actualWeapon][ACTIVE]->show();
			_weaponsBox[_actualWeapon][NO_AMMO]->hide();
			_weaponsBox[_actualWeapon][NO_WEAPON]->hide();
		}
		hudAmmo(ammo, weapon);
	}

	void CHudOverlay::hudSpawn(int spawmTime){
		
		if(_overlayPlay->isVisible()){
			_overlayPlay->hide();
		}

		std::stringstream sSpawn;
		sSpawn << "HAS MUERTO, LOSER \n Tiempo de respawn: " << spawmTime;
		_textAreaDie->setCaption(sSpawn.str());
		
		if(!_overlayDie->isVisible())
			_overlayDie->show();
		if(spawmTime <= 0.1){

			_overlayDie->hide();
			//reset para volver a mostrar solo el arma inicial al hacer show
			for(int i=1; i<_numWeapons;++i){
				_weaponsBox[i][NO_WEAPON]->show();
			}
			_overlayPlay->show();
		}

	}

} // namespace Logic

