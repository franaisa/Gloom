/**
@file ShootRocketLauncher.cpp

Contiene la implementacion del componente
de disparo del lanzacohetes.

@see Logic::CShootRocketLauncher
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Abril, 2013
*/

#include "ShootRocketLauncher.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "RocketController.h"
#include "Logic/GameNetMsgManager.h"
#include "../../../Net/Manager.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Physics/Server.h"
#include "PhysicDynamicEntity.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageContactEnter.h"

namespace Logic {
	IMP_FACTORY(CShootRocketLauncher);

	CShootRocketLauncher::~CShootRocketLauncher() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShootRocketLauncher::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootProjectile::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;
		std::string weapon = aux.str();

		_shootForce = entityInfo->getFloatAttribute(weapon + "ShootForce");
		_projectileRadius = entityInfo->getFloatAttribute(weapon + "ProjectileRadius");
		return true;
	}

	//__________________________________________________________________

	void CShootRocketLauncher::fireWeapon() {
		//Paso1
		//Lo primero es ver el punto exacto en el que saldría el centro del cohete de la capsula
		//Sacamos un punto en la dirección que apuntamos muy fuera de la capsula y trazamos un raycast para el cual solo nos interesa en que punto choca con nosotros
		Vector3 directionNormalise=Math::getDirection(_entity->getOrientation());
		directionNormalise.normalise();
		Vector3 outPosition = _entity->getPosition() +Vector3(0,_heightShoot,0)+directionNormalise*10;//Nos separamos lo suficiente
		//std::cout << "Posicion del jugador: " << _entity->getPosition()+Vector3(0,_heightShoot,0) << std::endl;
		//std::cout << "El punto exterior para hacer el punto de impacto es: " << outPosition << std::endl;
		Vector3 inverseDirection=directionNormalise*-1;
		Ray ray(outPosition, inverseDirection);
		Vector3 capsulleCollisionPoint=Physics::CServer::getSingletonPtr()->raycastClosestSpecificPoint(ray, 10, _entity->getEntityID());
		/*if( capsulleCollisionPoint!=Vector3(-5,-5,-5) ){
			std::cout << "El punto de choque con la capsula es: "<< capsulleCollisionPoint  <<std::endl;
		}
		else
			std::cout << "Algo hay mal" << std::endl;*/

		//Paso2
		//Separamos el cohete de la capsula en la direccion en la que se apuntaba con el radio(o más por interpolamierder)
		Vector3 separationPoint=capsulleCollisionPoint+directionNormalise*(_projectileRadius+3);//radio del cohete y algo mas por posibles errores decimales/inet/correr hacia delante.
		// r+1.5 no te das al saltar, con 1 falla a veces, se podria probar a reducir algo . Si le añades inet el servidor bien (evidentemente) y el cliente falla forever
		// r+6 no le da al cliente, lo mismo el primero por no tener precarga, esto no es escalable, cuanto mas ping mas te comes
		// Con el nuevo estilo de cohete r+2.5 ha fallado en el cliente al delante/salto, lo mismo por la interpolacion, poner a 3 y reducir en la nueva version si es conveniente
		//std::cout << "El punto de separacion para la creación del cohete es : " << separationPoint << std::endl;
		//std::cout << "La separacion entre el punto de capsula y el punto de separacion es : " << separationPoint.distance(capsulleCollisionPoint) << std::endl;

		//Paso3
		//Ahora creamos mediante overlap el cohete y nos aseguramos que no toque con nada 
		//ojo, si no hay colision con pared por salir detras habría que tirar un rayo como antiguamente 
		//Con la separacion anterior no nos puede tocar a nosotros y si toca cualquier otra significa que no tiene espacio
		//Si no tiene espacio tenemos que crear el cohete solo graficamente y automaticamente explotarlo
		//Para ello creo que una nueva entidad(sin fisicas) con un componente ExplotionRocketServer/Client
		//Para red el server tendria el tipo server que ademas aplica daño, y el cliente tendria solo lo gráfico
		CEntity** entitiesHit = NULL;
		int nbHits = 0;
		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra el cohete con el radio que posea
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(2);//radio del cohete
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, separationPoint, entitiesHit, nbHits);
		//No valen estaticos como itemSpawn o SpawnPoint
		int hitsValidos=0;
		for(int i = 0; i < nbHits; ++i) {
			// Si la entidad golpeada es valida ( no puedo decir todos los triggers porque a las sierras y pinchos les quiero dar, ¿o no?
			if(entitiesHit[i] != NULL) {
				if(entitiesHit[i]->getName().compare("David")==0)//Quitar cuando se vea que no pasa nunca(creo k no deberia pasar trololol)
					std::cout << "No deberias de chocar contigo mismo en el overlap" << std::endl;
				if(entitiesHit[i]->getType().compare("itemSpawn")!=0 && entitiesHit[i]->getType().compare("SpawnPoint")!=0
					&& entitiesHit[i]->getType().compare("ElevatorTrigger")!=0)
					hitsValidos++;
			}
		}
		if(nbHits>0) delete [] entitiesHit;
		//Tambien un rayo por si salieramos detrás de la pared (solo si el overlap no tiene queja)
		//Desde la posicion de choque con la capsula hasta el punto donde saldría el cohete
		bool noDistance=false;
		if(hitsValidos==0){
			float distance=capsulleCollisionPoint.distance(separationPoint);
			Ray ray(capsulleCollisionPoint, directionNormalise);
			if(Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, distance,_entity->getEntityID()) != NULL){
				std::cout << "El rayo toca por lo que hay que lanzar un cohete falso"<< std::endl;
				noDistance=true;
			}
		}
		//Si el overlap chocó o traspasabamos algo al intentar posicionarnos
		if(hitsValidos > 0 || noDistance){
			//Lanzamos el cohete falso
			std::cout << "Lanzamos un cohete falso" << std::endl;	
			Map::CEntity *entityInfoRocketExplotion = CEntityFactory::getSingletonPtr()->getInfo("RocketExplotion");
			float yaw=Math::fromRadiansToDegrees(_entity->getYaw())+180; //+180 porque esta del reves el modelo cohete	
			float pitch=360-Math::fromRadiansToDegrees(_entity->getPitch());
			CEntity* rocketExplotion = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
				entityInfoRocketExplotion, Logic::CServer::getSingletonPtr()->getMap(), separationPoint, yaw, pitch);
			assert(rocketExplotion != NULL);
			rocketExplotion->activate();
			CRocketController* comp = rocketExplotion->getComponent<CRocketController>("CRocketController");
			assert(comp != NULL);
			// Seteamos la entidad que dispara el cohete
			comp->setOwner(_entity);
			//Mandamos un mensaje de contacto
			std::shared_ptr<CMessageContactEnter> msg = std::make_shared<CMessageContactEnter>();
			msg->setEntity(_entity->getEntityID());
			rocketExplotion->emitMessage(msg);
			return;
		}

		//Paso4
		//Si el overlap fue bien y no tocamos con nada, es el momento de crear el cohete en la posicion validada
		//Ojo con las separaciones extra por el rollo de la mierder interpolation y el papapatrás ( lo mismo hay que modificar la posicion del paso 2 )
		std::cout << "Cohetes normales" << std::endl;
		Map::CEntity *entityInfoRocket = CEntityFactory::getSingletonPtr()->getInfo("Rocket");
		float yaw=Math::fromRadiansToDegrees(_entity->getYaw())+180; //+180 porque esta del reves el modelo cohete	
		float pitch=360-Math::fromRadiansToDegrees(_entity->getPitch());
		CEntity* rocket = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
			entityInfoRocket, Logic::CServer::getSingletonPtr()->getMap(), separationPoint, yaw, pitch);
		assert(rocket != NULL);
		rocket->activate();

		CRocketController* comp = rocket->getComponent<CRocketController>("CRocketController");
		assert(comp != NULL);
		// Seteamos la entidad que dispara el cohete
		comp->setOwner(_entity);

		// Mandar mensaje add force / o hacerlo a lo bestia todo?
		
		std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
		forceMsg->setForce(directionNormalise* _shootForce, Physics::ForceMode::eFORCE );
		forceMsg->setGravity(false);
		rocket->emitMessage(forceMsg);


		/*//ANTIGUO COHETE DESPUES DE CONSEGUIR XUSKERAMENTE KE MEDIO FUESE, no quitar hasta estar seguros
		//std::cout << "Posicion del jugador x,y,z: " << _entity->getPosition().x << "," << _entity->getPosition().y <<"," << _entity->getPosition().z << std::endl;
		//Calculamos la situacion de origen del cohete
		directionNormalise=Math::getDirection( _entity->getOrientation());
		directionNormalise.normalise();
		Vector3 shootPosition = _entity->getPosition() + directionNormalise* (_capsuleRadius+6.0);//2 es el radio del cohete y lo demas es la separacion para que vaya tanto en sp como mp (basura de interpolacion)
		shootPosition.y += _heightShoot; //Altura del pj menos el radio del cohete para que salga en el centro de la mira
		//std::cout << "Posicion del cohete a disparar x,y,z: " << shootPosition.x <<"," << shootPosition.y <<"," << shootPosition.z << std::endl;

		//Comprobamos si el misil tiene espacio para ser disparado
		//Creamos el origen del rayo que sera igual al de la posicion de disparo menos el desplazamiento
		Vector3 origin = _entity->getPosition()+Vector3(0,_heightShoot,0);
		Vector3 noSpacePosition=origin;
		//Calculamos la distancia entre la posicion de disparo y el origen
		float distance=origin.distance(shootPosition);
		// Creamos el ray desde el origen en la direccion del raton
		Vector3 direction=Math::getDirection(_entity->getOrientation());
		direction.normalise();
		Ray ray1(origin, direction);
		//Desde el centro del jugador en la dirección que será disparado el misil sin tenernos en cuenta a nosotros mismos.
		//Si hemos tocado algo es que no hay espacio por lo tanto lo lanzamos desde el centro del jugador (creo que seria mejor explotarlo directamente por movidas de shapes)
		if(Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray1, distance,_entity->getEntityID()) != NULL){
			std::cout << "El cohete no tiene espacio para salir"<< std::endl;
			shootPosition=noSpacePosition;
		}

		float yaw=Math::fromRadiansToDegrees(_entity->getYaw())+180; //+180 porque esta del reves el modelo cohete	
		float pitch=360-Math::fromRadiansToDegrees(_entity->getPitch());

		CEntity* rocket = CEntityFactory::getSingletonPtr()->createEntityWithPositionAndOrientation(
			entityInfo, Logic::CServer::getSingletonPtr()->getMap(), shootPosition, yaw, pitch);
		
		assert(rocket != NULL);
		
		rocket->activate();

		CRocketController* comp = rocket->getComponent<CRocketController>("CRocketController");
		assert(comp != NULL);
		// Seteamos la entidad que dispara el cohete
		comp->setOwner(_entity);

		// Mandar mensaje add force
		std::shared_ptr<CMessageAddForcePhysics> forceMsg = std::make_shared<CMessageAddForcePhysics>();
		forceMsg->setForce( (Math::getDirection( _entity->getOrientation()) * _shootForce), Physics::ForceMode::eFORCE );
		forceMsg->setGravity(false);
		rocket->emitMessage(forceMsg);*/
	}


} // namespace Logic

