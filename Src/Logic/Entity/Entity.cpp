/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author Antonio Jesús Narváez Corrales
@author Rubén Mulero Guerrero
@author Francisco Aisa García
@author Jaime Chapinal Cervantes


@date Junio, 2013
*/
#include "Components\SpawnPlayer.h"

#include "Entity.h"

// Componentes
#include "Component.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageTransform.h"

#include "Input/Server.h"
#include "Input/PlayerController.h"

#include "../../Audio/Server.h"

namespace Logic {

	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
										   _map(0),
										   _type(""), 
										   _name(""), 
										   _position(0,0,0),
										   _orientation(Ogre::Quaternion::IDENTITY),
										   _isPlayer(false), 
										   _activated(false) {
 

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity() {
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::dynamicSpawn(CMap* map, Map::CEntity* entityInfo) {
		Map::CEntity* newEntityInfo = entityInfo->clone(); 
		
		int entityId = getEntityID();
		std::ostringstream convert;
		convert << entityId;
		std::string nameId = convert.str();

		if(map->getEntityByName(newEntityInfo->getName()))
			newEntityInfo->setName(entityInfo->getStringAttribute("name") + nameId);

		return spawn(map, newEntityInfo);
	}

	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo) {
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();

		if(entityInfo->hasAttribute("name")) {
			_name = entityInfo->getStringAttribute("name");
		}

		if(entityInfo->hasAttribute("position")) {
			_position = entityInfo->getVector3Attribute("position");
		}

		if(entityInfo->hasAttribute("yaw")) {
			_orientation = Math::fromDegreesToQuaternion(entityInfo->getFloatAttribute("yaw"),Vector3(0,1,0));
		}
	
		if(entityInfo->hasAttribute("pitch")) {
			//Solo puede tener antes el yaw
			Quaternion pitchOrientation = Math::fromDegreesToQuaternion(entityInfo->getFloatAttribute("pitch"),Vector3(1,0,0));
			_orientation = Math::getYawQuaternion(_orientation) * pitchOrientation;
		}

		if(entityInfo->hasAttribute("roll")) { 
			//Puede tener tanto yaw como pitch
			Quaternion rollOrientation = Math::fromDegreesToQuaternion(entityInfo->getFloatAttribute("roll"),Vector3(0,0,1));
			_orientation = Math::getYawQuaternion(_orientation) * Math::getPitchQuaternion(_orientation) * rollOrientation;
		}

		if(entityInfo->hasAttribute("isPlayer")){
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		}

		// Inicializamos los componentes
		auto it = _components.begin();

		bool correct = true;

		for(int i = 1; it != _components.end() && correct; ++it, ++i ){
			correct = it->second.componentPtr->spawn(this,map,entityInfo) && correct;
			if(!correct)
				std::cout << "ERROR: La entidad " << entityInfo->getName() << " no ha podido ser ensamblada porque el componente " << i << " se ha muerto en el spawn" << std::endl;
		}
		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() {
		// Si somos jugador, se lo decimos al servidor
		// y nos registramos para que nos informen
		// de los movimientos que debemos realizar

		//Ademas como el sonido necesita saber la posición para actualizarse lo seteamos tambien

		if ( isPlayer() ) {
			CServer::getSingletonPtr()->setPlayer(this);
			Audio::CServer::getSingletonPtr()->setSoundAvatar(this);
			Input::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
		}

		// Activamos los componentes
		auto it = _components.begin();

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		for(; it != _components.end(); ++it) {
			if(it->second.componentPtr->isActivated())
				continue;
			it->second.componentPtr->activate();
			_activated = it->second.componentPtr->isActivated() && _activated;
		}

		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() {
		// Si éramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if ( isPlayer() ) {
			Input::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
			Audio::CServer::getSingletonPtr()->setSoundAvatar(NULL);
			CServer::getSingletonPtr()->setPlayer(0);
		}

		// Desactivamos los componentes
		for(auto it = _components.begin(); it != _components.end(); ++it)
			it->second.componentPtr->deactivate();

		_activated = false;

	} // deactivate
	//---------------------------------------------------------

	void CEntity::deactivateAllComponentsExcept(const std::set<std::string>& exceptionList) {
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			if( !exceptionList.count(it->first) ) {
				it->second.componentPtr->deactivate();
			}
		}
	}// deactivateAllComponentsExcept
	
	//---------------------------------------------------------

	void CEntity::deactivateComponents(const std::vector<std::string>& componentList) {
		TComponentMap::iterator it;
		for(unsigned int i = 0; i < componentList.size(); ++i) {
			it = _components.find(componentList[i]);
			if( it != _components.end() ) {
				it->second.componentPtr->deactivate();
			}
		}
	}
	
	//---------------------------------------------------------

	void CEntity::start() {
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			if( it->second.componentPtr->isActivated() ) {
				it->second.componentPtr->start();
			}
		}
	}

