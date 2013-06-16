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

#include "Archangel.h"

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
				lifeDomeTouched();
				break;
			}
		}
	} // process

	//________________________________________________________________________

	void CLifeDome::onStart() {
		
	}
	//________________________________________________________________________
	
	void CLifeDome::setOwner(Logic::CArchangel *owner)
	{
		_owner = owner;
	} // setOwner
	//________________________________________________________________________

	void CLifeDome::onFixedTick(unsigned int msecs) {

		if (_owner)
		{
			_entity->setPosition(_owner->getEntity()->getPosition());
		}
		
		CGraphics* cGraph;
		cGraph = _entity->getComponent<CGraphics>("CGraphics");
		if (cGraph)
		{
			printf("\n Entre");
			if (_scale < 10.0f)
			{
				_scale += 0.005f;
			}
			std::cout << "scale--------->" << _scale << std::endl;
			cGraph->changeScale(_scale);
			//Ponemos la posición del jugador subiéndolo un poco en el ejeY
			//cGraph->setPosition(_owner->getEntity()->getPosition() + Vector3(0,1,0));
		}
	}
	

	//________________________________________________________________________

	void CLifeDome::onActivate() {
		//_reducedDamageAbsorption = 0;
	}
	//________________________________________________________________________

	void CLifeDome::lifeDomeTouched()
	{
		std::cout << "Tocado! al principio me toca a mi" << std::endl;
	}
	//________________________________________________________________________


} // namespace Logic

