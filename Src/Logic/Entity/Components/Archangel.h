//---------------------------------------------------------------------------
// Archangel.h
//---------------------------------------------------------------------------

/**
@file Archangel.h

@author 
@date Marzo, 2013
*/

#ifndef __Logic_Archangel_H
#define __Logic_Archangel_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**


	@ingroup logicGroup

	@author 
	@date Febrero, 2013
	*/

	class CArchangel : public CPlayerClass {
		DEC_FACTORY(CArchangel);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CArchangel() {
			// No hay memoria dinamica que inicializar
		}

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CArchangel() { }
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		
		// =======================================================================
		//                     METODOS HEREDADOS DE CSkills
		// =======================================================================


		/**
		*/
		virtual void primarySkill() { /* Implementar */ };

		//__________________________________________________________________

		/**
		*/
		virtual void secondarySkill() { /* Implementar */ };


	protected:


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


	}; // class CArchangel

	REG_FACTORY(CArchangel);

} // namespace Logic

#endif // __Logic_SkillsArchangel_H