	//---------------------------------------------------------

	// @deprecated
	// Ahora mismo puedo hacer esta llamada y en cualquier momento
	// por que la lista de la stl me permite introducir el componente 
	// al final sin problemas incluso cuando se esta ejecutando un bucle for.
	// Cuando cambiemos a listas de prioridades esto puede ser un problema.
	void CEntity::wakeUp(IComponent* component) {
		if(component->_wantsTick) {
			_componentsWithTick.push_back(component);

			// Anotamos el iterador que se le asigna
			auto it = _components.find( component->getType() );
			if(it != _components.end()) {
				it->second.tickIterator = _componentsWithTick.rbegin();
			}

			// Reclamar ticks para la entidad si es necesario
			if( _componentsWithTick.size() == 1 ) _map->wantsTick(this);
		}
		if(component->_wantsFixedTick) {
			_componentsWithFixedTick.push_back(component);

			// Anotamos el iterador que se le asigna
			auto it = _components.find( component->getType() );
			if(it != _components.end()) {
				it->second.fixedTickIterator = _componentsWithFixedTick.rbegin();
			}

			// Reclamar fixed ticks para la entidad si es necesario
			if( _componentsWithFixedTick.size() == 1 ) _map->wantsFixedTick(this);
		}
	}

	//---------------------------------------------------------

