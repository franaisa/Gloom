/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Para informar por red que se ha acabado el juego
#include "Net/Manager.h"
#include "Net/buffer.h"

namespace Logic 
{
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
			_life = entityInfo->getFloatAttribute("life");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(CMessage *message)
	{
		return message->getMessageType() == Message::DAMAGED;

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::DAMAGED:
			{
				damaged( ((CMessageDamaged*)message)->getDamage());
				/*
				// PARTE ANTONIO - HAY QUE CAMBIARLO, YA QUE NO SE ENVIAN MENSAJES
				// ADEMÁS HAY QUE ADAPTARLO A LA RED.

				// Disminuir la vida de la entidad
				/*_life -= ((CMessageDamaged*)message)->getDamage();
				printf("\nAh!, ya solo me queda %i de vida", _life);
				
				// Si han matado al jugador salir de la partida
				
				//if ((_life <= 0) && (_entity->isPlayer())) {
				//	Application::CBaseApplication::getSingletonPtr()->setState("menu");
				//}
				
				// @todo Poner la animación de herido.
				// @todo Si la vida es menor que 0 poner animación de morir.

				// Si han matado al jugador sacarlo de la partida
				if (_life <= 0 && _entity->getType() == "Player") 
				{
					// TODO Si matan a un jugador habrá que avisarle que, para él, el 
					// juego ha terminado. Si hubiese más jugadores también deberían
					// enterarse de que ese jugador ha muerto para que eliminen su entidad...
					// TODO Pero queremos eliminar la entidad o dejarla deactive?
					// TODO Quizá aquí merezca la pena tener un CDeath que se encargue de estas cosas con un Msg DIE entidad tal
					
					// MENSAJE "END_GAME | TEntityID"
					Net::NetMessageType txMsg = Net::NetMessageType::END_GAME;
					TEntityID entityID = _entity->getEntityID();
					Net::CBuffer serialMsg;						
						serialMsg.write( &txMsg, sizeof(txMsg));
						serialMsg.write( &entityID, sizeof(entityID) );	
					Net::CManager::getSingletonPtr()->send( serialMsg.getbuffer(),  serialMsg.getSize() );

					// TRATAMIENTO DEL PLAYER
					if(_entity->isPlayer() ) // MONOJUGADOR -> GameOver
						Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
					else					// MULTIJUGADOR
						_entity->deactivate();
				}
				
				CMessageSetAnimation * msg = new CMessageSetAnimation(TMessageType::SET_ANIMATION);
											
				if(_life > 0)  // TODO Poner la animación de herido.
					msg->setString("Damage");
				else  // TODO Si la vida es menor que 0 poner animación de morir.
					msg->setString("Death");
				_entity->emitMessage(msg, this);
				*/
			}
			break;
		}

	} // process

	void CLife::damaged(float damage){
		_life -= damage;

		printf("\nAh!, ya solo me queda %i de vida", _life);

		//if ((_life <= 0) && (_entity->isPlayer())) {
		//	Application::CBaseApplication::getSingletonPtr()->setState("menu");
	}


} // namespace Logic

