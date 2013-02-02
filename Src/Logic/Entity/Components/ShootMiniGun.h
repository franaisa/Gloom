#ifndef __Logic_ShootMiniGun_H
#define __Logic_ShootMiniGun_H

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
	class CShootMiniGun : public CShoot
	{
		DEC_FACTORY(CShootMiniGun);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShootMiniGun() : CShoot("MiniGun") {}
	
		
	}; // class CShoot

	REG_FACTORY(CShootMiniGun);

} // namespace Logic

#endif // __Logic_Shoot_H
