//---------------------------------------------------------------------------
// AvatarController.h
//---------------------------------------------------------------------------

/**
@file AvatarController.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author Rubén Mulero Guerrero
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
	class CCameraFeedbackNotifier;
}

// Declaración de la clase
namespace Logic {

	/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes métodos que permiten avances o giros. El uso de este componente
	es un poco atípico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las órdenes que se deben
	llevar a cabo mediante llamadas a métodos públicos del componente.

	Realmente este es el componente que da las ordenes al character controller.
	Esta hecho así para facilitar la implementación de otro tipo de controladores
	de entidad como por ejemplo el espectador.
	
    @ingroup logicGroup

	@author Rubén Mulero Guerrero
	@author Francisco Aisa García
	@date Abril, 2013
	*/
	
	class CAvatarController : public IComponent {
		DEC_FACTORY(CAvatarController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CAvatarController();

		//________________________________________________________________________

		/** Destructor por defecto. */
		virtual ~CAvatarController();


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
			<li><strong>acceleration:</strong> Aceleración del movimiento del player. </li>
			<li><strong>maxVelocity:</strong> Máxima velocidad a la que puede correr el player. </li>
			<li><strong>frictionCoef:</strong> Coeficiente de deceleración del movimiento terrestre. </li>
			<li><strong>airFrictionCoef:</strong> Coeficiente de deceleración del movimiento aereo. </li>
			<li><strong>airSpeedCoef:</strong> Coeficiente de movimiento aereo. </li>
			<li><strong>gravity:</strong> Valor de la aceleración de la gravedad del player. </li>
			<li><strong>jumpForce:</strong> Fuerza de salto. </li>
			<li><strong>dodgeForce:</strong> Fuerza del salto de esquiva. </li>
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
		virtual void onActivate();

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTROL</li>
			<li>ADDFORCEPLAYER</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Dado una rotacion en radianes del eje X e Y rota la entidad,
		limitando el pitch a +90/-90 grados.

		@param XYturn array con los valores de giro del raton.
		*/
		void rotationXY(float XYturn[]);

		//________________________________________________________________________

		/**
		Dado un enumerado indicando el tipo de movimiento se desplaza al player.

		@param commandType Enumerado que indica el tipo de movimiento a realizar.
		*/
		void executeMovementCommand(ControlType commandType);

		//________________________________________________________________________

		/**
		Provoca que la entidad salte en vertical.
		*/
		void executeJump();

		//________________________________________________________________________

		/**
		Dado un enumerado indicando el tipo de salto esquiva se ejecuto dicho salto
		en el player.

		@param commandType Enumerado que indica el tipo de esquiva a realizar.
		*/
		void executeDodge(ControlType commandType);

		//________________________________________________________________________

		/**
		Dado un vector de fuerza (dirección + magnitud) empuja al player en dicha
		dirección.

		@param force Vector de fuerza.
		*/
		void addForce(const Vector3 &force);

		void executeAnimation(Vector3 dir);

		void stopAnimation(Vector3 dir);

		Vector3 getVelocity(){ return _momentum; }

		void setMaxVelocity(float velocity){_maxVelocity = velocity;}

		Vector3 getDisplacementDir(){return _displacementDir;}

		bool getFlying(){return !_touchingGround;}

	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		/**
		Setea el puntero al componente del controlador físico para evitar overhead
		de mensajes en el tick.

		@param unsigned int msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onStart();

		//________________________________________________________________________

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre
		constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Dado un entero con los flags de colisión reportados por el controlador 
		físico, actua sobre el movimiento del player.

		@param collisionFlags Flags de colisión generados por el controlador 
		físico del player.
		*/
		void manageCollisions(unsigned collisionFlags, Vector3 oldPosition);

		//________________________________________________________________________

		/**
		Dado un vector de dirección simplificado (con cada coordenada entre 0 y 1)
		devuelve la dirección en la que el player debería mirar una vez aplicada
		la rotación.

		@param direction Dirección relativa en la que queremos que el player se
		desplace.
		*/
		Vector3 estimateMotionDirection(const Vector3& direction);

		//________________________________________________________________________

		/**
		Calcula el desplazamiento del jugador sobre el suelo.

		@param msecs Milisegundos transcurridos desde el último tick.
		@return El vector de desplazamiento.
		*/
		Vector3 estimateGroundMotion(unsigned int msecs);

		//________________________________________________________________________

		/**
		Calcula el desplazamiento del jugador en el aire.

		@param msecs Milisegundos transcurridos desde el último tick.
		@return El vector de desplazamiento.
		*/
		Vector3 estimateAirMotion(unsigned int msecs);

		//________________________________________________________________________

		/**
		Normaliza el desplazamiento vertical si nuestra velocidad de caida es mayor
		al máximo establecido.
		*/
		void normalizeGravity();

		//________________________________________________________________________

		/**
		Inicializa el array que contiene los vectores que corresponden a cada comando
		de movimiento.
		*/
		void initMovementCommands();

		//________________________________________________________________________

		/**
		Según el tipo de movimiento configura el feedback de la camara.
		*/
		void setCameraEffect();

		//________________________________________________________________________

		/**
		Emite el sonido pasado por parámetro.

		@param soundName Nombre del archivo de audio que queremos reproducir.
		@param loopSound true queremos reproducir el sonido en loop.
		@param play3d true si queremos que el sonido se reproduzca en 3d.
		@param streamSound true si queremos que el sonido se reproduzca en streaming. Util
		para ficheros muy grandes como por ejemplo la música.
		*/
		void emitSound(const std::string &soundName, bool loopSound, bool play3d, bool streamSound);

		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** true si el personaje está tocando el suelo, false si esta en el aire. */
		bool _touchingGround;

		/** Vector de gravedad, puede ser sustituido por un flotante. */
		Vector3 _gravity;

		

		/** Velocidad máxima de caida. */
		float _maxGravVelocity;

		/** Velocidad máxima a la que nuestro personaje se puede desplazar. */
		float _maxVelocity;

		/** Velocidad de aceleración del personaje al desplazarse. */
		float _acceleration;

		/** 
		Coeficiente de rozamiento en el suelo, entre 0 y el tamaño de los msecs
		del tick. A mayor valor, mayor deslizamiento.
		*/
		float _frictionCoef;
		
		/** 
		Coeficiente de rozamiento del aire, entre 0 y 1. A menor valor, menor
		recorrido en el aire. 
		*/
		float _airFrictionCoef;

		/**
		Coeficiente de movimiento aereo, entre 0 y el tamaño de los msecs del tick.
		A mayor valor, mayor desplazamiento aereo.
		*/
		float _airSpeedCoef;

		/** Fuerza de salto vertical. */
		float _jumpForce;

		/** Vector de salto esquiva. */
		Vector3 _dodgeForce;

		/** Vector que indica la dirección del desplazamiento que el controlador debe realizar. */
		Vector3 _displacementDir;

		/** Vector de inercia. */
		Vector3 _momentum;

		/** Puntero al controlador fisico del player. Nos lo quedamos por motivos de eficiencia. */
		CPhysicController* _physicController;

		/** 
		Puntero al notificador de feedback de la camara. Para evitar mandar mensajes en cada tick.
		Será distinto de null solo en el caso del local player (que son los unicos que deberian mover
		a la camara).
		*/
		CCameraFeedbackNotifier* _cameraFX;

		/** 
		Array que contiene los vectores que corresponden a cada uno de los movimientos
		de desplazamiento y salto que se pueden realizar. 
		*/
		Vector3 _movementCommands[18];

		/** Número máximo de comandos de movimiento. */
		static const int MAX_MOVEMENT_COMMANDS = Control::CROUCH;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
