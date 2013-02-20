#ifndef __Logic_ShootProjectile_H
#define __Logic_ShootProjectile_H

#include "Logic/Entity/Components/Shoot.h"

namespace Logic {

	/**
	Este componente controla la capacidad de disparo de una entidad, asi como las armas que puede tener. Procesa mensajes de tipo 
	SHOOT (indican que la entidad ha disparado)
	<p>
	Poseera una vector, con las armas que tiene, asi como su daño y alcance.
	Tambien poseera un vector de booleanos que indicara las armas que posee.
	*/

	class CShootProjectile : public CShoot {
		DEC_FACTORY(CShootProjectile);

	public:

		/** Constructor por defecto. */
		CShootProjectile() : CShoot() {}

		CShootProjectile(const std::string &shoot) : CShoot(shoot) {}

		virtual void shoot();
	};

	REG_FACTORY(CShootProjectile);

} // namespace Logic

#endif // __Logic_Shoot_H
