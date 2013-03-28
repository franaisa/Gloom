//---------------------------------------------------------------------------
// Hound.h
//---------------------------------------------------------------------------

/**
@file Hound.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Hound".

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_Hound_H
#define __Logic_Hound_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**
	Clase que implementa las habilidades propias
	del personaje "Hound".

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CHound : public CPlayerClass {
		DEC_FACTORY(CHound);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CHound();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CHound();
		

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


		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Beserker, el personaje dispara más rápido y hace más daño. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();


		private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Tiempo que dura el berserker */
		float _berserkerDuration;

		/** Timer que controla la duración del berserker. */
		float _berserkerTimer;

		/** Porcentaje de incremento de daño del berserker. */
		float _berserkerDamagePercent;

		/** Porcentaje de reduccion de cooldown del berserker. */
		float _berserkerCooldownPercent;

	}; // class CHound

	REG_FACTORY(CHound);

} // namespace Logic

#endif // __Logic_Hound_H
