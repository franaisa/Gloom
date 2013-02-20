/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Antonio Jesus
@date Febrero, 2013
*/
#ifndef __Logic_HudOverlay_H
#define __Logic_HudOverlay_H

#include "Graphics/Server.h"
#include "Logic/Entity/Component.h"

/*
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
*/

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el hud, mediante overlays
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Febrero, 2013
*/
	class CHudOverlay : public IComponent
	{
		DEC_FACTORY(CHudOverlay);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CHudOverlay() : IComponent(), _health(0), _shield(0), _ammo(1), _actualWeapon(0), _numWeapons(0) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);
		

	protected:

		int _numWeapons;

		enum eWeaponIndex { HAMMER, SNIPER, SHOTGUN, NONE };
		enum eOverlayState { ACTIVE, NO_AMMO, NO_WEAPON };
		enum eOverlayTextArea {HEALTH, SHIELD, AMMO };
		

		

		void hudLife(int health);
		void hudShield(int shield);
		void hudWeapon(int ammo, int weapon);
		void hudAmmo(int ammo, int weapon);

		void hudSpawn(int spawn);


		int _health;
		int _shield;
		int _ammo;


		int _actualWeapon;

		Graphics::CServer* _server;

		//Ogre::Overlay *_overlayPlay;
		
		
		//Ogre::Overlay *_overlayDie;

		//Ogre::TextAreaOverlayElement *_textAreaDie;

		//Ogre::TextAreaOverlayElement *_textBoxArea[3];

		//Ogre::OverlayContainer *_weaponsBox[4][3];




		Graphics::COverlay *_overlayPlay;
		Graphics::COverlay *_overlayDie;

		Graphics::COverlay *_textAreaDie;

		Graphics::COverlay *_textBoxArea[3];

		// En vez de 4 deberia de ir el numero de armas pero no tengo cojones U.U
		Graphics::COverlay *_weaponsBox[4][3];

	}; // class CHudOverlay

	REG_FACTORY(CHudOverlay);

} // namespace Logic

#endif // __Logic_Life_H
