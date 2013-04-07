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
#include "RocketController.h"
#include "Logic/GameNetMsgManager.h"
#include "../../../Net/Manager.h"
#include "Logic/Entity/Components/AvatarController.h"

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

		_shootForce = entityInfo->getFloatAttribute(weapon + "ShootForce");

		return true;
	}

	//__________________________________________________________________

	void CShootRocketLauncher::fireWeapon() {
		// Obtenemos la informacion asociada al arquetipo del cohete
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Rocket");
		
		// Separacion para que la granada no toque al jugador
		int _rocketSeparation=2;

		Vector3 shootPosition = _entity->getPosition() + (Math::getDirection( _entity->getOrientation() )* (_capsuleRadius) * _rocketSeparation );
		shootPosition.y += _heightShoot;

		CEntity* rocket = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, Logic::CServer::getSingletonPtr()->getMap(), shootPosition);
		assert(rocket != NULL);
		rocket->activate();

		// Seteamos la entidad que dispara el cohete
		CRocketController* comp = rocket->getComponent<CRocketController>("CRocketController");
		assert(comp != NULL);
		comp->setOwner(_entity);

		// Mandar mensaje add force
		Logic::CMessageAddForcePhysics* forceMsg = new Logic::CMessageAddForcePhysics();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation()) * _shootForce), Physics::ForceMode::eFORCE );
		forceMsg->setGravity(false);
		rocket->emitMessage(forceMsg);
	}


} // namespace Logic

