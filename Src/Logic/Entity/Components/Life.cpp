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

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"


#include "Logic/Messages/MessagePlayerDead.h"

namespace Logic 
{
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------
	
	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
			_life = entityInfo->getIntAttribute("life");
		if(entityInfo->hasAttribute("shield"))
			_shield = entityInfo->getIntAttribute("shield");
		if(entityInfo->hasAttribute("lifeDamage"))
			_lifeDamage = entityInfo->getIntAttribute("lifeDamage");
		if(entityInfo->hasAttribute("lifeTimeDamage"))
			_lifeTimeDamage = entityInfo->getIntAttribute("lifeTimeDamage");
		if(entityInfo->hasAttribute("PorcentShield"))
			_porcentShield = entityInfo->getIntAttribute("PorcentShield");
		if(entityInfo->hasAttribute("maxLife"))
			_maxLife = entityInfo->getIntAttribute("maxLife");
		if(entityInfo->hasAttribute("maxShield"))
			_maxShield = entityInfo->getIntAttribute("maxShield");
		if(entityInfo->hasAttribute("playerDead"))
			_playerDead =  entityInfo->getBoolAttribute("playerDead");
		_varLifeCumulative=0;

		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(CMessage *message)
	{
		return message->getMessageType() == Message::DAMAGED || 
		message->getMessageType() == Message::ADD_LIFE ||
		message->getMessageType() == Message::ADD_SHIELD ;
	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::DAMAGED:
			{
				damaged( ((CMessageDamaged*)message)->getDamage());	
			}
			break;
			case Message::ADD_LIFE:
			{
				addLife( ((CMessageAddLife*)message)->getAddLife());
			}
			break;
			case Message::ADD_SHIELD:
			{
				addShield( ((CMessageAddShield*)message)->getAddShield());
			}
			break;
		}

	} // process
	//----------------------------------------------------------

	void CLife::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		_varLifeCumulative+=msecs;
		//Multiplicamos por mil ya que _varLifeCumulative es en milisegundos
		if(_varLifeCumulative >=_lifeTimeDamage*1000){
			if(_life > _lifeDamage){
				_life-=_lifeDamage;	
				//Actualización vida
				Logic::CMessageHudLife *message1 = new Logic::CMessageHudLife();
				message1->setHudLife(_life);
				_entity->emitMessage(message1);	
			}
			_varLifeCumulative=0;
		}
	} // tick
	//----------------------------------------------------------

	void CLife::damaged(int damage){
		if(_shield>0){
			int porcentajeEscudo = _porcentShield * damage * 0.01;
			int porcentajeVida = damage - porcentajeEscudo;
			if((_shield>=porcentajeEscudo)){
				_shield-=porcentajeEscudo;
				_life-=porcentajeVida;
				}
				else{
					porcentajeEscudo-=_shield;
					_shield=0;
					porcentajeVida= porcentajeVida + porcentajeEscudo;
					_life=_life-porcentajeVida;
				}
					Logic::CMessageHudShield *message2 = new Logic::CMessageHudShield();
					message2->setHudShield(_shield);
					_entity->emitMessage(message2);
		}
		else
			_life -= damage;

		//Si muero por el daño, envio un mensaje de playerDead
		if(_life<=0){
			_life=0;
			std::cout << "MUEROOOWNZ" << std::endl;
			Logic::CMessagePlayerDead *m=new Logic::CMessagePlayerDead();
			_entity->emitMessage(m);
		}
		//Actualizo la vida
		Logic::CMessageHudLife *message1 = new Logic::CMessageHudLife();
		message1->setHudLife(_life);
		_entity->emitMessage(message1);
	}// damaged
	//----------------------------------------------------------
	
	void CLife::addLife(int life){
		if(_life<_maxLife){
			if(_life+life<=_maxLife)
				_life+=life;
			else
				_life=_maxLife;

			Logic::CMessageHudLife *message1 = new Logic::CMessageHudLife();
			message1->setHudLife(_life);
			_entity->emitMessage(message1);
		}
	}// addLife
	//----------------------------------------------------------


	void CLife::addShield(int shield){
		if(_shield<_maxShield){
			if(_shield+shield<=_maxShield)
				_shield+=shield;
			else
				_shield=_maxShield;

			Logic::CMessageHudShield *message2 = new Logic::CMessageHudShield();
			message2->setHudShield(_shield);
			_entity->emitMessage(message2);
		}
	}// addShield
	//----------------------------------------------------------

} // namespace Logic


