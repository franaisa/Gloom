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
		// Creamos la entidad y la activamos
		CEntity* grenade = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, Logic::CServer::getSingletonPtr()->getMap());
		assert(grenade != NULL);
		grenade->activate();

		// Seteamos la entidad que dispara la granada
		CGrenadeController* comp = grenade->getComponent<CGrenadeController>("CGrenadeController");
		assert(comp != NULL);
		comp->setOwner(_entity);

		// Spawneamos la granada justo delante del jugador y a la altura de disparo que corresponda
		Vector3 entityPosition = _entity->getPosition();
		Vector3 myPosition = entityPosition + ( Math::getDirection( _entity->getOrientation() ) * _capsuleRadius );
		myPosition.y = entityPosition.y + _heightShoot;

		// Mensaje para situar el collider fisico de la granada en la posicion de disparo.
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition(myPosition);
		msg->setMakeConversion(false);
		grenade->emitMessage(msg);
		
		if(Net::CManager::getSingletonPtr()->imServer())
			Logic::CGameNetMsgManager::getSingletonPtr()->sendCreateEntity(grenade->getEntityID());

		// Mandar mensaje add force
		Logic::CMessageAddForcePhysics* forceMsg = new Logic::CMessageAddForcePhysics();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation() ) * _shootForce), Physics::ForceMode::eIMPULSE );
		forceMsg->setGravity(true);
		grenade->emitMessage(forceMsg);

		//enviamos mensaje por red para que se cree la entidad
		//NOTA: pregunto por si es el servidor, no vaya a ser que el single player se suicide
		
	}

} // namespace Logic

