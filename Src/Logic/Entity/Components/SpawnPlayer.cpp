/**
@file SpawnPlayer.cpp

Contiene la implementación del componente que gestiona el spawn del jugador.
 
@see Logic::CSpawnPlayer
@see Logic::IComponent

*/

#include "SpawnPlayer.h"
#include "AvatarController.h"
#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/GameSpawnManager.h"
#include "PhysicController.h"
#include "Logic/GameNetMsgManager.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Maps/WorldState.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessagePlayerSpawn.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageHudSpawn.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageCreateParticle.h"
#include "Logic/Messages/MessageSpawnIsLive.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace Logic 
{
	IMP_FACTORY(CSpawnPlayer);
	
	//---------------------------------------------------------

	bool CSpawnPlayer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("timeSpawn")){
			_timeSpawn= entityInfo->getIntAttribute("timeSpawn");
			_timeSpawn*=1000;
		}

		if(entityInfo->hasAttribute("audioSpawn"))
			_audioSpawn =  entityInfo->getStringAttribute("audioSpawn");

		return true;

	} // spawn
	//---------------------------------------------------------

	void CSpawnPlayer::onActivate()
	{
		_isDead=false;
		_actualTimeSpawn=_inmunityTimer=0;
		_reactivePhysicSimulation=false;

	} // activate
	//--------------------------------------------------------


	bool CSpawnPlayer::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::PLAYER_DEAD;
	} // accept
	//---------------------------------------------------------


	void CSpawnPlayer::process(const std::shared_ptr<CMessage>& message)
	{
		switch(message->getMessageType())
		{
		case Message::PLAYER_DEAD:
			dead();
			break;
		}

	} // process
	//---------------------------------------------------------

	void CSpawnPlayer::onTick(unsigned int msecs) {
		//Solamente si estamos muertos (se recibió el mensaje)
		if(_isDead){
			_actualTimeSpawn+=msecs;
			//Si superamos el tiempo de spawn tenemos que revivir
			if(_actualTimeSpawn>_timeSpawn){
				_isDead = false;
				_actualTimeSpawn = 0;
				//LLamamos al manager de spawn que nos devolverá una posición
				CEntity *spawn = CServer::getSingletonPtr()->getSpawnManager()->getSpawnPosition();

				//Ponemos la entidad física en la posición instantaneamente ( no se puede permitir el envio de mensajes )
				//La simulacion fisica tiene que ser activada en el siguiente tick, para que el player se resitue bien
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(spawn->getPosition());
				_reactivePhysicSimulation=true;

				//Volvemos a activar todos los componentes(lo que hace resetea _isDead y _actualTimeSpawn)
				_entity->activate();

				// Informamos al estado del mundo de que ha cambiado el estado de muerte de la entidad
				Logic::CWorldState::getSingletonPtr()->deleteChange(_entity, Message::PLAYER_DEAD);

				//Establecemos la orientación adecuada segun la devolución del manager de spawn
				Graphics::CCamera *cam=_entity->getMap()->getScene()->getCamera();
				cam->setYaw(Ogre::Quaternion(0,0,1,0));//180 grados
				_entity->setOrientation(Ogre::Quaternion(0,0,1,0));
				
				// Si eres el server mandar un mensaje de spawn
				std::shared_ptr<CMessagePlayerSpawn> spawnMsg = std::make_shared<CMessagePlayerSpawn>();
				spawnMsg->setSpawnTransform( _entity->getTransform() );
				_entity->emitMessage(spawnMsg);

				CEntity* camera = CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
				assert(camera != NULL && "Error: Esto no se puede hacer asi que sois unos lamers, ahora el servidor que hace?");
				camera->emitMessage(spawnMsg);
				
				//if(Net::CManager::getSingletonPtr()->imServer())
				//	Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(new CMessagePlayerSpawn(), camera->getEntityID(), _entity->getEntityID());
				//Mirar porque se creó esto, lo mismo antonio sabe
				/*
				std::shared_ptr<CMessageCreateParticle> particle =std::make_shared<CMessageCreateParticle>();
				particle->setParticle("SpawnParticle");
				particle->setPosition(_entity->getPosition());
				*/

				//Sonido Spawn
				std::shared_ptr<CMessageAudio> audioMsg = std::make_shared<CMessageAudio>();
				audioMsg->setRuta(_audioSpawn);
				audioMsg->setId("spawn");
				audioMsg->setPosition(_entity->getPosition());
				audioMsg->setNotIfPlay(false);
				audioMsg->setIsPlayer(_entity->isPlayer());
				_entity->emitMessage(audioMsg);

				_inmunityTimer = 0;
			}
		}
		else if(_reactivePhysicSimulation){
			_entity->getComponent<CPhysicController>("CPhysicController")->activateSimulation();
			_entity->getComponent<CAvatarController>("CAvatarController")->wakeUp();

			// Comprobamos el timer de inmunidad al respawnear
			_inmunityTimer += msecs;
			if(_inmunityTimer > _inmunityTime) {
				_inmunityTimer = 0;
				_entity->emitMessage( std::make_shared<CMessageSpawnIsLive>() );

				_reactivePhysicSimulation=false;
				putToSleep();
			}
		}
	} // tick
	//---------------------------------------------------------

	void CSpawnPlayer::dead()
	{
		//Si no esto muerto ya hago las acciones
		if(!_isDead){
			//Desactivamos todos menos el cspawnplayer
			std::set<std::string> except;
			except.insert( std::string("CAnimatedGraphics") );
			except.insert( std::string("CSpawnPlayer") );
			except.insert( std::string("CHudOverlay") );
			except.insert( std::string("CNetConnector") );
			except.insert( std::string("CAudio") );
			except.insert( std::string("CAvatarController") );

			//Desactivamos la simulación física (no puede estar activo en la escena física al morir)
			_entity->getComponent<CPhysicController>("CPhysicController")->deactivateSimulation();
			_entity->getComponent<CAvatarController>("CAvatarController")->putToSleep(true);

			_entity->deactivateAllComponentsExcept(except);
			_isDead=true;
			//Mensaje para el Hud (tiempo de spawn)
			std::shared_ptr<CMessageHudSpawn> m = std::make_shared<CMessageHudSpawn>();
			m->setTime(_timeSpawn/1000);
			_entity->emitMessage(m);

		}

	} // process
	//---------------------------------------------------------

} // namespace Logic

