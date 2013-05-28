/**
@file NetCommands.cpp

@author Rubén Mulero Guerrero
@date May, 2013
*/

#include "NetCommands.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessageSyncPosition.h"
#include "Logic/Messages/MessageControl.h"

#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/Interpolation.h"
#include "Logic/Entity/Components/AvatarController.h"

#include "Logic/GameNetMsgManager.h"

namespace Logic 
{
	IMP_FACTORY(CNetCommands);

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CNetCommands::onFixedTick(unsigned int msecs) {

		/*if(_timer > _syncPosTimeStamp) {
			std::shared_ptr<CMessageSyncPosition> msg = std::make_shared<CMessageSyncPosition>();
			Vector3 position = _entity->getPosition();
			msg->setPosition( position );
			msg->setSeq(++_seqNumber);
			//CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(msg, _entity->getEntityID());
			//insertamos el mensaje en la lista de mensajes no aceptados todavía
			TSyncMessage elem(_seqNumber,position);
			_msgBuffer.push_back(elem);

			//si sobro tiempo lo guardamos pa el siguiente
			_timer = _timer-_syncPosTimeStamp;
		}

		_timer += msecs;*/
	} // tick

////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CNetCommands::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if( entityInfo->hasAttribute("syncPosTimeStamp") ) {
			// Convertimos a msecs
			_syncPosTimeStamp = entityInfo->getFloatAttribute("syncPosTimeStamp") * 1000;
		}

		_physicController = _entity->getComponent<CPhysicController>("CPhysicController");

		_interpolater = _entity->getComponent<CInterpolation>("CInterpolation");

		if(!_physicController || !_interpolater)
			return false;

		_minDist = 0.05f;
		_maxDist = 7.0f;
		return true;
	} // spawn

////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CNetCommands::accept(const std::shared_ptr<CMessage>& message) {
		return message->getMessageType() == Message::CONTROL ||
			message->getMessageType() == Message::SYNC_POSITION;
	} // accept

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CNetCommands::process(const std::shared_ptr<CMessage>& message) {
		switch(message->getMessageType())
		{
		case Message::SYNC_POSITION:
			{
			acceptAndInterpolate(std::static_pointer_cast<CMessageSyncPosition>(message));
			break;
			}
		case Message::CONTROL:
			{
			sendControlMessage(std::static_pointer_cast<CMessageControl>(message));
			break;
			}
		}

	} // proces

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CNetCommands::sendControlMessage(const std::shared_ptr<CMessageControl>& message){
		//completamos el mensaje de control, nos lo guardamos en la lista de mensajes no
		//aceptados y lo enviamos por la red

		Vector3 position = _entity->getPosition();
		TSyncMessage elem(++_seqNumber,position);
		_msgBuffer.push_back(elem);

		//std::cout << "mensaje " << _seqNumber << " resultado " << _entity->getComponent<CAvatarController>("CAvatarController")->getVelocity() << std::endl;

		//añadimos el numero de secuencia al mensaje
		message->setSeqNumber(_seqNumber);

		//enviamos el mensaje por red
		CGameNetMsgManager::getSingletonPtr()->sendEntityMessage(message, _entity->getEntityID());

		//reset the sync timer
		_timer = 0;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CNetCommands::acceptAndInterpolate(const std::shared_ptr<CMessageSyncPosition>& message){
		//ensure the list is not empty
		if(_msgBuffer.empty())
			return;
		
/*		unsigned int seq = message->getSeq();

		std::deque<TSyncMessage>::iterator it = _msgBuffer.begin();
		std::deque<TSyncMessage>::iterator end = _msgBuffer.end();

		while(it!=end){
			if((*it).first<seq){
				it = _msgBuffer.erase(it);
			}else if((*it).first==seq){
				//calculate interpolation and return
				calculateInterpolation((*it).second, message->getPosition());
				it = _msgBuffer.erase(it);
				return;
			}else{
				++it;
			}
		}

		//calculateInterpolation(lastSeq.second, message->getPosition());
*/
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CNetCommands::calculateInterpolation(const Vector3 &myPosition, const Vector3 &srvPosition){
		Vector3 distance = srvPosition - myPosition;
		 
		//we don't need to interpolate if we are accurate enough
		if(distance.length()<_minDist){
			_interpolater->setLerpLevel(Vector3::ZERO);
			return;
		}

		

		//update non acknowledged points
		for(int i =0; i< _msgBuffer.size();++i){
			_msgBuffer[i].second +=distance;
		}

		//if distance is far enough, teleport the player
		if(distance.length()>_maxDist){
			_physicController->setPhysicPosition( _entity->getPosition()+distance );
			_interpolater->setLerpLevel(Vector3::ZERO);
			return;
		}
		distance*=Vector3(1,0,1);
		std::cout << distance.length() << std::endl;
		//interpolate
		_interpolater->setLerpLevel(distance);

	}

////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*void CNetCommands::lerp(){

		//get the tick lerp
		Vector3 distance = _lerp*0.3;
		_lerp-=distance;

		//refresh all the positions saved for acknowledge
		for(int i =0; i< _msgBuffer.size();++i){
			_msgBuffer[i].second +=distance;
		}

		//update entity position
		_physicController->setPhysicPosition( _entity->getPosition()+distance );

		//if lerp distance is close enough, we can ignore it
		if(_lerp.length()<_minDist)
			_lerp=Vector3::ZERO;
	}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace Logic

