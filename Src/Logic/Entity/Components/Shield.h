//---------------------------------------------------------------------------
// Shield.h
//---------------------------------------------------------------------------

/**
@file Shield.h

Contiene la declaración del componente que implementa el shield.

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#ifndef __Logic_Shield_H
#define __Logic_Shield_H

#include "Logic/Entity/Components/Spell.h"

namespace Logic{
	class CEntity;
}

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del gravity. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al gravity.

	@author Jaime Chapinal Cervantes
	@date Junio, 2013
	*/

	class CShield : public ISpell {
		DEC_FACTORY(CShield);

	public:

		/** Constructor por defecto. */
		CShield() : ISpell("shield"),_shieldSpell(0) { }

		//__________________________________________________________________

		virtual ~CShield();

		//__________________________________________________________________

		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>spellGravityDamageReflect:</strong> Daño que hace el gravity al golpear contra el mundo. </li>
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

		virtual void onDeactivate();
	private:
		
		/** Gravedad por defecto. */
		//float _defaultGravity;

		//float _newGravity;
		float _heightShoot;

		/** Daño del escudo al entrar en el */
		float _damage;

		/** Radio del escudo */
		float _radius;

		/** Porcentaje de daño absorvido */
		float _percentageAbsorbed;

		CEntity *_shieldSpell;
	}; // class 

	REG_FACTORY(CShield);

} // namespace Logic

#endif // __Logic__H
