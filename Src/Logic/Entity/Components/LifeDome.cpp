/**
@file LifeDome.cpp

Contiene la implementación del componente que controla la vida cúpula del arcángel que da vida a una entidad.
 
@see Logic::CLifeDome
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Mayo, 2013
*/

#include "LifeDome.h"

#include "PhysicDynamicEntity.h"

#include "Graphics/Entity.h"
#include "Graphics.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Archangel.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"

namespace Logic 
{
	IMP_FACTORY(CLifeDome);
		
	//________________________________________________________________________

	/*CLifeDome::CLifeDome() 
	{
					 //_owner(NULL) {

		// Nada que hacer
	}*/

	//________________________________________________________________________

	CLifeDome::~CLifeDome() {
		// Nada que borrar
	}

	//________________________________________________________________________

	bool CLifeDome::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_physicComponent = _entity->getComponent<CPhysicDynamicEntity>("CPhysicDynamicEntity"); 	

		_cGraph = _entity->getComponent<CGraphics>("CGraphics");

		_life  = entityInfo->getIntAttribute("lifeDomeLife");
		_lifePerFriend = entityInfo->getIntAttribute("lifeDomePerFriend");
		return true;

	} // spawn

	//________________________________________________________________________

	bool CLifeDome::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED || msgType == Message::SET_REDUCED_DAMAGE || msgType == Message::TOUCHED;
	} // accept
	
	//________________________________________________________________________

	void CLifeDome::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::DAMAGED: {
				//std::shared_ptr<CMessageDamaged> dmgMsg = std::static_pointer_cast<CMessageDamaged>(message);
				//damaged( dmgMsg->getDamage(), dmgMsg->getEnemy() );
				break;
			}
			case Message::SET_REDUCED_DAMAGE: {
				//std::shared_ptr<CMessageSetReducedDamage> reducedDmgMsg = std::static_pointer_cast<CMessageSetReducedDamage>(message);
				//reducedDamageAbsorption( reducedDmgMsg->getReducedDamage() );
				break;
			}
			case Message::TOUCHED:
			{				
				auto msg = std::static_pointer_cast<CMessageTouched>(message);
				lifeDomeTouched(msg->getEntity());
				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CLifeDome::onStart() {
		
	}
	//________________________________________________________________________
	
	void CLifeDome::setOwner(Logic::CEntity *owner)
	{
		printf("\Owner asignao a %s",owner->getName().c_str());
		_owner = owner;
	} // setOwner
	//________________________________________________________________________

	void CLifeDome::onFixedTick(unsigned int msecs) {
		
		if (_owner)
		{
			_physicComponent->setPosition(_owner->getPosition(), true);
			
			/*
			printf("\n\nYo soy %s y mi owner es %s. \nPosicon1: %f %f %f . \nPosicion2: %f %f %f ", _entity->getName().c_str(), _owner->getName().c_str(), 
				_owner->getPosition().x,_owner->getPosition().y,_owner->getPosition().z,_entity->getPosition().x,_entity->getPosition().y,_entity->getPosition().z);
			*/
			/*
			//Desesperacion de poner el cgraph aqui a ver si me mueve la posicion de la cupula pero nada
			if (cGraph)
				cGraph->setPosition(_owner->getPosition() + Vector3(0,1,0));
			*/
		}
		
		if (_cGraph)
		{
			_entity->setPosition(_owner->getPosition());
			if (_scale < 10.0f)
			{
				_scale += 0.005f;
			}
			_cGraph->changeScale(_scale);
			//Ponemos la posición del jugador subiéndolo un poco en el ejeY
			//cGraph->setPosition(_owner->getEntity()->getPosition() + Vector3(0,1,0));
		}	
	}
	

	//________________________________________________________________________

	void CLifeDome::onActivate() {
		//_reducedDamageAbsorption = 0;
	}
	//________________________________________________________________________

	void CLifeDome::lifeDomeTouched(CEntity *entityTouched)
	{
		std::cout << "Tocado! al principio me toca a mi" << std::endl;
		if(entityTouched->getEntityID() != _entity->getEntityID()){
			// He de comprobar que es amigo, o eso, o en el filtro que solo le de a los amigos
			std::shared_ptr<CMessageAddLife> addLifeMsg = std::make_shared<CMessageAddLife>();
			addLifeMsg->setAddLife(_life);
			entityTouched->emitMessage(addLifeMsg);

			std::shared_ptr<CMessageAddLife> addLifeMsg2 = std::make_shared<CMessageAddLife>();
			addLifeMsg2->setAddLife(_lifePerFriend);
			_entity->emitMessage(addLifeMsg2);
		}

	}
	//________________________________________________________________________


} // namespace Logic

