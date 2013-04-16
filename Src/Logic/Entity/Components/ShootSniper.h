//---------------------------------------------------------------------------
// ShootSniper.h
//---------------------------------------------------------------------------

/**
@file ShootSniper.h

Contiene la declaración del componente que implementa el arma sniper.

@author Antonio Jesus Narváez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_ShootSniper_H
#define __Logic_ShootSniper_H

#include "Logic/Entity/Components/ShootRaycast.h"

namespace Logic {
	
	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la minigun. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la minigun.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	@deprecated Actualmente la minigun funciona como la sniper, hay que cambiarlo para
	que la sniper tenga su propio componente y la minigun funcione como tal.
	*/

	class CShootSniper : public CShootRaycast {
		DEC_FACTORY(CShootSniper);

	public:

		/** Constructor por defecto. */
		CShootSniper() : CShootRaycast("sniper") {}

		virtual ~CShootSniper();

		//__________________________________________________________________

		/**
		Método que se encarga de mandar los mensajes que correspondan a la entidad
		que se ha golpeado en caso de hacer hit.

		Sencillamente reutiliza la implementación por defecto de CShootRaycast.

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit) { CShootRaycast::triggerHitMessages(entityHit); }

	}; // class CShootSniper

	REG_FACTORY(CShootSniper);

} // namespace Logic

#endif // __Logic_Shoot_H
