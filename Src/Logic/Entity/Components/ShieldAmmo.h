//---------------------------------------------------------------------------
// ShieldAmmo.h
//---------------------------------------------------------------------------

/**
@file ShieldAmmo.h

@see Logic::CShieldAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ShieldAmmo_H
#define __Logic_ShieldAmmo_H

#include "Logic/Entity/Components/SpellAmmo.h"

namespace Logic {

	/**
    @ingroup logicGroup

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CShieldAmmo : public ISpellAmmo {
		DEC_FACTORY(CShieldAmmo);
	public:

		/** Constructor por defecto. */
		CShieldAmmo();

		//__________________________________________________________________

		virtual ~CShieldAmmo();

		//__________________________________________________________________

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


		//virtual void reduceCooldown(unsigned int percentage);

		//__________________________________________________________________

		/**
		M�todo virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este m�todo es invocado cuando se deja de pulsar el bot�n de disparo
		primario.
		*/
		virtual void stopSpell();

		//virtual void addAmmo();
	protected:

		virtual bool canUseSpell();


		virtual void onActivate();

		virtual void onWake();

		virtual void onTick(unsigned int msecs);

		void shieldDamage();

	private:

		//std::string _shootAudio;

		//unsigned int _defaultCooldown;
		//unsigned int _cooldown;

		unsigned int  _duration;
		
		//unsigned int _maxAmmo;
		//unsigned int _ammoPerPull;
		//unsigned int _currentAmmo;

		

		//unsigned int _cooldownTimer;
		//unsigned int _durationTimer;
		
		float _shieldRadius;
		int _shieldDamage;

	};

	REG_FACTORY(CShieldAmmo);

} // namespace Logic

#endif // __Logic_ComeBackAmmo_H