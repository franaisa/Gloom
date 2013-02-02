#ifndef __Logic_ShootHammer_H
#define __Logic_ShootHammer_H

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
	class CShootHammer : public CShoot
	{
		DEC_FACTORY(CShootHammer);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CShootHammer() : CShoot("Hammer") {}
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:
		

		virtual void shoot();
		
	}; // class CShoot

	REG_FACTORY(CShootHammer);

} // namespace Logic

#endif // __Logic_Shoot_H
