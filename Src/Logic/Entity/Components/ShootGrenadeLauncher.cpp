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
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/GrenadeController.h"
#include "Logic/GameNetMsgManager.h"
#include "../../../Net/Manager.h"
#include "Logic/Entity/Components/AvatarController.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"

namespace Logic {
	IMP_FACTORY(CShootGrenadeLauncher);

	bool CShootGrenadeLauncher::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootProjectile::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;
		std::string weapon = aux.str();

		_shootForce = entityInfo->getFloatAttribute(weapon + "ShootForce");

		return true;
	}

	//__________________________________________________________________

	void CShootGrenadeLauncher::fireWeapon() {
		// Obtenemos la informacion asociada al arquetipo de la granada
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Grenade");

		// Ojo si vamos hacia atras necesitamos mas separacion o la liamos ( no se si la jode la orientacion o la posicion o estoy del reves )
		int inverse = 1;
		if(_entity->getComponent<CAvatarController>("CAvatarController")->getWalkingBack())
			inverse = 2;

		// Spawneamos la granada justo delante del jugador y a la altura de disparo que corresponda
		Vector3 shootPosition = _entity->getPosition() + ( Math::getDirection( _entity->getOrientation() )* (_capsuleRadius) * inverse );
		shootPosition.y += _heightShoot;
		
		// Creamos la entidad y la activamos
		CEntity* grenade = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, Logic::CServer::getSingletonPtr()->getMap(), shootPosition);
		assert(grenade != NULL);
		grenade->activate();

		// Seteamos la entidad que dispara la granada
		CGrenadeController* comp = grenade->getComponent<CGrenadeController>("CGrenadeController");
		assert(comp != NULL);
		comp->setOwner(_entity);
		
		// Mandar mensaje add force
		Logic::CMessageAddForcePhysics* forceMsg = new Logic::CMessageAddForcePhysics();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation() ) * _shootForce), Physics::ForceMode::eIMPULSE );
		forceMsg->setGravity(true);
		grenade->emitMessage(forceMsg);

	}

} // namespace Logic

