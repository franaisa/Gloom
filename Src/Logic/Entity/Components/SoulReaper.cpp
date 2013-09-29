/**
@file SoulReaper.cpp

Contiene la implementación del componente que representa al soulReaper.
 
@see Logic::CSoulReaper
@see Logic::CShootRaycast

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "SoulReaper.h"
#include "Physics/RaycastHit.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/PullingMovement.h"
#include "Logic/Entity/Components/SpawnItemManager.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/PhysicDynamicEntity.h"
#include "Logic/Entity/Components/PhysicStaticEntity.h"
#include "Logic/GameNetPlayersManager.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddForcePlayer.h"
#include "Logic/GameNetMsgManager.h"

#include "Graphics/Camera.h"


#include <OgreMeshManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>
#include <OgreCodec.h>

namespace Logic {
	IMP_FACTORY(CSoulReaper);

	CSoulReaper::~CSoulReaper() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CSoulReaper::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		Map::CEntity* weapon = CEntityFactory::getSingletonPtr()->getInfo(_weaponName);

		if( !IWeapon::spawn(entity, map, weapon) ) return false;

		
		_shotsDistanceSecondaryFire = weapon->getFloatAttribute("ShotsDistanceSecondaryFire");

		_shotsDistance = weapon->getFloatAttribute("ShotsDistance");

		_defaultPrimaryFireDamage = _primaryFireDamage = weapon->getFloatAttribute("PrimaryFireDamage");

		_defaultPrimaryFireCooldown = _primaryFireCooldown = weapon->getFloatAttribute("PrimaryFireCooldown") * 1000;


		return true;
	}

	//__________________________________________________________________

	void CSoulReaper::onActivate() {
		/*
		//Reiniciamos el cooldown
		reduceCooldown(0);
		//y los daños
		amplifyDamage(0);
		*/
	}

	//__________________________________________________________________

	void CSoulReaper::resetAmmo() {
		//si yo soy el weapon
		_currentAmmo = 1;
	} // resetAmmo
	//__________________________________________________________________



	void CSoulReaper::primaryFire() {

		_primaryFireTimer = _primaryFireCooldown;
	
		Vector3 direction = _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
		direction.normalise();

		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);

		Ray ray(origin, direction);
			
		std::vector <Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _shotsDistance, hits, true, Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eWORLD | Physics::CollisionGroup::eSCREAMER_SHIELD);

		bool targetHit = false;
		for (auto it = hits.begin(); it < hits.end(); ++it){
			//Si tocamos el mundo no continuamos viendo hits
			if((*it).entity->getType().compare("PhysicWorld")==0){
				Euler euler(Quaternion::IDENTITY);
				euler.setDirection(it->normal);
				euler.yaw( Ogre::Radian(Math::HALF_PI) );

				Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BulletSpark");
				CEntity* bulletSpark = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), it->impact, euler.toQuaternion() );
				bulletSpark->activate();
				bulletSpark->start();

				int randomValue = Math::unifRand(2);
				std::string hitSound = (randomValue == 1 ? "weapons/soulReaper/wallHit.wav" : "weapons/soulReaper/wallHit2.wav");
				emitSound(hitSound, false, true, false, false);

				targetHit = true;
				break;
			}
			if((*it).entity != _entity && (*it).entity->getType() != "ScreamerShield"){
				//send damage message
				CGameNetPlayersManager* playersMgr = CGameNetPlayersManager::getSingletonPtr();
				TEntityID enemyId = (*it).entity->getEntityID();
				TEntityID playerId = _entity->getEntityID();
				if( playersMgr->existsByLogicId(enemyId) ) {
					TeamFaction::Enum enemyTeam = playersMgr->getTeamUsingEntityId(enemyId);
					TeamFaction::Enum myTeam = playersMgr->getTeamUsingEntityId(playerId);

					if( !playersMgr->friendlyFireIsActive() && enemyId != playerId) {
						if(enemyTeam == TeamFaction::eNONE || myTeam == TeamFaction::eNONE || enemyTeam != myTeam) {
							std::shared_ptr<CMessageDamaged> damageDone = std::make_shared<CMessageDamaged>();
							damageDone->setDamage(_primaryFireDamage);
							damageDone->setEnemy( _entity );
							(*it).entity->emitMessage(damageDone);
						}
					}
					else {
						std::shared_ptr<CMessageDamaged> damageDone = std::make_shared<CMessageDamaged>();
						damageDone->setDamage(_primaryFireDamage);
						damageDone->setEnemy( _entity );
						(*it).entity->emitMessage(damageDone);
					}
				}
				else {
					std::shared_ptr<CMessageDamaged> damageDone = std::make_shared<CMessageDamaged>();
					damageDone->setDamage(_primaryFireDamage);
					damageDone->setEnemy( _entity );
					(*it).entity->emitMessage(damageDone);
				}
				
				// Pintamos las particulas de sangre
				Euler euler(Quaternion::IDENTITY);
				euler.setDirection((*it).normal);

				Map::CEntity* entityInfo = CEntityFactory::getSingletonPtr()->getInfo("BloodStrike");
				CEntity* bloodStrike = CEntityFactory::getSingletonPtr()->createEntity(entityInfo, _entity->getMap(), (*it).impact, euler.toQuaternion() );
				bloodStrike->activate();
				bloodStrike->start();

				emitSound("weapons/soulReaper/goreHit.wav", false, true, false, false);

				// esto es para que salga una vez que ya le ha dao a alguien que no eres tu mismo.
				targetHit = true;
				break;
			}
		}

		if(!targetHit)
			emitSound("weapons/soulReaper/miss.wav", false, true, false, false);
	} // primaryFire
	//__________________________________________________________________

	void CSoulReaper::secondaryFire() {
		
		//primero preguntamos si podemos atraer algun arma
		_elementPulled = checkPullItem();

		//si no podemos atraer nada, no hacemos nada
		if(!_elementPulled){
			return;
		}
		//cogemos la entidad estatica y la desactivamos
		_elementPulled->deactivate();
		CGameNetMsgManager::getSingletonPtr()->sendDeactivateEntity(_elementPulled->getEntityID());
		//nos creamos una nueva entidad como la que hemos cogido pero dinamica,
		//para ello cogemos la informacion basica de la entidad dinamica y la
		//rellenamos con la información de la entidad que estamos creando
		Map::CEntity* info = new Map::CEntity(*CEntityFactory::getSingletonPtr()->getInfo(_elementPulled->getType()+"Dynamic"));

		std::string weapon, mesh, reward;

		auto componentCSpawnItemManager= _elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager");
		std::stringstream weaponaux ;
		weaponaux <<  componentCSpawnItemManager->getWeaponType();
		weapon = weaponaux.str();

		std::stringstream rewardaux;
		rewardaux <<  componentCSpawnItemManager->getReward();
		reward = rewardaux.str();

		//info->setAttribute("model",_elementPulled->getComponent<CGraphics>("CGraphics")->getMeshName());
		info->setAttribute("weaponType", weapon);
		info->setAttribute("reward", reward);
		info->setAttribute("id", componentCSpawnItemManager->getId());
		
		Map::CEntity* clientEntityInfo = new Map::CEntity( info->getName() );
		clientEntityInfo->setAttribute("model",componentCSpawnItemManager->getModel());

		//creamos la entidad con la información obtenida
		CEntity * dynamicItem = CEntityFactory::getSingletonPtr()->createCustomClientEntity(info,
																				clientEntityInfo,
																				Logic::CServer::getSingletonPtr()->getMap(),
																				_elementPulled->getPosition(),
																				_elementPulled->getOrientation());

		if(!dynamicItem){
			std::cout << "ALGO HA IDO MAL Y NO SE HA CREADO LA ENTIDAD" << std::endl;
			return;
		}
		
		_elementPulling = dynamicItem;

		//por ultimo, ponemos a la entidad donde debe estar
		_elementPulling->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity")->setGlobalPose(_elementPulled->getPosition(),_elementPulled->getOrientation(), true);

		//le metemos donde estamos para que nos siga
		_elementPulling->getComponent<CPullingMovement>("CPullingMovement")->setPlayer(_entity);

		_elementPulling->activate();
		_elementPulling->start();

		delete info;
	} // secondaryShoot
	//__________________________________________________________________

	CEntity * CSoulReaper::checkPullItem() {
		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, (_entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z).normalisedCopy());

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		Physics::CRaycastHit hit;
		int nbHits = 0;
		//drawRaycast(ray);
		
		bool valid = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _shotsDistanceSecondaryFire, hit,Physics::CollisionGroup::eITEM);
		
		if(!valid)
			return NULL;
		CEntity *entity = hit.entity;

		if(entity->getType()!="ItemSpawn")
			return NULL;

		return entity;
	} // checkPullItem
	//__________________________________________________________________

	void CSoulReaper::stopSecondaryFire(){
		//si cuando hice click no cogi nada no puedo hacer nada aqui
		if(!_elementPulling)
			return;
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_elementPulling, true);
		_elementPulled->activate();
		CGameNetMsgManager::getSingletonPtr()->sendActivateEntity(_elementPulled->getEntityID());
		_elementPulling = NULL;
	} // stopSecondaryFire
	//__________________________________________________________________

	void CSoulReaper::resetEntityPulling(){
		_elementPulled->activate();
		CGameNetMsgManager::getSingletonPtr()->sendActivateEntity(_elementPulled->getEntityID());
		_elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager")->beginRespawn();
		std::shared_ptr<CMessageActivate> deactivateMsg = std::make_shared<CMessageActivate>();
		deactivateMsg->setActivated(false);
		_elementPulled->emitMessage(deactivateMsg);

		_elementPulling = NULL;

		//vamos a decirle al spawnitem original que le han cogido, diciendole
		//que comience su periodo de respawn y desactivando su fisica
		_elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager")->beginRespawn();
		_elementPulled->getComponent<CPhysicStaticEntity>("CPhysicStaticEntity")->deactivateSimulation();

		//nos liberamos del puntero
		_elementPulled=NULL;
	} // resetEntityPulling
	//__________________________________________________________________

	void CSoulReaper::amplifyDamage(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto
		if(percentage == 0) {
			_primaryFireDamage = _defaultPrimaryFireDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireDamage += percentage * _primaryFireDamage * 0.01f;
		}

	} // amplifyDamage
	//__________________________________________________________________

	void CSoulReaper::onTick(unsigned int msecs) {
		//el tick solo me interesa cuando estoy cogiendo algo
		if(!_elementPulling)
			return;


		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);

		// 3.5 mirao a ojo a traves del visual debbuger 
		Physics::SphereGeometry sphere  = Physics::CGeometryFactory::getSingletonPtr()->createSphere(3.5);
		std::vector<Physics::CSweepHit> hitSpots;
		Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, origin,(_entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z).normalisedCopy(), _shotsDistanceSecondaryFire,hitSpots, false, Physics::CollisionGroup::eITEM );
		
		if ( hitSpots.size() < 1 ){
			stopSecondaryFire();
			return;
		}
		
		//recorremos el resultado del sweep en busca de nuestro amado item
		CEntity * item = 0;

		for(auto it = hitSpots.begin(); it < hitSpots.end(); ++it){
			if( (*it).entity->getType() == "ItemSpawnDynamic"){
				item =(*it).entity;
				break;
			}
		}	

		if(!item){//si no lo hemos encontrado en el sweep, es que estamos fuera de rango
			stopSecondaryFire();
			return;
		}

		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, ( item->getPosition() - origin ).normalisedCopy());

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		Physics::CRaycastHit hit;
		int nbHits = 0;
		//drawRaycast(ray);
		
		bool valid = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _shotsDistanceSecondaryFire+1, hit,Physics::CollisionGroup::eITEM);
		
		if(!valid){
			stopSecondaryFire();
			return;
		}
		CEntity *entity = hit.entity;
		
		if( entity->getType()=="ItemSpawnDynamic" || entity->getType()=="ItemSpawn" )
			return;
		stopSecondaryFire();
	} // onTick
	//__________________________________________________________________
} // namespace Logic

