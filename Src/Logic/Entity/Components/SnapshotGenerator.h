/**
@file SnapshotGenerator.h


@see Logic::CSnapshotGenerator
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_SnapshotGenerator_H
#define __Logic_SnapshotGenerator_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CSnapshotGenerator : public IComponent {
		//DEC_FACTORY(CSnapshotGenerator); No permitido por ser de tipo abstracto
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CSnapshotGenerator();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CSnapshotGenerator();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>snapshotsPerSecond:</strong> Número de snapshots que se mandan por segundo. </li>
			<li><strong>samplesPerSnapshot:</strong> Número de muestras por snapshot. </li>
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


		virtual void sendSnapshot() = 0;

		//__________________________________________________________________

		virtual void takeSnapshot() = 0;

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
		
		float _snapshotTimestep;

		float _snapshotTimer;

		float _intervalTimestep;

		float _intervalTimer;


	}; // class CSnapshotGenerator

	//REG_FACTORY(CSnapshotGenerator);

} // namespace Logic

#endif // __Logic_SnapshotGenerator_H
