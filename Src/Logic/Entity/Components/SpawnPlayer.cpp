/**
@file SpawnPlayer.cpp

Contiene la implementación del componente que gestiona el spawn del jugador.
 
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

	void CSpawnPlayer::activate()
	{
		IComponent::activate();
		
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

	void CSpawnPlayer::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		//Solamente si estamos muertos (se recibió el mensaje)
		if(_isDead){
			_actualTimeSpawn+=msecs;
		
			//Si superamos el tiempo de spawn tenemos que revivir
			if(_actualTimeSpawn>_timeSpawn){
				//LLamamos al manager de spawn que nos devolverá una posición ( ahora hecho a lo cutre)
				CEntity *spawn = CServer::getSingletonPtr()->getSpawnManager()->getSpawnPosition();

				//Ponemos la entidad física en la posición instantaneamente ( no se puede permitir el envio de mensajes )
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(spawn->getPosition());

				//Una vez posicionado, activamos la simulación física (fue desactivada al morir)
				_entity->getComponent<CPhysicController>("CPhysicController")->activateSimulation();

				//Volvemos a activar todos los componentes
				_entity->activate();

				//Establecemos la orientación adecuada segun la devolución del manager de spawn
				_entity->setOrientation(spawn->getOrientation());

				// Si eres el server mandar un mensaje de spawn
				std::shared_ptr<CMessagePlayerSpawn> spawnMsg = std::make_shared<CMessagePlayerSpawn>();
				spawnMsg->setSpawnTransform( _entity->getTransform() );

				std::shared_ptr<CMessagePlayerSpawn> entitySpawnMsg = std::make_shared<CMessagePlayerSpawn>();
				_entity->emitMessage(entitySpawnMsg);

				CEntity * camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(spawnMsg);

				if(Net::CManager::getSingletonPtr()->imServer()) {
					std::shared_ptr<CMessagePlayerSpawn> serverSpawnMsg = std::make_shared<CMessagePlayerSpawn>();
					Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(serverSpawnMsg, camera->getEntityID(), _entity->getEntityID());
				}

				std::shared_ptr<CMessageHudSpawn> mS = std::make_shared<CMessageHudSpawn>();
				mS->setTime(0);
				_entity->emitMessage(mS);

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
			}
		}
	} // tick
	//---------------------------------------------------------

	void CSpawnPlayer::dead()
	{
		//Si no esto muerto ya hago las acciones
		if(!_isDead){
			//Desactivamos todos menos el cspawnplayer
			std::vector<std::string> except;
			except.reserve(5); // Solo necesitamos 5 slots
			except.push_back( std::string("CAnimatedGraphics") );
			except.push_back( std::string("CSpawnPlayer") );
			except.push_back( std::string("CHudOverlay") );
			except.push_back( std::string("CNetConnector") );
			except.push_back( std::string("CAudio") );

			//Desactivamos la simulación física (no puede estar activo en la escena física al morir)
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

