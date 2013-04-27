/**
@file Jumper.cpp

Contiene la implementación del componente que gestiona la aplicacion de una fuerza de salto al jugador.

@see Logic::CJumper
@see Logic::IComponent

*/

#include "Lava.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "PhysicController.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageDamaged.h"



namespace Logic 
{
	IMP_FACTORY(CLava);
	
	//---------------------------------------------------------

	bool CLava::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		if(entityInfo->hasAttribute("damage"))
			_damage = entityInfo->getFloatAttribute("damage");
		if(entityInfo->hasAttribute("timeDamage"))
			_timeDamage = entityInfo->getIntAttribute("timeDamage");
		return true;

	} // spawn
	//---------------------------------------------------------

	void CLava::activate()
	{
		IComponent::activate();
		_timestamp = 0;
	} // activate
	//--------------------------------------------------------


	bool CLava::accept(const std::shared_ptr<CMessage>& message)
	{
		Logic::TMessageType msgType = message->getMessageType();

		return	msgType == Message::TOUCHED || 
				msgType == Message::UNTOUCHED;
	} // accept
	//---------------------------------------------------------


	void CLava::process(const std::shared_ptr<CMessage>& message)
	{
		switch(message->getMessageType()) {
			case Message::TOUCHED: {
				std::shared_ptr<CMessageTouched> touchedMsg = std::static_pointer_cast<CMessageTouched>(message);

				_intrigger.push_back(touchedMsg->getEntity());
				break;
			}
			case Message::UNTOUCHED: {
				std::shared_ptr<CMessageUntouched> untouchedMsg = std::static_pointer_cast<CMessageUntouched>(message);

				_intrigger.remove(untouchedMsg->getEntity());
				break;
			}
		}

	} // process
	//---------------------------------------------------------


	//---------------------------------------------------------
	void CLava::onTick(unsigned int msecs) {
		if(_intrigger.size()>0 && _timestamp > _timeDamage){
			std::list<CEntity*>::const_iterator it = _intrigger.begin();
			for(; it != _intrigger.end(); ++it) {
				std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
				damage->setDamage(_damage);
				damage->setEnemy(_entity);
				(*it)->emitMessage(damage);
			}
			_timestamp= 0;
			return;
		}
		_timestamp+=msecs;
	} // tick

} // namespace Logic

