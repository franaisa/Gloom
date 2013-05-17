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
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>shadowInvisibilityDuration:</strong> Tiempo que dura la invsibilidad desde que se activa. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void onActivate();


		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** El personaje se vuelve invisible al invocar este método. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();


	protected:

		virtual void onStart();

		//__________________________________________________________________

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Tiempo que dura la invisibilidad. */
		float _invisibilityDuration;

		/** Timer que controla la duración de la invisibilidad. */
		float _invisibilityTimer;

		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingPrimarySkill;

		/**
		Nombre del material original de la entidad
		*/
		std::string _materialName;
	}; // class CShadow

	REG_FACTORY(CShadow);

} // namespace Logic

#endif // __Logic_Shadow_H
