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

namespace Logic {

	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
										   _map(0),
										   _type(""), 
										   _name(""), 
										   _transform(Matrix4::IDENTITY),
										   _position(0,0,0),
										   _orientation(1,0,0,0),
										   _yawOrientation(1,0,0,0),
										   _pitchOrientation(1,0,0,0),
										   _rollOrientation(1,0,0,0),//identidad
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
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
			_position=position;
		}

		// Si se establecen los 2 hay que hacerlo a la vez
		if(entityInfo->hasAttribute("pitch") && entityInfo->hasAttribute("yaw")) {
			float pitch = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("pitch"));
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("yaw"));
			Math::setPitchYaw(pitch,yaw,_transform);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		else if(entityInfo->hasAttribute("pitch")) { 
			float pitch = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("pitch"));
			Math::pitch(pitch,_transform);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		else if(entityInfo->hasAttribute("yaw")) {
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("yaw"));
			Math::yaw(yaw,_transform);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		

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

	void CEntity::deactivateAllComponentsExcept (std::set<std::string> exceptionList) {
		for(auto it = _components.begin(); it != _components.end(); ++it) {
			if( !exceptionList.count(it->first) ) {
				it->second.componentPtr->deactivate();
			}
		}
	}// deactivateAllComponentsExcept

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

	void CEntity::setTransform(const Matrix4& transform) {
		_transform = transform;
		_position=transform.getTrans();
	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) {
		_position=position;
		_transform.setTrans(position);
	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Ogre::Quaternion& orientation) {
		_orientation = orientation;
		Matrix3 rot;
		orientation.ToRotationMatrix(rot);
		_transform = rot;
	} // setOrientation

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const {
		Matrix3 rot;
		_orientation.ToRotationMatrix(rot);
		return rot;
	} // getOrientation
	//---------------------------------------------------------

	Ogre::Quaternion CEntity::getQuatOrientation() const {
		//Hay que formarla de acuerdo a los nodos
		return _yawOrientation*_pitchOrientation*_rollOrientation;
	} // getOrientation
	//---------------------------------------------------------

	void CEntity::setYaw(const Quaternion &yaw, bool reset){
		 _yawOrientation=yaw;
		if(reset){
			_rollOrientation=Quaternion(1,0,0,0);
			_pitchOrientation=Quaternion(1,0,0,0);
		 }
		setOrientation(getQuatOrientation());
	}
	// setYaw
	//---------------------------------------------------------

	void CEntity::setPitch(const Quaternion &pitch, bool reset) {
		_pitchOrientation=pitch;
		if(reset){
			_rollOrientation=Quaternion(1,0,0,0);
			_yawOrientation=Quaternion(1,0,0,0);
		 }
		setOrientation(getQuatOrientation());
	} // setPitch
	//---------------------------------------------------------

	void CEntity::setRoll(const Quaternion &roll, bool reset) {
		_rollOrientation=roll;
		if(reset){
			_pitchOrientation=Quaternion(1,0,0,0);
			_yawOrientation=Quaternion(1,0,0,0);
		 }
		setOrientation(getQuatOrientation());
	} // setRoll
	//---------------------------------------------------------

	void CEntity::setYawPitchMouse(float yaw, float pitch) {

		 Ogre::Real pitchAngle;
		 Ogre::Real pitchAngleSign;
 
		 //Rotamos el Yaw de la entidad de acuerdo a los grados en radianes pasados como parámetro.
		 rotate(Orientation::eYAW,yaw);
 
		 //Rotamos el Pitch de la entidad de acuerdo a los grados en radianes pasados como parámetro.
		 rotate(Orientation::ePITCH,pitch);
		
		 // Ángulo de rotación sobre el eje X.
		 pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(_pitchOrientation.w)).valueDegrees());
 
		 // Para saber el sentido.
		 pitchAngleSign = _pitchOrientation.x;
 
		 // Limitamos el angulo de -90 a +90 como en el Quake3.
		 if (pitchAngle > 90.0f)
		 {
			 if (pitchAngleSign > 0)
				 //Fijando a +90.
				 _pitchOrientation=Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),Ogre::Math::Sqrt(0.5f), 0, 0);
			 else if (pitchAngleSign < 0)
				 //Fijando a -90.
				 _pitchOrientation=Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),	-Ogre::Math::Sqrt(0.5f), 0, 0);
		 }
		 
		 //Actualizamos la orientacion
		setOrientation(_yawOrientation*_pitchOrientation*_rollOrientation);
	}

	void CEntity::rotate(int orientation, float rotation){
		switch(orientation){
			case Orientation::eYAW:{
				Quaternion q(Ogre::Radian(rotation), Vector3::UNIT_Y); 
				_yawOrientation=_yawOrientation*q;
				break;
			}
			case Orientation::ePITCH:{
				Quaternion q(Ogre::Radian(rotation), Vector3::UNIT_X); 
				_pitchOrientation=_pitchOrientation*q;
				break;
			}
			case Orientation::eROLL:{
				//No la he probado
				Quaternion q(Ogre::Radian(rotation), Vector3::UNIT_Z); 
				_rollOrientation=_rollOrientation*q;
				break;
			}
		}
	
	}

	/*float Quaternion::getPitchQuat()
	{
	  return atan2(2*(y*z + w*x), w*w - x*x - y*y + z*z);
	}

	float Quaternion::getYawQuat()
	{
	  return asin(-2*(x*z - w*y));
	}

	float Quaternion::getRollQuat()
	{
	  return atan2(2*(x*y + w*z), w*w + x*x - y*y - z*z);
	}*/

} // namespace Logic
