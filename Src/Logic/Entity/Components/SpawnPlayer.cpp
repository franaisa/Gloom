/**
@file SpawnPlayer.cpp

Contiene la implementaci�n del componente que gestiona el spawn del jugador.
 
@see Logic::CSpawnPlayer
@see Logic::IComponent

*/

#include "SpawnPlayer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/GameSpawnManager.h"
#include "PhysicController.h"
#include "Logic/GameNetMsgManager.h"

#include "Logic/Messages/MessagePlayerDead.h"
#include "Logic/Messages/MessagePlayerSpawn.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageHudSpawn.h"
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessageCreateParticle.h"

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
		IComponent::onActivate();
		
		_isDead=false;
		_actualTimeSpawn=0;

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
		//Solamente si estamos muertos (se recibi� el mensaje)
		if(_isDead){
			_actualTimeSpawn+=msecs;
			//Si superamos el tiempo de spawn tenemos que revivir
			if(_actualTimeSpawn>_timeSpawn){
				_isDead = false;
				_actualTimeSpawn = 0;
				//LLamamos al manager de spawn que nos devolver� una posici�n ( ahora hecho a lo cutre)
				CEntity *spawn = CServer::getSingletonPtr()->getSpawnManager()->getSpawnPosition();

				//Ponemos la entidad f�sica en la posici�n instantaneamente ( no se puede permitir el envio de mensajes )
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(spawn->getPosition());

				//Una vez posicionado, activamos la simulaci�n f�sica (fue desactivada al morir)
				_entity->getComponent<CPhysicController>("CPhysicController")->activateSimulation();

				//Volvemos a activar todos los componentes(lo que hace resetea _isDead y _actualTimeSpawn)
				_entity->activate();

				//Establecemos la orientaci�n adecuada segun la devoluci�n del manager de spawn
				_entity->setOrientation(spawn->getOrientation());

				// Si eres el server mandar un mensaje de spawn
				std::shared_ptr<CMessagePlayerSpawn> spawnMsg = std::make_shared<CMessagePlayerSpawn>();
				spawnMsg->setSpawnTransform( _entity->getTransform() );
				_entity->emitMessage(spawnMsg);

				CEntity * camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(spawnMsg);
				//if(Net::CManager::getSingletonPtr()->imServer())
				//	Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(new CMessagePlayerSpawn(), camera->getEntityID(), _entity->getEntityID());

				//Mirar porque se cre� esto, lo mismo antonio sabe
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

			//Desactivamos la simulaci�n f�sica (no puede estar activo en la escena f�sica al morir)
			_entity->getComponent<CPhysicController>("CPhysicController")->deactivateSimulation();

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

