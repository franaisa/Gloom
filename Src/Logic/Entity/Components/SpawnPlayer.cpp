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


#include "Logic/Messages/MessagePlayerDead.h"
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

	void CSpawnPlayer::tick(unsigned int msecs)
	{
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
				//Volvemos a activar todos los componentes para que la fisica pueda recibir el mensaje de spawn
				_entity->activate();
				//Ponemos la entidad física en la posición instantaneamente ( no se puede permitir el envio de mensajes )
				_entity->getComponent<CPhysicController>("CPhysicEntityController")->setPhysicPosition(spawn);
				//Establecemos la orientación adecuada segun la devolución del manager de spawn
				_entity->setYaw(180);

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
			std::list<std::string*> *except=new std::list<std::string*>();
			except->push_back(new std::string("CSpawnPlayer"));
			except->push_back(new std::string("CHudOverlay"));
			except->push_back(new std::string("CNetConnector"));
			_entity->deactivateAllComponentsExcept(except);
			_isDead=true;
			//Liberando memoria
			for(std::list<std::string*>::iterator it = except->begin(); it != except->end(); ++it)
				delete *it;
			delete except;
			//Mensaje para el Hud (tiempo de spawn)
			Logic::CMessageHudSpawn *m=new Logic::CMessageHudSpawn();
			m->setTime(_timeSpawn/1000);
			_entity->emitMessage(m);

			_timeToSendHud=_timeSpawn/1000;
		}

	} // process
	//---------------------------------------------------------

} // namespace Logic

