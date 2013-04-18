/**
@file RocketController.h

Contiene la declaración de la cabecera del componente
que controla el movimiento del cohete y su explosión.

@see Logic::CRocketController
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#ifndef __Logic_RocketController_H
#define __Logic_RocketController_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Jose Antonio García Yáñez
	@date Marzo, 2013
	*/
	
	class CRocketController : public IComponent {
		DEC_FACTORY(CExplotionController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CRocketController();


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

		//________________________________________________________________________


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

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

		/** true si la granada a golpeado a un enemigo. */
		bool _enemyHit;

		/** Velocidad a la que viaja el misil. */
		float _rocketSpeed;

		/** Radio de explosion del misil. */
		float _explotionRadius;

		/** Daño de la explosion. */
		float _explotionDamage;

		/** Ruta del sonido de la explosion. */
		std::string _audioExplotion;

	}; // class CRocketController

	REG_FACTORY(CRocketController);

} // namespace Logic

#endif
