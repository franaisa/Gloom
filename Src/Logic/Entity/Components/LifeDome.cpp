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
#include "ParticleSystem.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Archangel.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageSetOwner.h"



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

		_cGraph = _entity->getComponent<CParticleSystem>("CParticleSystem");

		_life  = entityInfo->getIntAttribute("lifeDomeLife");
		_lifePerFriend = entityInfo->getIntAttribute("lifeDomePerFriend");
		return true;

	} // spawn

	//________________________________________________________________________

	bool CLifeDome::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED				|| 
			   msgType == Message::SET_REDUCED_DAMAGE	|| 
			   msgType == Message::TOUCHED				||
			   msgType == Message::SET_OWNER;
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
				std::shared_ptr<CMessageTouched> msg = std::static_pointer_cast<CMessageTouched>(message);
				lifeDomeTouched(msg->getEntity());
				break;
			}
			case Message::SET_OWNER: {
				setOwner( std::static_pointer_cast<CMessageSetOwner>(message)->getOwner() );
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
		_owner = owner;
	} // setOwner
	//________________________________________________________________________

	void CLifeDome::onFixedTick(unsigned int msecs) {
		
		if (_owner) {
			if (_cGraph ) {
				Vector3 pos = _owner->getPosition();
				_entity->setPosition(pos);
				_cGraph->setPosition(pos);

				//Ponemos la posición del jugador subiéndolo un poco en el ejeY
				//cGraph->setPosition(_owner->getEntity()->getPosition() + Vector3(0,1,0));
			}else{
				if(_physicComponent){
					_physicComponent->setPosition(_owner->getPosition(), true);
				}
			}
		}
	}
	

	//________________________________________________________________________

	void CLifeDome::onActivate() {
		//_reducedDamageAbsorption = 0;
	}
	//________________________________________________________________________

	void CLifeDome::lifeDomeTouched(CEntity *entityTouched)
	{
		auto temp = _lifeGiven.find(entityTouched->getEntityID());
		if(entityTouched->getEntityID() != _owner->getEntityID() && temp == _lifeGiven.end()){
			// He de comprobar que es amigo, o eso, o en el filtro que solo le de a los amigos
			std::shared_ptr<CMessageAddLife> addLifeMsg = std::make_shared<CMessageAddLife>();
			addLifeMsg->setAddLife(_life);
			entityTouched->emitMessage(addLifeMsg);

			// Lo insertamos en la lista para que no se le envie mas veces.
			_lifeGiven.insert(entityTouched->getEntityID());
			std::shared_ptr<CMessageAddLife> addLifeMsg2 = std::make_shared<CMessageAddLife>();
			addLifeMsg2->setAddLife(_lifePerFriend);
			_owner->emitMessage(addLifeMsg2);
		}

	}
	//________________________________________________________________________


} // namespace Logic

