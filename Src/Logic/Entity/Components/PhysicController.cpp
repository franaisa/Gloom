/**
@file PhysicController.cpp

Contiene la implementación del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Messages/MessageAvatarWalk.h"
#include "Logic/Messages/MessageCollisionDown.h"
#include "Logic/Messages/MessageCealing.h"
#include "Logic/Messages/MessageSide.h"

#include "AvatarController.h"

#include <PxPhysicsAPI.h>


using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(), _controller(NULL), 
								       _movement(0,0,0), _falling(false)
{
	_server = CServer::getSingletonPtr();
}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	_server = NULL;
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);
	// Seteo de _falling a false para que se envie el primer mensaje de actualizacion
	_falling=false;
	return true;
}

//---------------------------------------------------------

bool CPhysicController::accept(CMessage *message)
{
	return message->getMessageType() == Message::AVATAR_WALK;
} 

//---------------------------------------------------------

void CPhysicController::process(CMessage *message)
{
	switch(message->getMessageType())
	{
	case Message::AVATAR_WALK:
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo sólo tendremos en cuenta el último.
		_movement = ((CMessageAvatarWalk*)message)->getDirection();
		break;
	}

}


//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al método de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Intentamos mover el controller a la posición recibida en el último mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = _server->moveController(_controller, _movement, msecs);

	// Actualizar la posición y orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	_entity->setPosition(_server->getControllerPosition(_controller));

	//Si tocamos con el techo lo notificamos
	if((flags & PxControllerFlag::eCOLLISION_UP)){
		Logic::CMessageCealing *em=new Logic::CMessageCealing();
		_entity->emitMessage(em);
	}
	//Si tocamos el lateral tenemos que parar la inercia de la direccion
	if((flags & PxControllerFlag::eCOLLISION_SIDES)){
		Logic::CMessageSide *side=new Logic::CMessageSide();
		_entity->emitMessage(side);
	}
	//Si hay cambio de estado en el flag de tocar suelo
	if(_falling != !(flags & PxControllerFlag::eCOLLISION_DOWN)){
		// Actualizamos el flag que indica si estamos cayendo
		_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);
		//Mandamos un mensaje que dirá si hay collision con el suelo para la lógica
		Logic::CMessageCollisionDown *m=new Logic::CMessageCollisionDown();
		m->setCollisionDown(_falling);
		_entity->emitMessage(m);
	}
	_movement = Vector3::ZERO;

}

//---------------------------------------------------------

void  CPhysicController::setPhysicPosition (const Vector3 &position){
	//Teletransportamos al player y ponemos la logica en el mismo momento(sino ocurrirían teletransportaciones gráficas)
	_server->setControllerPosition(_controller, position);
	_entity->setPosition(_server->getControllerPosition(_controller));
}


//---------------------------------------------------------

void CPhysicController::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList) {
	// Leer el grupo de colisión (por defecto grupo 0)
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Comprobamos los grupos con los que esta entidad deberia colisionar
	if (entityInfo->hasAttribute("physic_groupList")) {
		std::istringstream groupListString(entityInfo->getStringAttribute("physic_groupList"));

		// Para cada cadena entre comas...
		do {
			std::string groupNumber;
			std::getline(groupListString, groupNumber, ','); // linea entre delimitadores
				
			std::istringstream str(groupNumber);     // wrappeamos cadena como Input Stream
			do {									// Le quitamos los espacios...
				std::getline(str, groupNumber, ' ');  // linea entre espacios
			} while (groupNumber.size() == 0 && !str.eof());

			groupList.push_back( atoi(groupNumber.c_str()) );
		} while (!groupListString.eof());
	}
}

//---------------------------------------------------------

PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posición de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulación.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisión del controller. Por ahora sólo admitimos cápsulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la cápsula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la cápsula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	int group = 0;
	std::vector<int> groupList;
	readCollisionGroupInfo(entityInfo, group, groupList);

	// Crear el controller de tipo cápsula
	return _server->createCapsuleController(position, group, groupList, radius, height, this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad estática no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor().isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinemática no hacemos nada
	if (_server->isKinematic(actor))
		return;

	// Aplicar una fuerza a la entidad en la dirección del movimiento
	//actor->addForce(hit.dir * hit.length * 1000.0f);
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{

}

//---------------------------------------------------------


void CPhysicController::deactivateSimulation() {
	// Desactivamos todos los shapes del componente por completo en PhysX
	// Para ello, obtenemos todos sus shapes y ponemos los flags a false

	int nbShapes = _controller->getActor()->getNbShapes();
	PxShape** actorShapes = new PxShape* [nbShapes];
	_controller->getActor()->getShapes(actorShapes, nbShapes);
	for(int i = 0; i < nbShapes; ++i) {
		// Esta shape no tomara parte en barridos, raycasts...
		actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, false);
		// Esta shape no entrara dentro de la simulacion de fisicas
		actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , false);
	}
	_controller->setInteraction(PxCCTInteractionMode::eEXCLUDE);
	delete [] actorShapes;
}

//---------------------------------------------------------

void CPhysicController::activateSimulation() {
	// Activamos todos los shapes del componente por completo en PhysX
	// Para ello, obtenemos todos sus shapes y ponemos los flags a true

	int nbShapes = _controller->getActor()->getNbShapes();
	PxShape** actorShapes = new PxShape* [nbShapes];
	_controller->getActor()->getShapes(actorShapes, nbShapes);
	for(int i = 0; i < nbShapes; ++i) {
		// Esta shape tomara parte en barridos, raycasts...
		actorShapes[i]->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		// Esta shape entrara dentro de la simulacion de fisicas
		actorShapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE , true);
	}
	_controller->setInteraction(PxCCTInteractionMode::eINCLUDE);
	delete [] actorShapes;
}
