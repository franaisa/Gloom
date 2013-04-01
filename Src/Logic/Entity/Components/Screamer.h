//---------------------------------------------------------------------------
// Screamer.h
//---------------------------------------------------------------------------

/**
@file Screamer.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Screamer".

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_Screamer_H
#define __Logic_Screamer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaración de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer".

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CScreamer : public CPlayerClass {
		DEC_FACTORY(CScreamer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamer();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamer();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		De momento no lee nada nuevo, tan solo llama a la clase padre.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		
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

		//__________________________________________________________________

		/**
		Se dispara cuando el jugador deja de pulsar la tecla de uso de la habilidad
		primaria.

		Provoca que deje de usarse la habilidad primaria y comience la recuperacion
		de las defensas del Screamer.
		*/
		virtual void stopPrimarySkill();


	private:

		void refreshShieldPosition();

		/** true si la habilidad primaria está siendo usada. */
		bool _primarySkillIsActive;

		Logic::CEntity* _screamerShield;

	}; // class CScreamer

	REG_FACTORY(CScreamer);

} // namespace Logic

#endif // __Logic_Screamer_H
