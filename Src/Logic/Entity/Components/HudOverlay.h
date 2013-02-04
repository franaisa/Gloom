/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Antonio Jesus
@date Febrero, 2013
*/
#ifndef __Logic_HudOverlay_H
#define __Logic_HudOverlay_H

#include "Logic/Entity/Component.h"

#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

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
		CHudOverlay() : IComponent(), _health(0), _shield(0), NUM_WEAPONS(3) {}
		
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

		const int NUM_WEAPONS;

		enum eWeaponIndex { HAMMER, SNIPER, SHOTGUN, NONE };
		enum eOverlayState { ACTIVE, NO_AMMO, NO_WEAPON };
		

		

		void addLife(int health);

		void addShield(int shield);


		int _health;
		Ogre::TextAreaOverlayElement* _healthTextArea;
		int _shield;
		Ogre::TextAreaOverlayElement* _shieldTextArea;

		int _actualWeapon;

		Ogre::OverlayContainer* _weaponsBox[4][3];

	}; // class CHudOverlay

	REG_FACTORY(CHudOverlay);

} // namespace Logic

#endif // __Logic_Life_H