	void CEntity::processComponentMessages() {
		IComponent* component;
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			component = it->second.componentPtr;

			// No hace falta comprobar si estamos activados o no u ociosos
			// para procesar mensajes, ya que eso se tiene en cuenta en el
			// emitMessage
			if( !component->isInDeepSleep() && component->processMessages() && component->isSleeping() ) {
				component->wakeUp();
			}
		}
	}

	//---------------------------------------------------------

	bool CEntity::tick(unsigned int msecs) {
		IComponent* component;
		std::list<IComponent*>::const_iterator it = _componentsWithTick.begin();
		while(it != _componentsWithTick.end()) {
			component = *it;

			if( component->isActivated() ) {
				if( !component->tick(msecs) ) {
					auto tempIt = _components.find( component->getType() );
					if(tempIt != _components.end()) tempIt->second.tickIterator = _componentsWithTick.rend();
					it = _componentsWithTick.erase(it);
					continue;
				}
			}
			
			++it;
		}

		return !_componentsWithTick.empty();
	} // tick

	//---------------------------------------------------------

	bool CEntity::fixedTick(unsigned int msecs) {
		IComponent* component;
		std::list<IComponent*>::const_iterator it = _componentsWithFixedTick.begin();
		while(it != _componentsWithFixedTick.end()) {
			component = *it;

			if( component->isActivated() ) {
				if( !component->fixedTick(msecs) ) {
					auto tempIt = _components.find( component->getType() );
					if(tempIt != _components.end()) tempIt->second.fixedTickIterator = _componentsWithFixedTick.rend();
					it = _componentsWithFixedTick.erase(it);
					continue;
				}
			}

			++it;
		}

		return !_componentsWithFixedTick.empty();
	}

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component, const std::string& id) {
		// Registramos en todas las listas a todos los componentes
		// ellos solos se desapuntaran en funcion de lo que quieran
		_componentsWithTick.push_back(component);
		_componentsWithFixedTick.push_back(component);

		// Anotamos los iteradores para realizar borrados rapidos
		ComponentInfo info;
		info.componentPtr = component;
		info.tickIterator = _componentsWithTick.rbegin();
		info.fixedTickIterator = _componentsWithFixedTick.rbegin();

		_components[id] = info;

		component->setType(id);
		component->setEntity(this);
	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component) {
		ComponentInfo info;
		bool success = false;
		for(auto it = _components.begin(); it != _components.end() && !success; ++it) {
			info = it->second;
			if(info.componentPtr == component) {
				if( info.tickIterator != _componentsWithTick.rend() )
					_componentsWithTick.erase( (++info.tickIterator).base() );
				if( info.fixedTickIterator != _componentsWithFixedTick.rend() )
					_componentsWithFixedTick.erase( (++info.fixedTickIterator).base() );
				
				_components.erase(it);
				
				success = true;
			}
		}

		return success;
	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents() {
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			delete it->second.componentPtr;
		}

		_components.clear();
		_componentsWithTick.clear();
		_componentsWithFixedTick.clear();
	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(const std::shared_ptr<CMessage>& message, IComponent* emitter) {
		// Para saber si alguien quiso el mensaje.
		IComponent* component;
		bool anyReceiver = false;
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			component = it->second.componentPtr;
			// Si el componente es el propio emisor no se le envia el mensaje
			// Ademas solo recibe mensajes si esta activado y no esta ocioso
			if( emitter != component && component->isActivated() && !component->isBusy() )
				anyReceiver = component->set(message) || anyReceiver;
		}

		return anyReceiver;
	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) {
		_position = position;
	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Quaternion& orientation) {
		_orientation = orientation;
	} // setOrientation

	//---------------------------------------------------------

	Ogre::Quaternion CEntity::getOrientation() const {
		return _orientation;
	} // getOrientation
	//---------------------------------------------------------

	void CEntity::setYaw(const Quaternion &yaw){
		_orientation = yaw * Math::getPitchQuaternion(_orientation) * Math::getRollQuaternion(_orientation);
	}
	// setYaw
	//---------------------------------------------------------

	void CEntity::setPitch(const Quaternion &pitch) {
		_orientation = Math::getYawQuaternion(_orientation) * pitch * Math::getRollQuaternion(_orientation);
	} // setPitch
	//---------------------------------------------------------

	void CEntity::setRoll(const Quaternion &roll) {
		_orientation = Math::getYawQuaternion(_orientation) * Math::getPitchQuaternion(_orientation) * roll;
	} // setRoll
	//---------------------------------------------------------

	Quaternion CEntity::getYaw(){
		return Math::getYawQuaternion(_orientation);
	} // getYaw
	//---------------------------------------------------------

	Quaternion CEntity::getPitch(){
		return Math::getPitchQuaternion(_orientation);
	} // getPitch
	//---------------------------------------------------------

	Quaternion CEntity::getRoll(){
		return Math::getRollQuaternion(_orientation);
	} // getRoll
	//---------------------------------------------------------

	void CEntity::rotate(int orientation, Ogre::Radian rotation){
		Quaternion yawOrientation = Math::getYawQuaternion(_orientation);
		Quaternion pitchOrientation = Math::getPitchQuaternion(_orientation);
		Quaternion rollOrientation = Math::getRollQuaternion(_orientation);
		switch(orientation){
			case Orientation::eYAW:{
				Math::rotate(Vector3::UNIT_Y,rotation,yawOrientation);
				break;
			}
			case Orientation::ePITCH:{
				Math::rotate(Vector3::UNIT_X,rotation,pitchOrientation);
				break;
			}
			case Orientation::eROLL:{
				Math::rotate(Vector3::UNIT_Z,rotation,rollOrientation);
				break;
			}
		}
		//Actualizamos la orientacion final
		_orientation = yawOrientation * pitchOrientation * rollOrientation;
	}

} // namespace Logic
