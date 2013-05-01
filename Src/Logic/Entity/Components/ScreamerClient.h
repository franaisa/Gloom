//---------------------------------------------------------------------------
// ScreamerClient.h
//---------------------------------------------------------------------------

/**
@file ScreamerClient.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Screamer" (Cliente).

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerClient_H
#define __Logic_ScreamerClient_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaración de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer" (Cliente).

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CScreamerClient : public CPlayerClass {
		DEC_FACTORY(CScreamerClient);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerClient();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamerClient();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
		

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>ACTIVATE_SCREAMER_SHIELD</li>
			<li>DEACTIVATE_SCREAMER_SHIELD</li>
			<li>SET_RELATED_ENTITY</li>
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

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** 
		Absorbe todos los proyectiles que le llegan de frente. Esta habilidad tiene
		un límite, si se sobrepasa explotas.
		*/
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();


	protected:

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);


	private:


		// =======================================================================
		//                          MÉTODOS PRIVADOS
		// =======================================================================


		/** Activa el shader del escudo y a la entidad */
		void activateScreamerShield();

		//__________________________________________________________________

		/** Desactiva el shader del escudo y a la entidad. */
		void deactivateScreamerShield();

		//__________________________________________________________________

		/** Recoloca el escudo del screamer en la posición del punto de mira. */
		void refreshShieldGraphicsPos();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** true si el escudo está activado */
		bool _shieldIsActive;

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

	}; // class CScreamerClient

	REG_FACTORY(CScreamerClient);

} // namespace Logic

#endif // __Logic_ScreamerClient_H
