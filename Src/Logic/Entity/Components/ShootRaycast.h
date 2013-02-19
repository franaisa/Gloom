#ifndef __Logic_ShootRaycast_H
#define __Logic_ShootRaycast_H

#include "Logic/Entity/Components/Shoot.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la capacidad de disparo de una entidad, asi como las armas que puede tener. Procesa mensajes de tipo 
	SHOOT (indican que la entidad ha disparado)
	<p>
	Poseera una vector, con las armas que tiene, asi como su daño y alcance.
	Tambien poseera un vector de booleanos que indicara las armas que posee.

*/


	class CShootRaycast : public CShoot
	{
		DEC_FACTORY(CShootRaycast);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShootRaycast() : CShoot() {}

		CShootRaycast(const std::string &shoot) : CShoot(shoot) {}

		virtual void shoot();

		// Metodo que realmente realiza el disparo
		virtual std::pair<CEntity*, Ray> fireWeapon();

		// Metodo que se encarga de mandar los mensajes que correspondan cuando se hace hit
		virtual void triggerHitMessages(std::pair<CEntity*, Ray> entityHit);
	};

	REG_FACTORY(CShootRaycast);

} // namespace Logic

#endif // __Logic_Shoot_H
