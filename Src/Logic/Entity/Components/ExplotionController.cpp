/**
@file ExplotionController.cpp

@see Logic::IComponent

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "ExplotionController.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"

#include "Logic/Entity/Components/ExplotionHitNotifier.h"

#include "Logic/Messages/MessageSetPhysicPosition.h"

namespace Logic {
	
	IMP_FACTORY(CExplotionController);
	
	//________________________________________________________________________

	void CExplotionController::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		// Actualizamos el timer. Si se ha cumplido el tiempo limite de explosion
		// eliminamos la entidad granada y creamos la entidad explosion.
		_timer += msecs;
		if(_timer > _explotionTime) {
			// Eliminamos la entidad en diferido
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			// Creamos la explosion
			createExplotion();
		}
	} // tick

	//________________________________________________________________________

	bool CExplotionController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// Leer el timer que controla la explosion
		if( entityInfo->hasAttribute("explotionTime") ) {
			// Pasamos a msecs
			_explotionTime = entityInfo->getFloatAttribute("explotionTime") * 1000;
		}

		return true;

	} // spawn

	//________________________________________________________________________

	void CExplotionController::createExplotion() {
		// Obtenemos la informacion asociada al arquetipo de la explosion de la granada
		Map::CEntity *entityInfo = CEntityFactory::getSingletonPtr()->getInfo("Explotion");
		// Creamos la entidad y la activamos
		CEntity* grenadeExplotion = CEntityFactory::getSingletonPtr()->createEntity( entityInfo, Logic::CServer::getSingletonPtr()->getMap() );
		grenadeExplotion->activate();

		// Enviamos el mensaje para situar a la explosion en el punto en el que estaba la granada
		Logic::CMessageSetPhysicPosition* msg = new Logic::CMessageSetPhysicPosition();
		msg->setPosition( _entity->getPosition() ); // spawneamos la explosion justo en el centro de la granada
		msg->setMakeConversion(false);
		grenadeExplotion->emitMessage(msg);

		// Seteamos la entidad que dispara la granada
		CExplotionHitNotifier* comp = grenadeExplotion->getComponent<CExplotionHitNotifier>("CExplotionHitNotifier");
		assert(comp != NULL);
		comp->setOwner(_owner);
	} // createExplotion

	//________________________________________________________________________

	void CExplotionController::setOwner(CEntity* owner) {
		this->_owner = owner;
	}

} // namespace Logic

