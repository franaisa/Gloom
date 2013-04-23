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

#include "Interpolation.h"
#include "AvatarController.h"

#include "Logic/Messages/MessageCameraToEnemy.h"
#include "Logic/Messages/MessageCameraOffset.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		Vector3 camPos;
		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");
		if(entityInfo->hasAttribute("target"))
			_targetV = entityInfo->getVector3Attribute("target");
		if(entityInfo->hasAttribute("position")){
			camPos = entityInfo->getVector3Attribute("position");
			_graphicsCamera->setCameraPosition(camPos);
		}
		_dead=false;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CCamera::activate()
	{
		IComponent::activate();
		
		_target = CServer::getSingletonPtr()->getPlayer();

		if(!_target){
			_graphicsCamera->setTargetCameraPosition(_targetV);
			_target = NULL;
			//deactivate();
		}

		_fOffsetTimer = 0.0f;

		//return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		IComponent::deactivate();

		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	bool CCamera::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();
		
		return msgType == Message::CAMERA_TO_ENEMY ||
			   msgType == Message::PLAYER_SPAWN || 
			   msgType == Message::CAMERA_OFFSET;
	} // accept
	//---------------------------------------------------------


	void CCamera::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CAMERA_TO_ENEMY: {
				std::shared_ptr<CMessageCameraToEnemy> cameraToEnemyMsg = std::static_pointer_cast<CMessageCameraToEnemy>(message);
				setTargetEnemy( cameraToEnemyMsg->getEnemy() );
				//std::cout << "MENSAJE DE CAMERATOENEMY: " << std::endl;
				break;
			}
			case Message::PLAYER_SPAWN: {
				_dead=false;
				//std::cout << "mensaje respawn recibido" << std::endl;
				break;
			}
			case Message::CAMERA_OFFSET: {
<<<<<<< HEAD
				std::shared_ptr<CMessageCameraOffset> camOffset = std::static_pointer_cast<CMessageCameraOffset>(message);
				_fOffsetTimer = camOffset->getOffsetTimer(); //asignamos el tiempo del offset
=======
				_bCameraOffset = true;
				_fOffsetTimer = 2000.0f; 
				std::shared_ptr<CMessageCameraToEnemy> cameraToEnemyMsg = std::static_pointer_cast<CMessageCameraToEnemy>(message);

>>>>>>> 2374cb4a86f485e369637a785a3f40d78157ba67
				break;
			}
		}

	} // process
	//---------------------------------------------------------

	void CCamera::setTargetEnemy(CEntity* enemy){
		
		this->_enemy=enemy;
		//std::cout << "el enemigo a apuntar se llama " << _enemy->getName() << std::endl;
		//td::cout << "y su tipo es " << _enemy->getType() << std::endl;
		_dead=true;
	}

	//---------------------------------------------------------

	void CCamera::tick(unsigned int msecs)
	{
		if(_target)
		{
			// Actualizamos la posición de la cámara.
			Vector3 position = _target->getPosition();
			position.y+=_height;

			//Si hay offset (vibración) de cámara
			if (_fOffsetTimer > 0)
			{
				//Actualizo el timer
				_fOffsetTimer -= msecs;

				//"Vibración" de la cámara
				Matrix4 transf = _entity->getTransform();
 				Math::yaw(Math::HALF_PI, transf);
				_entity->setTransform(transf);
				
				//En el eje de movimiento horizontal
				Vector3 directionStrafe = Math::getDirection(_entity->getYaw() + Math::PI/2);
				position += directionStrafe;
				std::cout << "CamaraOffset!!!" << std::endl;
			}
			_graphicsCamera->setCameraPosition(position);
			
			if(!_dead){
				// Y la posición hacia donde mira la cámara.
				Vector3 direction = Math::getDirection(_target->getOrientation());
				_graphicsCamera->setTargetCameraPosition(position + direction);
			}
			//Si estamos muertos miramos al enemigo, diferenciamos entre nosotros mismos o el rival
			else if(_enemy){
				 if(_enemy->getType().compare("LocalPlayer")!=0)
				    _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 else{	
					 _graphicsCamera->setCameraPosition(_enemy->getPosition()+Vector3(0,50,0));
					 _graphicsCamera->setTargetCameraPosition(_enemy->getPosition());
				 }
			}

		}else{
			_graphicsCamera->setTargetCameraPosition(_targetV);
		}
	} // tick


} // namespace Logic

