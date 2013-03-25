//---------------------------------------------------------------------------
// Screamer.h
//---------------------------------------------------------------------------

/**
@file Screamer.h

@author 
@date Marzo, 2013
*/

#ifndef __Logic_Screamer_H
#define __Logic_Screamer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**


	@ingroup logicGroup

	@author 
	@date Febrero, 2013
	*/

	class CScreamer : public CPlayerClass {
		DEC_FACTORY(CScreamer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamer() {
			// No hay memoria dinamica que inicializar
		}

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamer() { }
		

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


	}; // class CScreamer

	REG_FACTORY(CScreamer);

} // namespace Logic

#endif // __Logic_Screamer_H
