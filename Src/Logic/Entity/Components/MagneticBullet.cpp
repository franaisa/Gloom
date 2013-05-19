/**
@file MagneticBullet.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CMagneticBullet
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Mayo, 2013
*/

// Logica
#include "MagneticBullet.h"
#include "ShootShotGun.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Fisica
#include "Logic/Messages/MessageTouched.h"
#include "PhysicDynamicEntity.h"



namespace Logic 
{
	IMP_FACTORY(CMagneticBullet);
	
	//---------------------------------------------------------
	
	
	bool CMagneticBullet::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity"); 

		

		return true;

	} // spawn
	//---------------------------------------------------------

	bool CMagneticBullet::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::TOUCHED;
		//return message->getMessageType() == Message::DAMAGED;
		
	} // accept
	//---------------------------------------------------------

	void CMagneticBullet::process(const std::shared_ptr<CMessage>& message)
	{
		
		switch(message->getMessageType())
		{
			case Message::TOUCHED:
				auto msg = std::static_pointer_cast<CMessageTouched>(message);
				impact(msg->getEntity());
		}
		

	} // process
	//----------------------------------------------------------
	
	void CMagneticBullet::setOwner(CShootShotGun *owner)
	{
		_owner = owner;
	} // setOwner
	//----------------------------------------------------------

	void CMagneticBullet::impact(CEntity *impactEntity)
	{
		printf("\nSoy %d Y Impacto con %s", _entity->getName().c_str(), impactEntity->getName().c_str());

		if(impactEntity->getType() == "World"){
			_owner->destroyProjectile(_entity);
		}else{
			if(_returning && impactEntity->getType() == _owner->getEntity()->getType()){
				// no me gusta poner que el arma es el dos, pero bueno
				_owner->addAmmo(2,1,true);
				_owner->destroyProjectile(_entity);
			}
		}
	} // impact
	//----------------------------------------------------------

	void CMagneticBullet::setProjectileDirection(Vector3 projectileDirection){
		_projectileDirection = projectileDirection;
		_returning = true;
	} // setProjectileDirection
	//----------------------------------------------------------



	void CMagneticBullet::onFixedTick(unsigned int msecs)
	{
		if(_returning){
			//_projectileDirection = Math::getDirection(_owner->getEntity()->getOrientation());
			
			/*
			Vector3 aux = _owner->getEntity()->getPosition();
			aux += (Vector3(0,_heightShoot,0));
			Vector3 aux2 = _entity->getPosition();
			*/
			_projectileDirection = (_owner->getEntity()->getPosition()+(Vector3(0,_heightShoot,0)) - _entity->getPosition());
			_projectileDirection.normalise();
		}
		_physicComponent->move(_projectileDirection * _speed);
	} // tick
	//----------------------------------------------------------



} // namespace Logic


