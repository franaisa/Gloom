/**
@file PositionInterpolator.h


@see Logic::CPositionInterpolator
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_PositionInterpolator_H
#define __Logic_PositionInterpolator_H

#include "Logic/Entity/Component.h"

#include <deque>

namespace Logic {
	class CPhysicController;
}

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CPositionInterpolator : public IComponent {
		DEC_FACTORY(CPositionInterpolator);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CPositionInterpolator();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CPositionInterpolator();


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

		virtual void onStart();


		/**
		Método llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		parámetros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onFixedTick(unsigned int msecs);


	private:


		void interpolateSnapshot(const std::vector<Vector3>& buffer);

		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================
		

		unsigned int _lostTicks;
		unsigned int _ticksPerSample;
		unsigned int _samplesPerSnapshot;

		std::deque<Vector3> _buffer;

		CPhysicController* _controller;

	}; // class CPositionInterpolator

	REG_FACTORY(CPositionInterpolator);

} // namespace Logic

#endif // __Logic_SnapshotInterpolator_H
