//---------------------------------------------------------------------------
// Gravity.h
//---------------------------------------------------------------------------

/**
@file Gravity.h

@see Logic::CGravity
@see Logic::ISpellAmmo

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_GravityAmmo_H
#define __Logic_GravityAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CGravityAmmo : public ISpellAmmo {
		DEC_FACTORY(CGravityAmmo);
	public:

		/** Constructor por defecto. */
		CGravityAmmo();

		//__________________________________________________________________

		virtual ~CGravityAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		virtual void reduceCooldown(unsigned int percentage);

		//__________________________________________________________________

		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell();

	protected:

		virtual bool canUseSpell();


		virtual void onActivate();

		virtual void onAvailable();

		virtual void onTick(unsigned int msecs);

	private:

		std::string _shootAudio;

		unsigned int _defaultCooldown;
		unsigned int _cooldown;

		unsigned int  _duration;
		
		unsigned int _maxAmmo;
		unsigned int _ammoPerPull;
		unsigned int _currentAmmo;

		bool _spellIsActive;
		unsigned int _cooldownTimer;
		unsigned int _durationTimer;
		
	};

	REG_FACTORY(CGravityAmmo);

} // namespace Logic

#endif // __Logic_GravityAmmo_H