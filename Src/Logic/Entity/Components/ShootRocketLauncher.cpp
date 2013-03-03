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

		_shootForce = entityInfo->getFloatAttribute(weapon+"ShootForce");

		return true;
	}

	//__________________________________________________________________

	void CShootRocketLauncher::fireWeapon() {
		// Obtenemos la informacion asociada al arquetipo de la granada
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Rocket");
		// Creamos la entidad y la activamos
		CEntity* grenade = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, Logic::CServer::getSingletonPtr()->getMap() );
		assert(grenade != NULL);
		grenade->activate();

		// Seteamos la entidad que dispara la granada
		CExplotionController* comp = grenade->getComponent<CExplotionController>("CExplotionController");
		assert(comp != NULL);
		comp->setOwner(_entity);

		// Spawneamos la granada justo delante del jugador y a la altura de disparo que corresponda
		Vector3 myPosition = _entity->getPosition() + ( Math::getDirection( _entity->getOrientation() )* (_capsuleRadius) );
		myPosition.y = _heightShoot;
		grenade->setPosition(myPosition);
		//myPosition.y = _heightShoot;

		// Mensaje para situar el collider fisico de la granada en la posicion de disparo.
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition(myPosition);
		msg->setMakeConversion(true);
		grenade->emitMessage(msg);
		
		// Mandar mensaje add force
		Logic::CMessageAddForcePhysics* forceMsg = new Logic::CMessageAddForcePhysics();
		forceMsg->setForceVector( Math::getDirection( _entity->getOrientation()) * _shootForce );
		grenade->emitMessage(forceMsg);
	}
	
} // namespace Logic

