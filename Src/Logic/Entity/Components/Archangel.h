//---------------------------------------------------------------------------
// Archangel.h
//---------------------------------------------------------------------------

/**
@file Archangel.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Archangel".

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_Archangel_H
#define __Logic_Archangel_H

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

	class CArchangel : public CPlayerClass {
		DEC_FACTORY(CArchangel);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CArchangel();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CArchangel();
		

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


		/** Su piel se transforma en piedra. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** El personaje cura a los aliados que tiene en un radio. */
		virtual void secondarySkill();

	}; // class CArchangel

	REG_FACTORY(CArchangel);

} // namespace Logic

#endif // __Logic_Archangel_H
