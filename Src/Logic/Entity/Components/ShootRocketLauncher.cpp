/**
@file ShootRocketLauncher.cpp

Contiene la implementacion del componente
de disparo del lanzacohetes.

@see Logic::CShootRocketLauncher
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "ShootRocketLauncher.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/ExplotionController.h"
#include "RocketController.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"

namespace Logic {
	IMP_FACTORY(CShootRocketLauncher);

	bool CShootRocketLauncher::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootProjectile::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;
		std::string weapon = aux.str();

		return true;
	}

	//__________________________________________________________________

	void CShootRocketLauncher::fireWeapon() {
		// Obtenemos la informacion asociada al arquetipo del cohete
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Rocket");
		// Creamos la entidad y la activamos
		CEntity* rocket = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, Logic::CServer::getSingletonPtr()->getMap() );
		assert(rocket != NULL);
		rocket->activate();

		// Seteamos la entidad que dispara el cohete
		CRocketController* comp = rocket->getComponent<CRocketController>("CRocketController");
		assert(comp != NULL);
		comp->setOwner(_entity);
		comp->setDirection(Math::getDirection( _entity->getOrientation()));

		// Spawneamos el cohete justo delante del jugador y a la altura de disparo que corresponda
		Vector3 myPosition = _entity->getPosition() + ( Math::getDirection( _entity->getOrientation() )* (_capsuleRadius) );
		myPosition.y = _heightShoot;
		rocket->setPosition(myPosition);

		// Mensaje para situar el collider fisico del cohete en la posicion de disparo.
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition(myPosition);
		msg->setMakeConversion(true);
		rocket->emitMessage(msg);
		
	}
	
} // namespace Logic

