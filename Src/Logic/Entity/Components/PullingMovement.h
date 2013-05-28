/**
@file FloatingMovement.h

Contiene la declaración de la cabecera del componente
que controla el movimiento de los items.

@see Logic::CFloatingMovement
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_PullingMovement_H
#define __Logic_PullingMovement_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	Este componente se encarga de hacer que los items del escenario
	floten (rotando y desplazandose verticalmente).

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CPhysicDynamicEntity;
	class CShootHammer;
	
	class CPullingMovement : public IComponent {
		DEC_FACTORY(CPullingMovement);
	
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		
		/** Constructor por defecto; en la clase base no hace nada. */
		CPullingMovement();

		
		// =======================================================================
		//                   METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>orbitalSpeed:</strong> Velocidad a la que el item sube y baja. </li>
			<li><strong>orbitalOffset:</strong> Distancia que el item recorre en el desplazamiento vertical. </li>
			<li><strong>orbitalRotationSpeed:</strong> Velocidad a la que rota el objeto. </li>
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
			<li>TOUCHED</li>
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
		//							METODOS PUBLICOS
		// =======================================================================

		/**
		Método que establece el player al que este item va a seguir
		*/
		void setPlayer(CEntity * player);

	protected:

		/**
		Método llamado en cada frame que actualiza la posicion flotante del item.

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre
		constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

	private:


		// =======================================================================
		//                           METODOS PRIVADOS
		// =======================================================================
		

		void itemGrabbed(CEntity* actor);
		

		// =======================================================================
		//                           CAMPOS PRIVADOS
		// =======================================================================


		/** 
		Jugador al que está siguiendo este objeto 
		*/
		CEntity* _player;

		/** Velocidad actual el item */
		float _momentum;

		float _acceleration;

		CPhysicDynamicEntity *_physicComponent;

		/** Id del item que controla este spawn manager */
		std::string _id;

		/** 
		Tipo del arma o la municion de este item. Si no se trata de municion o de un arma
		su valor es -1.
		*/
		int _weaponType;

		/** 
		Cantidad de puntos que ofrece este item al ser cogido (los puntos pueden indicar
		balas, vida, armadura...
		*/
		int _reward;

		float _height;

	}; // class CPullingMovement

	REG_FACTORY(CPullingMovement);

} // namespace Logic

#endif // __Logic_PullingMovement_H
