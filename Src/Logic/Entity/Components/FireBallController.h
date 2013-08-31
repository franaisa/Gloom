/**
@file FireBallController.h

Contiene la declaración de la cabecera del componente
que controla las acciones de la bola de fuego.

@see Logic::CFireBallController
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_FireBallController_H
#define __Logic_FireBallController_H

#include "Logic/Entity/Component.h"
#include "Physics/ContactPoint.h"

namespace Logic {
	class CIronHellGoat;
	class CPhysicDynamicEntity;
	class CEntity;
}

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CFireBallController : public IComponent {
		DEC_FACTORY(CFireBallController);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CFireBallController();

		virtual ~CFireBallController();

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
			<li><strong>: </strong>.</li>
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
			<li></li>
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
		void setOwner(CIronHellGoat* _owner);

		// @todo Setear una nueva direccion calculada en base a la direccion donde
		// apunta el puntero

		void alterDirection(const Vector3& direction);

	protected:

		virtual void onStart();

		/**

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onFixedTick(unsigned int msecs);

	private:

		void createExplotion(const Physics::CContactPoint& contactPoint);

		void estimateDamage(CEntity* entityHit, const Vector3& explotionPos);

		CIronHellGoat* _owner;

		CPhysicDynamicEntity* _physicComponent;

		Vector3 _direction;

		float _speed;

		float _damage;

		float _explotionRadius;

		float _strength;

		Logic::CEntity *_world;
	}; // class CFireBallController

	REG_FACTORY(CFireBallController);

} // namespace Logic

#endif
