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
			//Actualizamos el HUD para el respawn
			if(_timeSpawn/1000-_actualTimeSpawn/1000!=_timeToSendHud && _timeToSendHud>1){
				_timeToSendHud=_timeSpawn/1000-_actualTimeSpawn/1000;
				//Mensaje para el Hud (tiempo de spawn)
				Logic::CMessageHudSpawn *m=new Logic::CMessageHudSpawn();
				m->setTime(_timeToSendHud);
				_entity->emitMessage(m);
			}
		
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
				_entity->emitMessage(spawnMsg);
				CEntity * camera = CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
					
				camera->emitMessage(spawnMsg);
				Logic::CGameNetMsgManager::getSingletonPtr()->sendMessageToOne(new CMessagePlayerSpawn(), camera->getEntityID(), _entity->getEntityID());

				Logic::CMessageHudSpawn *mS=new Logic::CMessageHudSpawn();
				mS->setTime(0);
				_entity->emitMessage(mS);
			}
		}


	} // tick
	//---------------------------------------------------------

	void CSpawnPlayer::dead()
	{
		//Si no esto muerto ya hago las acciones
		if(!_isDead){
			//Desactivamos todos menos el cspawnplayer
			std::vector<std::string> except(4);
			except.push_back( std::string("CAnimatedGraphics") );
			except.push_back( std::string("CSpawnPlayer") );
			except.push_back( std::string("CHudOverlay") );
			except.push_back( std::string("CNetConnector") );

			//Desactivamos la simulación física (no puede estar activo en la escena física al morir)
			_entity->getComponent<CPhysicController>("CPhysicController")->deactivateSimulation();

			_entity->deactivateAllComponentsExcept(except);
			_isDead=true;
			//Mensaje para el Hud (tiempo de spawn)
			Logic::CMessageHudSpawn *m=new Logic::CMessageHudSpawn();
			m->setTime(_timeSpawn/1000);
			_entity->emitMessage(m);

			_timeToSendHud=_timeSpawn/1000;
		}

	} // process
	//---------------------------------------------------------

} // namespace Logic

