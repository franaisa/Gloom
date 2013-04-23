//---------------------------------------------------------------------------
// Archangel.h
//---------------------------------------------------------------------------

/**
@file Archangel.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Archangel".

@author Francisco Aisa García
@author Jaime Chapinal Cervantes
@date Marzo, 2013
*/

#ifndef __Logic_Archangel_H
#define __Logic_Archangel_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	/**
	Clase que implementa las habilidades propias
	del personaje "Archangel".

	@ingroup logicGroup

	@author Francisco Aisa García.
	@author Jaime Chapinal Cervantes.
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
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>archangelPrimarySkillCooldown:</strong> Tiempo que dura la piel de diamante desde que se activa. </li>
		</ul>

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

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void activate();

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Su piel se transforma en piedra. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** El personaje cura a los aliados que tiene en un radio. */
		virtual void secondarySkill();


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Tiempo que dura siendo inmune (piel de diamante) */
		float _inmunityDuration;

		/** Timer que controla la duración que lleva siendo inmune. */
		float _inmunityTimer;

		/**
		Nombre del material original de la entidad
		*/
		std::string _materialName;

	}; // class CArchangel

	REG_FACTORY(CArchangel);

} // namespace Logic

#endif // __Logic_Archangel_H
