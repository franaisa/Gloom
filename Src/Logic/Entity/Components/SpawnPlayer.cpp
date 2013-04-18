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


	bool CSpawnPlayer::accept(CMessage *message)
	{
		return message->getMessageType() == Message::PLAYER_DEAD;
	} // accept
	//---------------------------------------------------------


	void CSpawnPlayer::process(CMessage *message)
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
				Vector3 spawn = CServer::getSingletonPtr()->getSpawnManager()->getSpawnPosition();

				//Activamos la simulación física (fue desactivada al morir)
				_entity->getComponent<CPhysicController>("CPhysicController")->activateSimulation();

				//Ponemos la entidad física en la posición instantaneamente ( no se puede permitir el envio de mensajes )
				_entity->getComponent<CPhysicController>("CPhysicController")->setPhysicPosition(spawn);

				//Volvemos a activar todos los componentes
				_entity->activate();

				//Establecemos la orientación adecuada segun la devolución del manager de spawn
				_entity->setYaw(180);

				// Si eres el server mandar un mensaje de spawn
				Logic::CMessagePlayerSpawn* spawnMsg = new Logic::CMessagePlayerSpawn();
				spawnMsg->setSpawnTransform( _entity->getTransform() );
				_entity->emitMessage(new CMessagePlayerSpawn());
				CEntity * camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
					
				camera->emitMessage(spawnMsg);
				//if(Net::CManager::getSingletonPtr()->imServer())
				//	Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(new CMessagePlayerSpawn(), camera->getEntityID(), _entity->getEntityID());

				

				
				CMessageCreateParticle *particle = new CMessageCreateParticle();
				particle->setParticle("SpawnParticle");
				particle->setPosition(_entity->getPosition());

				//Sonido Spawn
				Logic::CMessageAudio *maudio=new Logic::CMessageAudio();
				maudio->setRuta(_audioSpawn);
				maudio->setId("spawn");
				maudio->setPosition(_entity->getPosition());
				maudio->setNotIfPlay(false);
				maudio->setIsPlayer(_entity->isPlayer());
				_entity->emitMessage(maudio);
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
			Logic::CMessageHudSpawn *m=new Logic::CMessageHudSpawn();
			m->setTime(_timeSpawn/1000);
			_entity->emitMessage(m);

		}

	} // process
	//---------------------------------------------------------

} // namespace Logic

