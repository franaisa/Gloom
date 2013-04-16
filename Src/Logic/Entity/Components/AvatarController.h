/**
@file AvatarController.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes métodos que permiten avances o giros. El uso de este componente
	es un poco atípico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las órdenes que se deben
	llevar a cabo mediante llamadas a métodos públicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las órdenes, sin embargo de momento así es suficiente.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CAvatarController : public IComponent
	{
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(), _jumping(false), _walking(false), _walkingBack(false), 
			_strafingLeft(false), _strafingRight(false), _speed(0.05f) {}
		
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
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
		*/
		virtual void deactivate();

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
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Provoca que la entidad avance.
		*/
		void walk();

		/**
		Provoca que la entidad retroceda.
		*/
		void walkBack();

		/**
		Provoca que la entidad cese el avance.
		*/
		void stopWalk();

		/**
		Provoca que la entidad cese el retroceso
		*/
		void stopWalkBack();

		/**
		Provoca que la entidad se desplace lateralmente a la izquierda.
		*/
		void strafeLeft();

		/**
		Provoca que la entidad se desplace lateralmente a la derecha.
		*/
		void strafeRight();

		/**
		Provoca que la entidad cese el desplazamiento lateral derecho.
		*/
		void stopStrafeRight();
		
		/**
		Provoca que la entidad cese el desplazamiento lateral izquierdo.
		*/
		void stopStrafeLeft();

		/**
		Provoca que la entidad salte.
		*/
		void jump();

		/**
		Provoca que la entidad rebote
		*/
		void rebound();

		/**
		Provoca que a la entidad se le aplique una fuerza
		*/
		void force();

		/**
		Provoca que la entidad gire. Números Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void mouse(const float* amount);

		/**
		Devuelve el estado del booleano falling
		*/
		bool getFalling(){
			return _falling;
		};

		/**
		Devuelve el estado del booleano WalkingBack
		*/
		bool getWalkingBack(){
			return _walkingBack;
		};

	protected:

		/**
		Atributo para saber si la entidad está avanzando.
		*/
		bool _walking;

		/**
		Atributo para saber si la entidad está retrocediendo.
		*/
		bool _walkingBack;

		/**
		Atributo para saber si la entidad está moviendose lateralmente
		a la izquierda.
		*/
		bool _strafingLeft;

		/**
		Atributo para saber si la entidad está moviendose lateralmente
		a la derecha.
		*/
		bool _strafingRight;

		/**
		Atributos para el control del salto de la entidad
		*/
		bool _jumping;
		bool _jumpingControl;
		bool _canJump;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;

		/**
		Atributo que indica la magnitud de velocidad del salto de la entidad.
		*/
		double _speedJump;

		/**
		Atributo que indica la magnitud de la gravedad para el MRUA
		*/
		double _gravity;

		/**
		Atributo que indica si estamos cayendo (se actualizará mediante un mensaje que nos envia el PhysicController)
		*/
		bool _falling;

		/**
		Atributo que indica la dirección en el momento de saltar o de empezar a caer
		*/
		Vector3 _direccionSaltoCaida;

		/**
		Atributo que indica si la entidad empezó a caer
		*/
		bool _caida;

		/**
		Atributos que se encargan de los saltos laterales (activacion y velocidad)
		*/
		bool _sideJump;
		bool _velocitySideJump;

		int _nConcatSideJump;
		bool _sideContact;
		bool _sideFly;
		int _timeConcatSideJump;
		bool _activeContact;

		/**
		Atributo que lleva la activacion de la concatenacion de saltos laterales
		*/
		bool _activeConcat;

		/**
		Atributo que lleva la dirección en la que tiene que ir el rebote del jugador
		*/
		Vector3 _dirRebound;

		/**
		Atributo que dice si el rebote está activo
		*/
		bool _rebound;

		/**
		Atributo que dice si hay que aplicar una fuerza
		*/
		bool _force;

		/**
		Atributo que indica la potencia explosiva que se el aplicará al salto lateral al iniciarse
		*/
		float _powerSideJump;

		/**
		Atributo que indica la potencia explosiva que se el aplicará al salto al iniciarse
		*/
		float _powerJump;

		/**
		Atributo que indica la potencia explosiva que se el aplicará al salto en jumper al iniciarse (eje Y)
		*/
		float _powerJumpForce;

		/**
		Atributo que indica la potencia explosiva que se el aplicará al salto en jumper al iniciarse (eje X y Z)
		*/
		float _velocityForce;

		/**
		Atributo que indica la potencia explosiva que se el aplicará al salto en jumper al iniciarse (eje X y Z)
		*/
		Vector3 _directionForce;

		/**
		Atributo que indica que hay que aplicar la velocidad de jumper
		*/
		bool _applyForce;


		/**
		Atributos que indican que velocidad extra tendrá el salto lateral, segun si no es concatenado o sí respectivamente
		*/
		float _sumSpeedSideJump;
		float _sumSpeedSideJumpConcat;

		/**
		Atributo que indica el factor con el que el movimiento actual afectará a la dirección del jugador cuando se encuentra en el aire
		*/
		float _restitutionMoveAir;

		/**
		Atributo que indica el maximo tiempo para contabilizar la concatenacion de un salto lateral
		*/
		int _maxTimeConcatSideJump;

		/**
		Atributos que indica la velocidad máxima de caida que puedes llegar a alcanzar
		*/
		float _maxSpeedDown;

		/**
		Atributo que indica la ruta del sonido de pasos
		*/
		std::string _audioStep;

		/**
		Atributo que indica la ruta del sonido de salto
		*/
		std::string _audioJump;

		/**
		Atributo que indica la ruta del sonido de salto lateral
		*/
		std::string _audioSideJump;

		/**
		Atributo que lleva el tiempo de el movimiento rectilineo uniformemente acelerado.
		*/
		unsigned int _timeMrua;
	
		/**
		Atributo que lleva la velocidad inicial con la que inicias.
		*/
		float _vo;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
