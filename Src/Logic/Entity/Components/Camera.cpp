/**
@file Camera.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

#include "Life.h"

#include "Logic/Messages/MessageCameraToEnemy.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("distance"))
			_distance = entityInfo->getFloatAttribute("distance");

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

		if(entityInfo->hasAttribute("targetDistance"))
			_targetDistance = entityInfo->getFloatAttribute("targetDistance");

		if(entityInfo->hasAttribute("targetHeight"))
			_targetHeight = entityInfo->getFloatAttribute("targetHeight");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CCamera::activate()
	{
		IComponent::activate();
		
		_target = CServer::getSingletonPtr()->getPlayer();

		if(!_target)
			deactivate();

		//return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		IComponent::deactivate();

		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

		bool CCamera::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CAMERA_TO_ENEMY;
	} // accept
	//---------------------------------------------------------


	void CCamera::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::CAMERA_TO_ENEMY:
					setTargetEnemy(((CMessageCameraToEnemy*)message)->getEnemy());
			break;
		}

	} // process
	//---------------------------------------------------------

	void CCamera::setTargetEnemy(CEntity *enemy){
		_enemy=enemy;
	}



	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
		if(_target)
		{
			// Actualizamos la posición de la cámara.
			Vector3 position = _target->getPosition();
			Vector3 direction = -_distance * Math::getDirection(_target->getOrientation());
			direction.y = _height;
			_graphicsCamera->setCameraPosition(position + direction);

			if(_target->getComponent<CLife>("CLife")->isActivate()){
				// Y la posición hacia donde mira la cámara.
				direction = _targetDistance * Math::getDirection(_target->getOrientation());
				direction.y += _targetHeight;
				_graphicsCamera->setTargetCameraPosition(position + direction);
			}
			else if(_enemy){
				 if(_enemy->getName().compare("David")!=0)
				    _graphicsCamera->setTargetCameraPosition(_enemy->getPosition() );
				 else{	
					  _graphicsCamera->setCameraPosition(_enemy->getPosition()+Vector3(0,50,0));
					 _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 }
			}
			//Implementacion del grupo de Calm night of nose que :P
			/*
			Vector3 position = _target->getPosition();
			position.y += _height;
			_graphicsCamera->setCameraPosition(position);

			Vector3 direction = Math::getDirection(_target->getOrientation());
			_graphicsCamera->setTargetCameraPosition(position + direction);
			*/
		}

	} // tick

} // namespace Logic

