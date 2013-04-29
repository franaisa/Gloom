/**
@file GrenadeController.h

Contiene la declaración de la cabecera del componente
que controla el timer de la explosion.

@see Logic::CGrenadeController
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_GrenadeController_H
#define __Logic_GrenadeController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	Controla el timer de la explosion de la granada. No puede implementarse
	como una creacion con tiempo porque hay cosas que tienen que hacerse
	al desaparecer la granada (como crear la explosion - y esta si que 
	puede crearse con un timer).

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	
	class CGrenadeController : public IComponent {
		DEC_FACTORY(CGrenadeController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CGrenadeController();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Método llamado en cada frame que controla el timer para la creacion de la
		explosion.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//________________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>explotionTime: </strong>tiempo en segundos tras el cual la granada explota.</li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>CONTACT_ENTER</li>
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
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CEntity* _owner);

	private:


		// =======================================================================
		//                            METODOS PRIVADOS
		// =======================================================================

		/** Crea una entidad GrenadeExplotion justo en el lugar en el que se encuentre la granada (_entity). */
		void createExplotion();

		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Booleano que controla que solo se trate el primer contacto ( debe explotar ). */
		bool _explotionActive;

		/** Transcurrido este tiempo, la se destruye la entidad granada y se ejecuta la explosion. */
		float _explotionTime;

		/** Timer que controla cuando explota la granada. */
		unsigned int _timer;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

		/** Radio de explosion de la granada. */
		float _explotionRadius;

		/** Daño de la explosion. */
		float _explotionDamage;

		/** Fuerza de la explosion*/
		float _explotionForce;

		/** Fuerza de desplazamiento de la granada. */
		float _displacement;

		/** Ruta del sonido de la explosion. */
		std::string _audioExplotion;

	}; // class CGrenadeController

	REG_FACTORY(CGrenadeController);

} // namespace Logic

#endif
