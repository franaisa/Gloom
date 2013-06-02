/**
@file ShootHammer.cpp

Contiene la implementación del componente que representa al hammer.
 
@see Logic::CShootHammer
@see Logic::CShootRaycast

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "ShootHammer.h"
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
#include "Logic/Entity/Components/Shoot.h"
#include "Logic/Entity/Components/SpawnItemManager.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/PhysicDynamicEntity.h"
#include "Logic/Entity/Components/PhysicStaticEntity.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddForcePlayer.h"

#include "Graphics/Camera.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	IMP_FACTORY(CShootHammer);

	CShootHammer::~CShootHammer() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CShootHammer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootRaycast::spawn(entity,map,entityInfo)) return false;
		
		_currentAmmo = 1;
		_distance = 10000;
		return true;
	}

	//__________________________________________________________________


	void CShootHammer::decrementAmmo() {
		// Redefinimos el metodo para que no se haga nada ya que el hammer
		// realmente no tiene municion
	}// decrementAmmo
	
	//__________________________________________________________________

	void CShootHammer::resetAmmo() {
		//si yo soy el weapon
		_currentAmmo = 1;
	} // resetAmmo

	//__________________________________________________________________

	void CShootHammer::secondaryShoot() {

		//primero preguntamos si podemos atraer algun arma
		_elementPulled = fireSecondary();

		//si no podemos atraer nada, no hacemos nada
		if(!_elementPulled)
			return;
		
		//cogemos la entidad estatica y la desactivamos
		_elementPulled->deactivate();

		//nos creamos una nueva entidad como la que hemos cogido pero dinamica,
		//para ello cogemos la informacion basica de la entidad dinamica y la
		//rellenamos con la información de la entidad que estamos creando
		Map::CEntity* info = new Map::CEntity(*CEntityFactory::getSingletonPtr()->getInfo(_elementPulled->getType()+"Dynamic"));

		std::string weapon, mesh, reward;

		std::stringstream weaponaux;
		weaponaux <<  _elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager")->getWeaponType();
		weapon = weaponaux.str();

		std::stringstream rewardaux;
		rewardaux <<  _elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager")->getReward();
		reward = rewardaux.str();

		info->setAttribute("model",_elementPulled->getComponent<CGraphics>("CGraphics")->getMeshName());
		info->setAttribute("id", weapon);
		info->setAttribute("reward", reward);

		//creamos la entidad con la información obtenida
		CEntity * dynamicItem = CEntityFactory::getSingletonPtr()->createEntity(info,
																				Logic::CServer::getSingletonPtr()->getMap(), 
																				true);

		if(!dynamicItem){
			std::cout << "ALGO HA IDO MAL Y NO SE HA CREADO LA ENTIDAD" << std::endl;
			return;
		}
		
		_elementPulling = dynamicItem;

		//por ultimo, ponemos a la entidad donde debe estar
		_elementPulling->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity")->setTransform(_elementPulled->getTransform(), true);

		//le metemos donde estamos para que nos siga
		_elementPulling->getComponent<CPullingMovement>("CPullingMovement")->setPlayer(_entity);

		delete info;
	} // secondaryShoot

	//__________________________________________________________________

	CEntity * CShootHammer::fireSecondary() {
		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, (_entity->getQuatOrientation()*Vector3::NEGATIVE_UNIT_Z).normalisedCopy());

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		Physics::CRaycastHit hit;
		int nbHits = 0;
		if(!Physics::CServer::getSingletonPtr()->raycastSingle(ray, _distance, hit))
			return NULL;
		CEntity *entity = hit.entity;

		if(entity->getType()!="ItemSpawn")
			return NULL;

		return entity;
	}

	void CShootHammer::stopSecondaryShoot(){
		//si cuando hice click no cogi nada no puedo hacer nada aqui
		if(!_elementPulling)
			return;
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_elementPulling, true);
		_elementPulled->activate();
	}

	void CShootHammer::resetEntityPulling(){
		_elementPulling = NULL;

		//vamos a decirle al spawnitem original que le han cogido, diciendole
		//que comience su periodo de respawn y desactivando su fisica
		_elementPulled->getComponent<CSpawnItemManager>("CSpawnItemManager")->beginRespawn();
		_elementPulled->getComponent<CPhysicStaticEntity>("CPhysicStaticEntity")->deactivateSimulation();

		//nos liberamos del puntero
		_elementPulled=NULL;
	}

} // namespace Logic

