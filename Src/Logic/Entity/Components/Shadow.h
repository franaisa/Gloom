//---------------------------------------------------------------------------
// Shadow.h
//---------------------------------------------------------------------------

/**
@file Shadow.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Shadow".

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_Shadow_H
#define __Logic_Shadow_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**
	Clase que implementa las habilidades propias
	del personaje "Shadow".

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CShadow : public CPlayerClass {
		DEC_FACTORY(CShadow);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CShadow();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CShadow();
		

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

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** El personaje se vuelve invisible al invocar este método. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

	}; // class CShadow

	REG_FACTORY(CShadow);

} // namespace Logic

#endif // __Logic_Shadow_H
