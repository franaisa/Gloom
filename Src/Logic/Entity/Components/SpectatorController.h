/**
@file SpectatorController.h

Contiene la declaración del componente que controla el movimiento 
del espectador.

@see Logic::CSpectatorController
@see Logic::IComponent

@author Francisco Aisa García
@date Abril, 2013
*/

#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	class CMessageControl;
	class CPhysicController;
}

//declaración de la clase
namespace Logic {

	/**
	
	*/

	class CSpectatorController : public IComponent {
		DEC_FACTORY(CSpectatorController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CSpectatorController();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual void activate();

		virtual void onStart(unsigned int msecs);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage *message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		

		/**
		Provoca que la entidad gire. Números Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/

		/**
		Dado un giro en X y otro en Y provoca que la entidad (y la camara
		giren).

		@param XYturn array con los valores de giro del raton.
		*/
		void mouse(float XYturn[]);

		
	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		Vector3 estimateMotionDirection();

		void estimateMotion(unsigned int msecs);

		void executeMovementCommand(ControlType commandType);

		void normalizeDirection();

		void initMovementCommands();


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		/** Velocidad máxima a la que nuestro personaje se puede desplazar. */
		float _maxVelocity;

		/** 
		Coeficiente de rozamiento del aire, entre 0 y 1. A menor valor, menor
		recorrido en el aire. 
		*/
		float _frictionCoef;

		/** Velocidad de aceleración del personaje al desplazarse. */
		float _acceleration;

		/** Vector que indica la dirección del desplazamiento que el controlador debe realizar. */
		Vector3 _displacementDir;

		/** Vector de inercia. */
		Vector3 _momentum;

		/** 
		Array que contiene los vectores que corresponden a cada uno de los movimientos
		de desplazamiento y salto que se pueden realizar. 
		*/
		Vector3 _movementCommands[8];




		/** Puntero al controlador fisico del player. */
		CPhysicController* _physicController;

	}; // class CSpectatorController

	REG_FACTORY(CSpectatorController);

} // namespace Logic

#endif // __Logic_AvatarController_H
