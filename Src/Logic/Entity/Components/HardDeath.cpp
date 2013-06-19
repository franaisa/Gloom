/**
@file CoolDown.cpp

Contiene la implementación del componente que representa al coolDown.
 
@see Logic::CCoolDown

@author Jose Antonio García Yáñez
@date Febrero,2013
*/

#include "HardDeath.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

#include "Logic/Messages/MessageReducedCooldown.h"

#include "PhysicDynamicEntity.h"

#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageDamaged.h"


namespace Logic {
	IMP_FACTORY(CHardDeath);

	CHardDeath::~CHardDeath() {
		// Nada que hacer
	}

	//__________________________________________________________________
	
	bool CHardDeath::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!ISpell::spawn(entity,map,entityInfo)) return false;

		//assert( entityInfo->hasAttribute(_spellName + "percentage") );
		assert( entityInfo->hasAttribute("heightShoot") );
		assert( entityInfo->hasAttribute(_spellName + "DamageExplotion") );
		assert( entityInfo->hasAttribute(_spellName + "ExplotionRadius") );


		_heightShoot = entityInfo->getFloatAttribute("heightShoot");
		_damage = entityInfo->getFloatAttribute(_spellName + "DamageExplotion");
		_explotionRadius = entityInfo->getFloatAttribute(_spellName + "ExplotionRadius");

		return true;
	} // spawn
	//__________________________________________________________________

	void CHardDeath::spell(){ 
			createExplotion();
	} // spell
	//__________________________________________________________________
		
	void CHardDeath::stopSpell() {

	} // stopSpell
	//__________________________________________________________________

	void CHardDeath::createExplotion() {
		// Generar un overlap y mandar los mensajes de daño a las entidades
		// con las que se impacte
		std::vector<CEntity*> entitiesHit;
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_explotionRadius);
		Vector3 explotionPos = _entity->getPosition();
		// @deprecated hay que tener en cuenta el grupo del screamer shield, cuando esten
		// bien puestos los grupos lo meto
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit, Physics::CollisionGroup::ePLAYER);

		for(int i = 0; i < entitiesHit.size(); ++i) {
			// Mandamos el mensaje de daño
			if(entitiesHit[i]->getEntityID() != _entity->getEntityID())
				estimateDamage(entitiesHit[i], explotionPos);
		}
	}

	//________________________________________________________________________

	void CHardDeath::estimateDamage(CEntity* entityHit, const Vector3& explotionPos) {
		// Comprobamos el punto de contacto
		Vector3 rayDirection = entityHit->getPosition();

		// Tiramos el rayo al centro de la capsula del enemigo
		// No podemos tirar el rayo directamente porque se lo tira a los pies y a veces no hay contacto

		rayDirection.y += _heightShoot;
		rayDirection = rayDirection - explotionPos;
				
		// Hacemos una query de raycast desde el punto de explosion hasta la entidad analizada
		// de la query de overlap (para saber el punto de colision).
		// No es ni lo mas exacto ni lo mas eficiente, pero soluciona la papeleta.
		Ogre::Ray ray( explotionPos, rayDirection);

		std::vector<Physics::CRaycastHit> hitBuffer;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _explotionRadius, hitBuffer);
		int bufferSize = hitBuffer.size();

		// Calculamos el daño en base a la distancia del golpe
		float dmg = 0;
		for(int k = 0; k < bufferSize; ++k) {
			if(hitBuffer[k].entity == entityHit) {
				dmg = _damage * ( 1 - (hitBuffer[k].distance/_explotionRadius) );
			}
		}

		//send damage message
		std::shared_ptr<CMessageDamaged> damageDone = std::make_shared<CMessageDamaged>();
		damageDone->setDamage(dmg);
		damageDone->setEnemy( _entity );
		entityHit->emitMessage(damageDone);
	}

} // namespace Logic

