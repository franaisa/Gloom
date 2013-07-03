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
#include "ShotGun.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Logic/Maps/EntityFactory.h"

// Fisica
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddAmmo.h"
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
	
	void CMagneticBullet::setOwner(CShotGun *owner)
	{
		_owner = owner;
	} // setOwner
	//----------------------------------------------------------

	void CMagneticBullet::impact(CEntity *impactEntity)
	{
		//printf("\nSoy %d Y Impacto con %s", _entity->getName().c_str(), impactEntity->getName().c_str());
		if(impactEntity->getType() == "World"){
			// Por ahora le paso x quien me he muerto, en un futuro deberian estar los decals en mas facil acceso, como en graphics y ya ta :D
			if(_owner)
				_owner->destroyProjectile(_entity, impactEntity);
			else
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity, true);
		}else{
			if( impactEntity == _owner->getEntity() ){
				if(_returning){
					std::shared_ptr<CMessageAddAmmo> addAmmoMsg = std::make_shared<CMessageAddAmmo>();
					addAmmoMsg->setAddAmmo(1);
					addAmmoMsg->setAddWeapon(WeaponType::eSHOTGUN);
					_owner->getEntity()->emitMessage(addAmmoMsg);
					//_owner->addAmmo(WeaponType::eSHOTGUN,1,true);
					_owner->destroyProjectile(_entity, impactEntity);
				}
			}else{
				if(impactEntity->getType() == "FireBall")
				{
					_burned = true;
					// lo suyo seria cambiar el efecto y ponerle algo guapo para que se vea q esta incendia
				}else{
					std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
					m->setDamage(_damage);
					if(_owner)
						m->setEnemy(_owner->getEntity());
					impactEntity->emitMessage(m);

					if(_burned){
						//Envio el mensaje que daño de fuego tb.

					}
				}
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
			/*
			_projectileDirection = Math::getDirection(_owner->getEntity()->getOrientation());
			/*/
			_projectileDirection = (_owner->getEntity()->getPosition()+(Vector3(1.5,_heightShoot*0.5,0)) - _entity->getPosition());
			_projectileDirection.normalise();
			/* */
		}
		_physicComponent->move(_projectileDirection * _speed);
	} // tick
	//----------------------------------------------------------



} // namespace Logic


