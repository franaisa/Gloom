//---------------------------------------------------------------------------
// HardDeath.h
//---------------------------------------------------------------------------

/**
@file HardDeath.h

Contiene la declaración del componente que implementa el hechizo de resurreción.

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_ResurrectionClient_H
#define __Logic_ResurrectionClient_H

#include "Logic/Entity/Components/Spell.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del hechizo resurrección. 

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CResurrectionClient : public ISpell {
		DEC_FACTORY(CResurrectionClient);

	public:

		/** Constructor por defecto. */
		CResurrectionClient() : ISpell("resurrection") { }

		//__________________________________________________________________

		virtual ~CResurrectionClient();

		//__________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>spellCoolDownDamageReflect:</strong> Daño que hace el coolDown al golpear contra el mundo. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual puro que debe ser implementado por las clases derivadas para
		especificar que ocurre al usar el disparo primario.
		*/
		virtual void spell();

		//__________________________________________________________________
		
		/**
		Este método es invocado cuando se deja de pulsar el botón de disparo
		primario.
		*/
		virtual void stopSpell();

	private:
		

	}; // class CShootRaycast

	REG_FACTORY(CResurrectionClient);

} // namespace Logic

#endif // __Logic__H
