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

namespace Logic 
{

	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
										   _map(0),
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
				std::cout << "ERROR: La entidad no ha podido ser ensamblada porque el componente " << i << " se ha muerto en el spawn" << std::endl;
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

			// Registramos en todas las listas a todos los componentes
			// ellos solos se desapuntaran en funcion de lo que quieran
			_componentsWithTick.push_back(component);
			_componentsWithFixedTick.push_back(component);
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

			// Reclamar ticks para la entidad si es necesario
			if( _componentsWithTick.size() == 1 ) _map->wantsTick(this);
		}
		if(component->_wantsFixedTick) {
			_componentsWithFixedTick.push_back(component);

			// Reclamar fixed ticks para la entidad si es necesario
			if( _componentsWithFixedTick.size() == 1 ) _map->wantsFixedTick(this);
		}
	}

	//---------------------------------------------------------

	void CEntity::processComponentMessages() {
		IComponent* component;
		std::list<IComponent*>::const_iterator it = _componentList.begin();
		for(; it != _componentList.end(); ++it) {
			component = *it;

			// No hace falta comprobar si estamos activados o no u ociosos
			// para procesar mensajes, ya que eso se tiene en cuenta en el
			// emitMessage
			if( component->processMessages() && component->isSleeping() && !component->isInDeepSleep() ) {
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
			// Si el componente es el propio emisor no se le envia el mensaje
			// Ademas solo recibe mensajes si esta activado y no esta ocioso
			if( emitter != component && component->isActivated() && !component->isBusy() )
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

} // namespace Logic
