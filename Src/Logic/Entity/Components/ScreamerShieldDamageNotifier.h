/**
@file ScreamerShieldDamageNotifier.h

Contiene la declaración del componente que 
controla los daños recibidos por el escudo
del Screamer.

@see Logic::CScreamerShieldDamageNotifier
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerShieldDamageNotifier_H
#define __Logic_ScreamerShieldDamageNotifier_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CPhysicDynamicEntity;
}

namespace Logic {
	
	/**
	
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	
	class CScreamerShieldDamageNotifier : public IComponent {
		DEC_FACTORY(CScreamerShieldDamageNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerShieldDamageNotifier();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CScreamerShieldDamageNotifier();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>SET_REDUCED_DAMAGE</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea el valor del porcentaje de daños absorbido. Se entiende que
		se llama a este metodo cuando la entidad muere y vuelve a ser
		despertada.
		*/
		virtual void onActivate();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Resta los daños indicados a la entidad (tanto de escudo
		como de vida), dispara los sonidos de daño correspondientes
		y envia el mensaje de muerte si es necesario.

		@param damage Daño que queremos restar al personaje.
		@param enemy Entidad que nos está haciendo daño.
		*/
		void damaged(int damage, CEntity* enemy);

		//__________________________________________________________________

		/**
		Dado un porcentaje de reducción de daños (comprendido entre
		0 y 1) reduce todo el daño recibido en base a ese porcentaje.

		Por ejemplo, un porcentaje de 1 haría que el personaje siempre
		recibiera un daño de 0 puntos (o en otras palabras, que
		fuera inmune).

		@param percentage Porcentaje de reducción de daños. Tiene que 
		estar comprendido entre 0 y 1.
		*/
		inline void reducedDamageAbsorption(float percentage);

		//__________________________________________________________________

		/**
		Este método debe ser usado por aquella entidad que cree a esta entidad,
		para indicar que ésta última le pertenece.

		@param owner Entidad lógica que generó a esta entidad.
		*/
		void setOwner(Logic::CEntity* owner);

	protected:

		virtual void onStart();

		virtual void onFixedTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** 
		Porcentaje de daños recibidos. Su valor debe estar entre 0 (el daño no se
		reduce) y 1 (no recibimos daños).
		*/
		float _reducedDamageAbsorption;

		/** Entidad dueña de esta entidad. */
		CEntity* _owner;

		CPhysicDynamicEntity* _physicComponent;

	}; // class CScreamerShieldDamageNotifier

	REG_FACTORY(CScreamerShieldDamageNotifier);

} // namespace Logic

#endif // __Logic_Life_H
