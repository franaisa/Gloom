/**
@file PhysicStaticCompound.cpp

@see Logic::CPhysicStaticCompound
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#include "PhysicStaticCompound.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/GeometryFactory.h"
#include "Physics/MaterialManager.h"
#include "Physics/StaticEntity.h"

#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageTouched.h"
#include "Logic/Messages/MessageUntouched.h"
#include "Logic/Messages/MessageSetPhysicPosition.h"
#include "Logic/Messages/MessageAddForcePhysics.h"
#include "Logic/Messages/MessageContactEnter.h"
#include "Logic/Messages/MessageContactExit.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CPhysicStaticCompound);

	//________________________________________________________________________

	CPhysicStaticCompound::CPhysicStaticCompound() {
		// Nada que hacer
	}

	//________________________________________________________________________

	CPhysicStaticCompound::~CPhysicStaticCompound() {
		// El destructor de _physicEntity ya sabe que hacer
		for(int i = 0; i < _physicEntitiesList.size(); ++i) {
			delete _physicEntitiesList[i];
		}

		_physicEntitiesList.clear();
	} 

	//________________________________________________________________________

	bool CPhysicStaticCompound::spawn(Logic::CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		// Invocar al método de la clase padre
		if( !IComponent::spawn(entity, map, entityInfo) ) return false;

		// Crear el objeto físico asociado al componente
		createPhysicEntity(entityInfo);

		return true;
	} 

	//________________________________________________________________________

	bool CPhysicStaticCompound::accept(const shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ACTIVATE;
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::ACTIVATE: {
				bool activate = static_pointer_cast<CMessageActivate>(message)->getActivated();
				if(activate)
					activateSimulation();
				else
					deactivateSimulation();
				
				break;
			}
		}
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, vector<int>& groupList) {
		// Leer el grupo de colisión (por defecto grupo 0)
		if (entityInfo->hasAttribute("physic_group"))
			group = entityInfo->getIntAttribute("physic_group");

		// Comprobamos los grupos con los que esta entidad deberia colisionar
		if (entityInfo->hasAttribute("physic_groupList")) {
			istringstream groupListString(entityInfo->getStringAttribute("physic_groupList"));

			// Para cada cadena entre comas...
			do {
				string groupNumber;
				getline(groupListString, groupNumber, ','); // linea entre delimitadores
				
				istringstream str(groupNumber);     // wrappeamos cadena como Input Stream
				do {									// Le quitamos los espacios...
					getline(str, groupNumber, ' ');  // linea entre espacios
				} while (groupNumber.size() == 0 && !str.eof());

				groupList.push_back( atoi(groupNumber.c_str()) );
			} while (!groupListString.eof());
		}
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::createPhysicEntity(const Map::CEntity *entityInfo) {
		// Leemos la informacion de grupos de colision
		int group = 0;
		vector<int> groupList;
		readCollisionGroupInfo(entityInfo, group, groupList);

		// Deserializamos las entidades fisicas
		deserializeEntities(entityInfo, group, groupList);
	}


	//________________________________________________________________________

	void CPhysicStaticCompound::deserializeEntities(const Map::CEntity *entityInfo, int group, const vector<int>& groupList) {	
		assert(entityInfo->hasAttribute("physic_entityList"));
		istringstream physicEntities( entityInfo->getStringAttribute("physic_entityList") );

		bool isTrigger = false;
		if( entityInfo->hasAttribute("physic_trigger") )
			isTrigger = entityInfo->getBoolAttribute("physic_trigger");

		Physics::CServer* physicsServer = Physics::CServer::getSingletonPtr();
		Physics::CGeometryFactory* geometryFactory = Physics::CGeometryFactory::getSingletonPtr();
		Physics::CMaterialManager* materialManager = Physics::CMaterialManager::getSingletonPtr();

		// GUARRER MODE ON

		string att, shapeType;
		float radius;
		Vector3 dimensions, position;
		do {
			physicEntities >> att;
			if(att == "physic_position") {
				// Leemos el =
				physicEntities >> att;
				// Leemos la posicion
				physicEntities >> position.x;
				physicEntities >> position.y;
				physicEntities >> position.z;
			}

			physicEntities >> att;
			if(att == "physic_shape") {
				// Leemos el =
				physicEntities >> att;
				physicEntities >> shapeType;

				if(shapeType == "sphere") {
					// Leer el radio
					physicEntities >> att;
					if(att == "physic_radius") {
						// Leemos el =
						physicEntities >> att;
						physicEntities >> radius;

						Physics::SphereGeometry sphere = geometryFactory->createSphere(radius);
						Physics::Material* defaultMaterial = materialManager->getMaterial(Physics::MaterialType::eDEFAULT);

						Physics::CStaticEntity* newEntity = new Physics::CStaticEntity();
						newEntity->load(position, Quaternion::IDENTITY, sphere, *defaultMaterial, isTrigger, group, groupList, this);
						_physicEntitiesList.push_back(newEntity);
					}
				}
				else if(shapeType == "box") {
					// Leer las dimensiones
					physicEntities >> att;
					if(att == "physic_dimensions") {
						// Leemos el =
						physicEntities >> att;
						// Leemos las dimensiones
						physicEntities >> dimensions.x;
						physicEntities >> dimensions.y;
						physicEntities >> dimensions.z;

						Physics::BoxGeometry box = geometryFactory->createBox(dimensions);
						Physics::Material* defaultMaterial = materialManager->getMaterial(Physics::MaterialType::eDEFAULT);
		
						Physics::CStaticEntity* newEntity = new Physics::CStaticEntity();
						newEntity->load(position, Quaternion::IDENTITY, box, *defaultMaterial, isTrigger, group, groupList, this);
						_physicEntitiesList.push_back(newEntity);
					}
				}
			}

		} while (!physicEntities.eof());

		// GUARRER MODE OFF
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::onTrigger(IPhysics *otherComponent, bool enter) {
		// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
		// todos los componentes de la entidad. 
		if(enter) {
			shared_ptr<CMessageTouched> triggerEnterMsg = make_shared<CMessageTouched>();
			triggerEnterMsg->setEntity( otherComponent->getEntity() );
			_entity->emitMessage(triggerEnterMsg);
		} 
		else {
			shared_ptr<CMessageUntouched> triggerExitMsg = make_shared<CMessageUntouched>();
			triggerExitMsg->setEntity( otherComponent->getEntity() );
			_entity->emitMessage(triggerExitMsg);
		}
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::onContact (IPhysics *otherComponent,bool enter) {
		if(enter) {
			shared_ptr<CMessageContactEnter> contactEnterMsg = make_shared<CMessageContactEnter>();
			contactEnterMsg->setEntity( otherComponent->getEntity() );
			_entity->emitMessage(contactEnterMsg);
		} 
		else {
			shared_ptr<CMessageContactExit> contactExitMsg = make_shared<CMessageContactExit>();
			contactExitMsg->setEntity(otherComponent->getEntity()->getEntityID());
			_entity->emitMessage(contactExitMsg);
		}
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::deactivateSimulation() {
		for(int i = 0; i < _physicEntitiesList.size(); ++i) {
			_physicEntitiesList[i]->deactivateSimulation();
		}
	}

	//________________________________________________________________________

	void CPhysicStaticCompound::activateSimulation() {
		for(int i = 0; i < _physicEntitiesList.size(); ++i) {
			_physicEntitiesList[i]->activateSimulation();
		}
	}

}