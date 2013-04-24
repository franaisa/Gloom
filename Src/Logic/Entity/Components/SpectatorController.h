//---------------------------------------------------------------------------
// SpectatorController.h
//---------------------------------------------------------------------------

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

// Predeclaración de clases
namespace Logic {
	class CMessageControl;
	class CPhysicController;
}

// Declaración de la clase
namespace Logic {

	/**
	Componente que controla el manejo del espectador. Se encarga
	de mandar los comandos de movimiento al controlador físico y
	de procesar los comandos de movimiento recibidos.

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Abril, 2013
	*/

	class CSpectatorController : public IComponent {
		DEC_FACTORY(CSpectatorController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CSpectatorController();
		
		//________________________________________________________________________

		/** Destructor. */
		virtual ~CSpectatorController();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>acceleration:</strong> Aceleración del movimiento del espectador. </li>
			<li><strong>maxVelocity:</strong> Máxima velocidad a la que se puede mover el espectador. </li>
			<li><strong>frictionCoef:</strong> Coeficiente de deslizamiento del espectador. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de inercia y desplazamiento.
		*/
		virtual void activate();

		//________________________________________________________________________

		/**
		Setea el puntero al componente del controlador físico para evitar overhead
		de mensajes en el tick.

		@param unsigned int msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onStart(unsigned int msecs);

		//________________________________________________________________________

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
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

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Dado un giro en X y otro en Y provoca que la entidad (y la camara
		giren).

		@param XYturn array con los valores de giro del raton.
		*/
		void mouse(float XYturn[]);

		//________________________________________________________________________

		/**
		Dado un enumerado indicando el tipo de movimiento se desplaza al player.

		@param commandType Enumerado que indica el tipo de movimiento a realizar.
		*/
		void executeMovementCommand(ControlType commandType);
		

	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		/**
		Dado un vector de dirección simplificado (con cada coordenada entre 0 y 1)
		devuelve la dirección en la que el player debería mirar una vez aplicada
		la rotación.

		@param direction Dirección relativa en la que queremos que el player se
		desplace.
		*/
		Vector3 estimateMotionDirection();

		//________________________________________________________________________

		/**
		Calcula el desplazamiento del espectador.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void estimateMotion(unsigned int msecs);

		//________________________________________________________________________

		/** Normaliza el vector de desplazamiento al máximo establecido. */
		void normalizeDirection();

		//________________________________________________________________________	

		/**
		Inicializa el array que contiene los vectores que corresponden a cada comando
		de movimiento.
		*/
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

		/** Puntero al controlador fisico del player. Nos lo quedamos por motivos de eficiencia. */
		CPhysicController* _physicController;

		/** 
		Array que contiene los vectores que corresponden a cada uno de los movimientos
		de desplazamiento y salto que se pueden realizar. 
		*/
		Vector3 _movementCommands[8];

	}; // class CSpectatorController

	REG_FACTORY(CSpectatorController);

} // namespace Logic

#endif // __Logic_AvatarController_H
