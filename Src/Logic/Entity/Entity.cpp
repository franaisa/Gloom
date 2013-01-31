/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

// Componentes
#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false)
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo) 
	{
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();

		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));
			Math::yaw(yaw,_transform);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
		

		// Inicializamos los componentes
		TComponentMap::const_iterator it;

		bool correct = true;

		for( it = _components.begin(); it != _components.end() && correct; ++it )
			correct = it->second->spawn(this,map,entityInfo) && correct;

		return correct;

	} // spawn

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		// Si somos jugador, se lo decimos al servidor
		// y nos registramos para que nos informen
		// de los movimientos que debemos realizar

		std::cout << "activando ... " << getName() << " que es ... " << isPlayer() << std::endl;

		if (isPlayer())
		{
			std::cout << "seteando como player ... " << std::endl;
			CServer::getSingletonPtr()->setPlayer(this);
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
		}

		// Activamos los componentes
		TComponentMap::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		for( it = _components.begin(); it != _components.end(); ++it )
			_activated = it->second->activate() && _activated;


		return _activated;

	} // activate

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si éramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
		{
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
			CServer::getSingletonPtr()->setPlayer(0);
		}


		TComponentMap::const_iterator it;

		// Desactivamos los componentes
		for( it = _components.begin(); it != _components.end(); ++it )
			it->second->deactivate();

		_activated = false;

	} // deactivate

	//---------------------------------------------------------

	void CEntity::tick(unsigned int msecs) 
	{
		TComponentMap::const_iterator it;

		for( it = _components.begin(); it != _components.end(); ++it )
			it->second->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component, const std::string& id)
	{
		_components[id] = component;
		component->setEntity(this);

	} // addComponent

	//---------------------------------------------------------

	bool CEntity::removeComponent(const std::string& id)
	{
		return _components.erase(id) == 1? true : false;

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{
		_components.clear();

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(CMessage *message, IComponent* emitter)
	{
		
		// Interceptamos los mensajes que además de al resto de los
		// componentes, interesan a la propia entidad.
		switch(message->getMessageType())
		{
		case Message::SET_TRANSFORM:
			_transform = ((CMessageTransform*)message)->getTransform();
		}
		

		TComponentMap::const_iterator it;
		//Por si nadie quiso el mensaje
		message->addSmartP();
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != it->second )
				anyReceiver = it->second->set(message) || anyReceiver;
		}
		//Por si nadie quiso el mensaje
		message->subSmartP();

		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position) 
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);
		
	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);

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

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);

	} // setYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);
	} // yaw

	//---------------------------------------------------------

	void CEntity::setPitch(float pitch) 
	{
		Math::setYaw(pitch,_transform);

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);
	} // setPitch

	//---------------------------------------------------------

	void CEntity::pitch(float pitch) 
	{
		Math::pitch(pitch,_transform);

		// Avisamos a los componentes del cambio.
		Logic::CMessageTransform *m=new Logic::CMessageTransform(Message::SET_TRANSFORM);
		m->setTransform(_transform);
		emitMessage(m);
	} // pitch

	//---------------------------------------------------------

	


} // namespace Logic
