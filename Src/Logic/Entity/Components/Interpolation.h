/**
@file Interpolation.h

Contiene la declaración del componente que realiza la interpolacion
del movimiento cuando el servidor nos informa de la posicion que deberiamos
tener.

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_Interpolation_H
#define __Logic_Interpolation_H

#include "Logic/Entity/Component.h"

namespace Logic  {

	/**
    @ingroup logicGroup

	Clase encargada de recolocar al jugador en caso de no estar sincronizados
	con el servidor.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CInterpolation : public IComponent {
		DEC_FACTORY(CInterpolation);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto */
		CInterpolation() : IComponent() {}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa. Actualmente este componente no lee nada para inicializarse.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Tick de reloj del componente
		*/
		virtual void tick();


		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>SYNC_POSITION</li>
			<li>CONTROL</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		/**
		Método que mueve el punto del servidor para que esté actualizado
		*/
		void moveServerPos();
		/**
		calcula si hay que hacer interpolación, o mueve al player si la distancia es
		muy grande
		*/
		void calculateInterpolation();

	private:
		/**
		Posición que el servidor me ha dicho que es donde debo estar
		*/
		Matrix4 _serverPos;
		float _yawDifference;
		/**
		distancia maxima a la que interpolo poco a poco
		*/
		float _maxDistance;
		/**
		distancia minima a la que interpolo poco a poco
		*/
		float _minDistance;
		/**
		minima diferencia de yaw que puede haber entre servidor y cliente
		*/
		float _minYaw;
		/**
		maxima diferencia de yaw que puede haber entre servidor y cliente
		*/
		float _maxYaw;
		/**
		minima diferencia de pitch que puede haber entre servidor y cliente
		*/
		float _minPitch;
		/**
		maxima diferencia de pitch que puede haber entre servidor y cliente
		*/
		float _maxPitch;
		/**
		Variables de control de interpolación (para no liarla)
		*/
		bool _interpolating;
		bool _canInterpolateMove;
		bool _canInterpolateRotation;
		/**
		variable que indica el ping con el que estamos corrigiendo
		*/
		unsigned int _actualPing;
		/**
		dirección en la que se está moviendo el server
		*/
		Vector3 _serverDirection;

		float _speed;
		float _rotationSpeed;
	}; // class CInterpolation

	REG_FACTORY(CInterpolation);

} // namespace Logic

#endif // __Logic_Life_H
