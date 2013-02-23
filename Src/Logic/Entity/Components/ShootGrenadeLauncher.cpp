/**
@file ShootGrenadeLauncher.cpp

Contiene la implementacion del componente
de disparo del lanzagranadas.

@see Logic::CShootGrenadeLauncher
@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ShootGrenadeLauncher.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"

namespace Logic {
	IMP_FACTORY(CShootGrenadeLauncher);

	void CShootGrenadeLauncher::fireWeapon() {
		// Crear entidad fisica proyectil
		std::cout << "DISPARO!!!" << std::endl;

		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Grenade");
		CEntity* grenade = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, Logic::CServer::getSingletonPtr()->getMap() );

		// addForce sobre la entidad creada, que en este caso
		// es un rigid dynamic (el lanzacohetes es kinematic)

		// La entidad creada deberia crear otra mas tarde

	}
	
} // namespace Logic

