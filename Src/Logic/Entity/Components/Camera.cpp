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

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

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
			position.y+=_height;
			_graphicsCamera->setCameraPosition(position);

			if(_target->getComponent<CLife>("CLife")->isActivate()){
				// Y la posición hacia donde mira la cámara.
				Vector3 direction = Math::getDirection(_target->getOrientation());
				_graphicsCamera->setTargetCameraPosition(position + direction);
			}
			//Si estamos muertos miramos al enemigo, diferenciamos entre nosotros mismos o el rival (ojo con que el rival sea de tipo player)
			else if(_enemy){
				 if(_enemy->getType().compare("Player")!=0)
				    _graphicsCamera->setTargetCameraPosition(_enemy->getPosition() );
				 else{	
					  _graphicsCamera->setCameraPosition(_enemy->getPosition()+Vector3(0,50,0));
					 _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 }
			}

		}

	} // tick

} // namespace Logic

