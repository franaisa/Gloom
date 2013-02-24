/**
@file ExplotionHitNotifier.h

Contiene la declaración de la cabecera del componente
que se encarga de mandar el mensaje de daño a aquellas
entidades que se vean afectadas por la explosion.

@see Logic::CExplotionHitNotifier
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Logic_ExplotionHitNotifier_H
#define __Logic_ExplotionHitNotifier_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	
	class CExplotionHitNotifier : public IComponent {
		DEC_FACTORY(CExplotionHitNotifier);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto; en la clase base no hace nada. */
		CExplotionHitNotifier() : IComponent() { }


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
			<li><strong>explotionDamage: </strong>Daño máximo que puede llegar a hacer la explosión.</li>
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
		virtual bool accept(CMessage *message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		//________________________________________________________________________

		/**
		Setea el puntero a la entidad que ha disparado la granada

		@param CEntity Puntero a la entidad que disparo la granada.
		*/
		void setOwner(CEntity* _owner);

	private:


		// =======================================================================
		//                            MIEMBROS PRIVADOS
		// =======================================================================


		void explotionHit(CEntity* entity);


		// =======================================================================
		//                            CAMPOS PRIVADOS
		// =======================================================================

		/** Daño máximo que puede realizar la explosión. */
		float _explotionDamage;

		/** Radio de acción de la onda expansiva. */
		float _explotionRadius;

		/** Entidad que ha disparado la granada. */
		CEntity* _owner;

	}; // class CExplotionHitNotifier

	REG_FACTORY(CExplotionHitNotifier);

} // namespace Logic

#endif
