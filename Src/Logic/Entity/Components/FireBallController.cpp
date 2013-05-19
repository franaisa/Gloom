/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "FireBallController.h"
#include "IronHellGoat.h"
#include "PhysicDynamicEntity.h"

#include "Logic/Maps/EntityFactory.h"
#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"

#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageDamaged.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CFireBallController);

	//________________________________________________________________________

	CFireBallController::CFireBallController() { 
		// Nada que hacer
	}

	//________________________________________________________________________

	CFireBallController::~CFireBallController() {
		// Le indicamos al componente dueño que nos vamos a destruir
		// y que por lo tanto ya no vamos a poder recibir mas instrucciones
		// de move
		if(_owner != NULL) {
			_owner->removeFireBall(this);
			_owner = NULL;
		}
	}

	//________________________________________________________________________

	void CFireBallController::onStart() {
		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity");
		assert(_physicComponent != NULL && "Error: La bola de fuego no tiene collider");
	}

	//________________________________________________________________________

	bool CFireBallController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("speed") );
		assert( entityInfo->hasAttribute("damage") );
		assert( entityInfo->hasAttribute("explotionRadius") );
		assert( entityInfo->hasAttribute("direction") );

		_speed = entityInfo->getFloatAttribute("speed");
		_damage = entityInfo->getFloatAttribute("damage");
		_explotionRadius = entityInfo->getFloatAttribute("explotionRadius");
		_direction = entityInfo->getVector3Attribute("direction");

		return true;
	}

	//________________________________________________________________________

	bool CFireBallController::accept(const shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTACT_ENTER;
	}

	//________________________________________________________________________

	void CFireBallController::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTACT_ENTER: {
				createExplotion();
			}
		}
	}

	//________________________________________________________________________

	void CFireBallController::onFixedTick(unsigned int msecs) {
		// Muevo a la bola de fuego
		_physicComponent->move( _direction * _speed * msecs );
	}
	
	//________________________________________________________________________

	void CFireBallController::setOwner(CIronHellGoat* owner) {
		this->_owner = owner;
	}

	//________________________________________________________________________

	void CFireBallController::createExplotion() {
		// Generar un overlap y mandar los mensajes de daño a las entidades
		// con las que se impacte
		vector<CEntity*> entitiesHit;
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_explotionRadius);
		Vector3 explotionPos = _entity->getPosition();
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit);

		for(int i = 0; i < entitiesHit.size(); ++i) {
			if( entitiesHit[i] != NULL && entitiesHit[i]->isPlayer() ) {
				// Mandamos el mensaje de daño
				estimateDamage(entitiesHit[i], explotionPos);
			}
		}

		// Destruir en diferido esta entidad
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity, true);
	}

	//________________________________________________________________________

	void CFireBallController::estimateDamage(CEntity* entityHit, const Vector3& explotionPos) {
		// Comprobamos el punto de contacto
		Vector3 rayDirection = entityHit->getPosition();

		// Tiramos el rayo al centro de la capsula del enemigo
		// No podemos tirar el rayo directamente porque se lo tira a los pies y a veces no hay contacto
		// Como solucion rapida (aehem.. ñapa) voy a sacar la info de la altura del screamer
		// que teoricamente es la misma que la de todos los players
		Map::CEntity * info = Logic::CEntityFactory::getSingletonPtr()->getInfo("Screamer");
		if(info != NULL) rayDirection.y += info->getIntAttribute("physic_height");
		rayDirection = rayDirection - explotionPos;
				
		// Hacemos una query de raycast desde el punto de explosion hasta la entidad analizada
		// de la query de overlap (para saber el punto de colision).
		// No es ni lo mas exacto ni lo mas eficiente, pero soluciona la papeleta.
		Ogre::Ray ray( explotionPos, rayDirection.normalisedCopy() );

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
		damageDone->setEnemy( _owner->getEntity() );
		entityHit->emitMessage(damageDone);
	}

} // namespace Logic

