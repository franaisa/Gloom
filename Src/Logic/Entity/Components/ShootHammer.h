//---------------------------------------------------------------------------
// ShootHammer.h
//---------------------------------------------------------------------------

/**
@file ShootHammer.h

Contiene la declaración del componente que implementa el hammer.

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#ifndef __Logic_ShootHammer_H
#define __Logic_ShootHammer_H

#include "Logic/Entity/Components/ShootRaycast.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad del hammer. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit, ya que 
	el resto del comportamiento esta definido en el arquetipo que describe 
	al hammer.

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
	*/

	class CShootHammer : public CShootRaycast {
		DEC_FACTORY(CShootHammer);

	public:

		/** Constructor por defecto. */
		CShootHammer() : CShootRaycast("Hammer") { }

		//__________________________________________________________________

		/** */
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:

		/**
		Redefine el método de decremento de munición, que en este caso se deja vacio, 
		ya que el hammer no tiene munición.
		*/
		virtual void decrementAmmo();

		//__________________________________________________________________

		/**
		Método que se encarga de mandar los mensajes que corresopondan a la entidad
		que se ha golpeado en caso de hacer hit.

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit);
		
	}; // class CShootRaycast

	REG_FACTORY(CShootHammer);

} // namespace Logic

#endif // __Logic_Shoot_H
