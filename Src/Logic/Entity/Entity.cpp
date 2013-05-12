/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
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

#include "Logic/Messages/MessageHudDebugData.h"

namespace Logic 
{

	unsigned int CEntity::_fixedTimeStep = 16;

	//---------------------------------------------------------

	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
										   _map(0),
										   _acumTime(0),
										   _type(""), 
										   _name(""), 
										   _transform(Matrix4::IDENTITY),
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

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		//Si se establecen los 2 hay que hacerlo a la vez
		if(entityInfo->hasAttribute("pitch") && entityInfo->hasAttribute("yaw")){
			float pitch = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("pitch"));
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("yaw"));
			Math::setPitchYaw(pitch,yaw,_transform);
		}


		// Por comodidad en el mapa escribimos los ángulos en grados.
		else if(entityInfo->hasAttribute("pitch"))
		{ 
			float pitch = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("pitch"));
			Math::pitch(pitch,_transform);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		else if(entityInfo->hasAttribute("yaw"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("yaw"));
			Math::yaw(yaw,_transform);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		

		// Inicializamos los componentes
		TComponentList::const_iterator it = _componentList.begin();

		bool correct = true;

		for(int i = 1; it != _componentList.end() && correct; ++it, ++i ){
			correct = (*it)->spawn(this,map,entityInfo) && correct;
			if(!correct)
				std::cout << "ERROR: La entidad " << entityInfo->getName() << " no ha podido ser ensamblada porque el componente " << i << " se ha muerto en el spawn" << std::endl;
		}
		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() {
		initFunctionPointers();

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
		TComponentList::const_iterator it = _componentList.begin();

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		for(; it != _componentList.end(); ++it){
			(*it)->activate();
			_activated = (*it)->isActivated() && _activated;
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
			CServer::getSingletonPtr()->setPlayer(0);
		}

		TComponentList::const_iterator it = _componentList.begin();

		// Desactivamos los componentes
		for(; it != _componentList.end(); ++it)
			(*it)->deactivate();

		_activated = false;

	} // deactivate
	//---------------------------------------------------------

	void CEntity::deactivateAllComponentsExcept (std::set<std::string> exceptionList) {
		TComponentMap::iterator it = _components.begin();
		for(; it != _components.end(); ++it) {
			if( !exceptionList.count(it->first) ) {
				it->second->deactivate();
			}
		}
	}// deactivateAllComponentsExcept

	//---------------------------------------------------------

	void CEntity::start() {
		IComponent* component;
		TComponentList::const_iterator it = _componentList.begin();
		for(; it != _componentList.end(); ++it) {
			component = *it;
			if( component->isActivated() ) {
				component->start();
			}
		}
	}

	//---------------------------------------------------------

	void CEntity::tick(unsigned int msecs) {
		// Estimamos el numero de pasos que tiene que hacer el fixed tick
		_acumTime += msecs;
		unsigned int steps = _acumTime / _fixedTimeStep;

		_acumTime  = _acumTime % _fixedTimeStep;
		std::shared_ptr<CMessageHudDebugData> m = std::make_shared<CMessageHudDebugData>();
		m->setKey("msecs");
		m->setValue(msecs);
		this->emitMessage(m);

		std::shared_ptr<CMessageHudDebugData> m2 = std::make_shared<CMessageHudDebugData>();
		m2->setKey("steps");
		m2->setValue(steps);
		this->emitMessage(m2);

		IComponent* component;
		TComponentList::const_iterator it = _componentList.begin();
		for(; it != _componentList.end(); ++it) {
			component = *it;
			if( component->isActivated() ) {
				// Ejecutamos el puntero a funcion que toque
				(this->*this->entityProcessMode[component->_state][component->_tickMode])(component, msecs, steps);
			}
		}
	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component, const std::string& id) {
		_components[id] = component;
		_componentList.push_back(component);

		component->setEntity(this);
	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component) {
		TComponentMap::const_iterator itMap = _components.begin();
		TComponentList::const_iterator itList = _componentList.begin();

		bool mapErase = false;
		for(; itMap != _components.end() && !mapErase; ++itMap) {
			if(itMap->second == component) {
				_components.erase(itMap);
				mapErase = true;
			}
		}

		bool listErase = false;
		for(; itList != _componentList.end() && !listErase; ++itList) {
			if(*itList == component) {
				_componentList.erase(itList);
				listErase = true;
			}
		}

		return mapErase && listErase;
	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents() {
		_components.clear();

		TComponentList::const_iterator it = _componentList.begin();
		for(; it != _componentList.end(); ++it) {
			delete *it;
		}

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(const std::shared_ptr<CMessage>& message, IComponent* emitter) {
		TComponentList::const_iterator it = _componentList.begin();
		// Para saber si alguien quiso el mensaje.
		IComponent* component;
		bool anyReceiver = false;
		for(; it != _componentList.end(); ++it) {
			component = *it;
			// Al emisor no se le envia el mensaje y si esta desactivado el componente tampoco se le envia
			if( emitter != component && component->isActivated() && component->_state != ComponentState::eBUSY )
				anyReceiver = component->set(message) || anyReceiver;
		}

		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;
	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) 
	{
		_transform.setTrans(position);
	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;
	} // setOrientation

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;
	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYaw(float yaw) 
	{
		Math::setYaw(yaw,_transform);
	} // setYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);
	} // yaw

	//---------------------------------------------------------

	void CEntity::setPitch(float pitch) 
	{
		Math::setPitch(pitch,_transform);
	} // setPitch

	//---------------------------------------------------------

	void CEntity::pitch(float pitch) 
	{
		Math::pitch(pitch,_transform);
	} // pitch

	//---------------------------------------------------------

	void CEntity::roll(float roll) {
		Math::roll(roll, _transform);
	}

	void CEntity::setYawPitch(float yaw, float pitch) {
		Math::pitchYaw(pitch, yaw, _transform);
	}










	void CEntity::initFunctionPointers() {
		entityProcessMode[ComponentState::eSLEEPING][TickMode::eNONE]		= &CEntity::sleepingWithoutTick;
		entityProcessMode[ComponentState::eSLEEPING][TickMode::eTICK]		= &CEntity::sleepingWithTick;
		entityProcessMode[ComponentState::eSLEEPING][TickMode::eFIXED_TICK] = &CEntity::sleepingWithFixedTick;
		entityProcessMode[ComponentState::eSLEEPING][TickMode::eBOTH]		= &CEntity::sleepingWithBothTicks;

		entityProcessMode[ComponentState::eAWAKE][TickMode::eNONE]			= &CEntity::awakeWithoutTick;
		entityProcessMode[ComponentState::eAWAKE][TickMode::eTICK]			= &CEntity::awakeWithTick;
		entityProcessMode[ComponentState::eAWAKE][TickMode::eFIXED_TICK]	= &CEntity::awakeWithFixedTick;
		entityProcessMode[ComponentState::eAWAKE][TickMode::eBOTH]			= &CEntity::awakeWithBothTicks;

		entityProcessMode[ComponentState::eBUSY][TickMode::eNONE]			= &CEntity::busyWithoutTick;
		entityProcessMode[ComponentState::eBUSY][TickMode::eTICK]			= &CEntity::busyWithTick;
		entityProcessMode[ComponentState::eBUSY][TickMode::eFIXED_TICK]		= &CEntity::busyWithFixedTick;
		entityProcessMode[ComponentState::eBUSY][TickMode::eBOTH]			= &CEntity::busyWithBothTicks;
	}




	void CEntity::sleepingWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		if( !component->_deepSleep && component->processMessages() ) {
			component->_state = ComponentState::eAWAKE;
			component->onWake();

			// No deberia ocurrir nunca
			std::cerr << "WARNING!!! Existe un componente durmiendo sin tick" << std::endl;
		}
	}

	void CEntity::sleepingWithTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		if( !component->_deepSleep && component->processMessages() ) {
			component->_state = ComponentState::eAWAKE;
			component->onWake();

			component->tick(msecs);
		}
	}

	void CEntity::sleepingWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		if( !component->_deepSleep && component->processMessages() ) {
			component->_state = ComponentState::eAWAKE;
			component->onWake();

			for(int i = 0; i < steps; ++i) {
				component->fixedTick(_fixedTimeStep);
			}
		}
	}

	void CEntity::sleepingWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps) {
		if( !component->_deepSleep && component->processMessages() ) {
			component->_state = ComponentState::eAWAKE;
			component->onWake();

			component->tick(msecs);
			for(int i = 0; i < steps; ++i) {
				component->fixedTick(_fixedTimeStep);
			}
		}
	}




	void CEntity::awakeWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->processMessages();
	}

	void CEntity::awakeWithTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->processMessages();
		component->tick(msecs);
	}

	void CEntity::awakeWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->processMessages();





























		

		for(int i = 0; i < steps; ++i) {
			component->fixedTick(_fixedTimeStep);
		}
	}

	void CEntity::awakeWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->processMessages();
		component->tick(msecs);
		if(steps>1)std::cout << "WARNING: estas ejecutando la logica " << steps  << " veces!" << std::endl;
		for(int i = 0; i < steps; ++i) {
			component->fixedTick(_fixedTimeStep);
		}
	}




	void CEntity::busyWithoutTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		// No deberia de ocurrir nunca
		std::cerr << "WARNING!!! Existe un componente busy sin tick" << std::endl;
	}

	void CEntity::busyWithTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->tick(msecs);
	}

	void CEntity::busyWithFixedTick(IComponent* component, unsigned int msecs, unsigned int steps) {
		for(int i = 0; i < steps; ++i) {
			component->fixedTick(_fixedTimeStep);
		}
	}

	void CEntity::busyWithBothTicks(IComponent* component, unsigned int msecs, unsigned int steps) {
		component->tick(msecs);
		for(int i = 0; i < steps; ++i) {
			component->fixedTick(_fixedTimeStep);
		}
	}

} // namespace Logic
