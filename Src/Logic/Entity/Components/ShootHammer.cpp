/**
@file ShootHammer.cpp

Contiene la implementación del componente que representa al hammer.
 
@see Logic::CShootHammer
@see Logic::CShootRaycast
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
#include "Logic/Messages/MessageRebound.h"

#include "Graphics/Camera.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	IMP_FACTORY(CShootHammer);
	
	bool CShootHammer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShootRaycast::spawn(entity,map,entityInfo)) return false;
		
		_currentAmmo = 1;
		_damageReflect = entityInfo->getIntAttribute("weapon"+_nameWeapon+"DamageReflect");

		return true;
	}

	//__________________________________________________________________


	void CShootHammer::decrementAmmo() {
		// Redefinimos el metodo para que no se haga nada ya que el hammer
		// realmente no tiene municion
	}// decrementAmmo
	
	//__________________________________________________________________

	void CShootHammer::triggerHitMessages(std::pair<CEntity*, Ray> entityHit) {
		if(entityHit.first->getType().compare("World")==0){
			Vector3 direccionOpuestaRay= entityHit.second.getDirection()*-1;
			Logic::CMessageRebound *m=new Logic::CMessageRebound();
			m->setDir(direccionOpuestaRay);
			_entity->emitMessage(m);

			Logic::CMessageDamaged *damage=new Logic::CMessageDamaged();
			damage->setDamage(_damageReflect);
			damage->setEnemy(_entity);
			_entity->emitMessage(damage);

		}
		// LLamar al componente que corresponda con el daño hecho (solamente si no fuera el mundo el del choque)
		else{
			Logic::CMessageDamaged *m=new Logic::CMessageDamaged();
			m->setDamage(_damage);
			m->setEnemy(_entity);
			entityHit.first->emitMessage(m);
		}
	}// triggerHitMessages
	//__________________________________________________________________

} // namespace Logic

