//---------------------------------------------------------------------------
// ShootShotGun.h
//---------------------------------------------------------------------------

/**
@file ShootShotGun.h

Contiene la declaración del componente que implementa la escopeta.
 
@author Antonio Jesus Narváez Corrales
@date Febrero, 2013
*/

#ifndef __Logic_ShootShotGun_H
#define __Logic_ShootShotGun_H

#include "Logic/Entity/Components/ShootRaycast.h"

namespace Logic {

	/**
	@ingroup logicGroup

	Este componente implementa la funcionalidad de la escopeta. Tan solo necesitamos
	redefinir los mensajes que se mandan a las entidades en caso de hit (que
	en este caso nos sirve la implementación por defecto), ya que el resto
	del comportamiento esta definido en el arquetipo que describe a la escopeta.

	@author Antonio Jesus Narváez Corrales
	@date Febrero, 2013
	*/

	class CShootShotGun : public CShootRaycast {
		DEC_FACTORY(CShootShotGun);
	public:

		/** Constructor por defecto. */
		CShootShotGun() : CShootRaycast("ShotGun") { }

		//__________________________________________________________________

		/**
		Método que se encarga de mandar los mensajes que corresopondan a la entidad
		que se ha golpeado en caso de hacer hit.

		Sencillamente reutiliza la implementación por defecto de CShootRaycast.

		@param entityHit Pareja que contiene el puntero a la entidad que se ha dado
		(o NULL si no se ha colisionado con ninguna) y el rayo que se disparo para
		comprobar si habia colisiones.
		*/
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit) { CShootRaycast::triggerHitMessages(entityHit); }
	};

	REG_FACTORY(CShootShotGun);

} // namespace Logic

#endif // __Logic_Shoot_H
