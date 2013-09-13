/*
@file Bite.h

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_Bite_H
#define __Logic_Bite_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

#include <string>

namespace Logic {

	class CPhysicDynamicEntity;

	/**
	@ingroup logicGroup

	@author Antonio Jesus Narváez Corrales
	@author Francisco Aisa García
	@date Mayo, 2013
	*/

	class CBite : public IComponent {

		DEC_FACTORY(CBite);
	public:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================

		/**
		Constructor parametrizado. Las clases hijas deberán llamar a este constructor
		con el nombre de su arma.

		@param weaponName Nombre del arma implementada por la clase derivada.
		*/
		CBite(){}

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CBite(){}


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>MaxAmmo:</strong> Munición máxima que puede llevar el arma. </li>
			<li><strong>ID:</strong> Identificador de arma. </li>
			<li><strong>physic_radius:</strong> Radio de la cápsula física del personaje. </li>
			<li><strong>heightShoot:</strong> Altura desde donde sale el disparo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>TOUCHED</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Metodo virtual invocado cuando se desactiva la entidad
		*/
		//virtual void onDeactivate();

		/**
		Metodo virtual invocado cuando se desactiva la entidad
		*/
		//virtual void onActivate();

		void setOwner(CEntity * owner);

	protected:

		virtual void onStart();
		virtual void onTick(unsigned int msecs);

	private:

		CPhysicDynamicEntity* _physicComponent;
		CEntity* _owner;
		float _biteDamage;
		float _lifeSteal;
	};
	REG_FACTORY(CBite);
}
#endif