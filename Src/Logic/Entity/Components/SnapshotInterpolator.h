/**
@file SnapshotInterpolator.h


@see Logic::CSnapshotInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_SnapshotInterpolator_H
#define __Logic_SnapshotInterpolator_H

#include "Logic/Entity/Component.h"

#include <deque>

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CSnapshotInterpolator : public IComponent {
		DEC_FACTORY(CSnapshotInterpolator);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CSnapshotInterpolator();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CSnapshotInterpolator();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>interpolationTimestep:</strong> Tiempo de interpolación. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li></li>
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
		//                            METODOS PROPIOS
		// =======================================================================


	protected:


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Método llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		parámetros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================
		
		float _interpolationTimestep;

		float _interpolationTimer;

		//std::deque<Matrix4> _buffer;

	}; // class CSnapshotInterpolator

	REG_FACTORY(CSnapshotInterpolator);

} // namespace Logic

#endif // __Logic_SnapshotInterpolator_H
