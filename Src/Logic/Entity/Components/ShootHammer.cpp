/**
@file ShootHammer.cpp

Contiene la implementación del componente que representa al hammer.
 
@see Logic::CShootHammer
@see Logic::CShootRaycast

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "ShootHammer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/Shoot.h"

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
		_damageReflect = entityInfo->getIntAttribute("weapon"+_nameWeapon+"DamageReflect");
		_reboundForce = entityInfo->getFloatAttribute("reboundForce");

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

	void CShootHammer::triggerHitMessages(std::pair<CEntity*, Ray> entityHit) {
		if(entityHit.first->getType().compare("World")==0){
			Vector3 reboundDirection= entityHit.second.getDirection()*-1;
			std::shared_ptr<CMessageAddForcePlayer> forceRebound = std::make_shared<CMessageAddForcePlayer>();
			forceRebound->setForce(reboundDirection*_reboundForce);
			_entity->emitMessage(forceRebound);

			std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
			damage->setDamage(_damageReflect);
			damage->setEnemy(_entity);
			_entity->emitMessage(damage);

		}
		// LLamar al componente que corresponda con el daño hecho (solamente si no fuera el mundo el del choque)
		else{
			std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
			m->setDamage(_damage);
			m->setEnemy(_entity);
			entityHit.first->emitMessage(m);
		}
	}// triggerHitMessages
	//__________________________________________________________________

} // namespace Logic

