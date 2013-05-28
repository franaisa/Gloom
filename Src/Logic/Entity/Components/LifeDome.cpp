/**
@file LifeDome.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifeDome
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Mayo, 2013
*/

#include "LifeDome.h"

#include "PhysicDynamicEntity.h"

#include "Graphics/Entity.h"
#include "Graphics.h"

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

		return msgType == Message::DAMAGED || msgType == Message::SET_REDUCED_DAMAGE;
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
		}
	} // process

	//________________________________________________________________________

	void CLifeDome::onStart() {
		
	}

	/*void CLifeDome::onTick(unsigned int msecs) {
		std::cout << "Ola ke ase " << std::endl;
	}*/

	void CLifeDome::onFixedTick(unsigned int msecs) {
	
		CGraphics* cGraph;
		cGraph = _entity->getComponent<CGraphics>("CGraphics");
		if (cGraph)
		{
			if (_scale < 10.0f)
			{
				_scale += 0.005f;
			}
			cGraph->changeScale(_scale);
			std::cout << "posicion " << _entity->getPosition() << std::endl;
			cGraph->setPosition(_entity->getPosition());
		}
	}

	//________________________________________________________________________

	void CLifeDome::onActivate() {
		//_reducedDamageAbsorption = 0;
	}


} // namespace Logic

