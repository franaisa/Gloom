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
#include "Physics/Server.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"

namespace Logic {
	IMP_FACTORY(CShootGrenadeLauncher);

	CShootGrenadeLauncher::~CShootGrenadeLauncher() {
		// Nada que hacer
	}

	//__________________________________________________________________

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

		//std::cout << "Posicion del jugador x,y,z: " << _entity->getPosition().x << "," << _entity->getPosition().y <<"," << _entity->getPosition().z << std::endl;
		// Spawneamos la granada justo delante del jugador y a la altura de disparo que corresponda
		Vector3 directionNormalise=Math::getDirection( _entity->getOrientation());
		directionNormalise.normalise();
		Vector3 shootPosition = _entity->getPosition() + directionNormalise* (_capsuleRadius+6.0);////3.4 es el radio del cohete y lo demas es la separacion para que vaya tanto en sp como mp (culpa del mp)
		shootPosition.y += _heightShoot-1.7; //Altura del pj menos algo menos del radio de la granada para que salga en el centro de la mira
		//std::cout << "Posicion de la granada a disparar x,y,z: " << shootPosition.x <<"," << shootPosition.y <<"," << shootPosition.z << std::endl;
		
		//Comprobamos si la granada tiene espacio para ser disparado
		//Creamos el origen del rayo que sera igual al de la posicion de disparo menos el desplazamiento
		Vector3 origin = _entity->getPosition()+Vector3(0,_heightShoot-1.7,0);
		Vector3 noSpacePosition=origin;
		//Calculamos la distancia entre la posicion de disparo y el origen
		float distance=origin.distance(shootPosition);
		// Creamos el ray desde el origen en la direccion del raton
		Vector3 direction=Math::getDirection(_entity->getOrientation());
		direction.normalise();
		Ray ray(origin, direction);
		//Desde el centro del jugador en la dirección que será disparado la granada sin tenernos en cuenta a nosotros mismos.
		//Si hemos tocado algo es que no hay espacio por lo tanto lo lanzamos desde el centro del jugador (creo que seria mejor explotarlo directamente por movidas de shapes)
		if(Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, distance+10,_entity->getEntityID()) != NULL){ //se le ha puesto 10 porque las unidades de physx no coinciden con las de ogre y fui a ojo
			std::cout << "La granada no tiene espacio para salir"<< std::endl;
			shootPosition=noSpacePosition;
		}

		float yaw=Math::fromRadiansToDegrees(_entity->getYaw());
		float pitch=360-Math::fromRadiansToDegrees(_entity->getPitch());

		CEntity* grenade = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
			entityInfo, Logic::CServer::getSingletonPtr()->getMap(), shootPosition, yaw, pitch);
		
		assert(grenade != NULL);
		grenade->activate();

		// Seteamos la entidad que dispara la granada
		CGrenadeController* comp = grenade->getComponent<CGrenadeController>("CGrenadeController");
		assert(comp != NULL);
		comp->setOwner(_entity);
		
		// Mandar mensaje add force
		std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation() ) * _shootForce), Physics::eIMPULSE );

		forceMsg->setGravity(true);
		grenade->emitMessage(forceMsg);

	}

} // namespace Logic

