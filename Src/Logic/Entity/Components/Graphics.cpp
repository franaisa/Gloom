/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/GlowingEntity.h"
#include "Graphics/StaticEntity.h"

#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageSleep.h"
#include "Logic/Messages/MessageActivate.h"
#include "Logic/Messages/MessageWakeUp.h"
#include "Logic/Messages/MessageChangeMaterial.h"

#include <OgreSceneManager.h>
#include "OgreEntity.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			_scene->removeEntity(_graphicsEntity);
			_graphicsEntity = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();

		if(entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");

		if(entityInfo->hasAttribute("materialName"))
			_materialName = entityInfo->getStringAttribute("materialName");

		_graphicsEntity = createGraphicsEntity(entityInfo);
		if(!_graphicsEntity)
			return false;

		if(entityInfo->hasAttribute("scale")){

			//con el escale puedo escalar lo que quiera cuanto quiera. El mapa ya sale al tamaño que queramos ^^
			_graphicsEntity->setScale(entityInfo->getVector3Attribute("scale"));
			// con esto puedo girar lo que quiera cuanto quiera. Lo he probado con el mapa
		}

		/*
		//// Esto tb es una guarrada, habra q hacer un componente de luces para controlarlas todas y cargarlas.
		if(_entity->getName() == "World")
		{
			Ogre::Light *luz = _scene->getSceneMgr()->createLight("Luz Puntual");
			luz->setPosition(entity->getPosition() + Vector3(0,0,0));
			luz->setType(Ogre::Light::LT_POINT);
			luz->setDiffuseColour(1.0f,1.0f,0.0f);
			luz->setPowerScale(Ogre::Real(550));
		}
		*/
		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo) {
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		if(isStatic) {
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else {
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}

		
		_graphicsEntity->setTransform(_entity->getTransform());
		
		return _graphicsEntity;

	} // createGraphicsEntity

	//---------------------------------------------------------
	
	void CGraphics::setTransform(const Matrix4& transform) {
		_graphicsEntity->setTransform(transform);
	}

	//---------------------------------------------------------

	bool CGraphics::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return //msgType == Message::SET_TRANSFORM    ||
			   msgType == Message::ACTIVATE			|| 
			   msgType == Message::CHANGE_MATERIAL;
	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::SET_TRANSFORM: {
				std::shared_ptr<CMessageTransform> transformMsg = std::static_pointer_cast<CMessageTransform>(message);
				_graphicsEntity->setTransform( transformMsg->getTransform() );
				break;
			}
			case Message::ACTIVATE: {
				setVisible(std::static_pointer_cast<CMessageActivate>(message)->getActivated());
				break;
			}
			case Message::CHANGE_MATERIAL: {
				std::shared_ptr<CMessageChangeMaterial> chgMatMsg = std::static_pointer_cast<CMessageChangeMaterial>(message);
				changeMaterial( chgMatMsg->getMaterialName() );
				break;
			}
		}

	} // process

	//---------------------------------------------------------

	void CGraphics::setVisible(bool renderGraphicEntity) {
		_graphicsEntity->setVisible(renderGraphicEntity);
	}

	//---------------------------------------------------------

	void CGraphics::changeMaterial(const std::string& materialName) {
		if(materialName != "original")
			_graphicsEntity->changeMaterial(materialName);
		else
			_graphicsEntity->changeMaterial(_materialName);
		
	}

	//---------------------------------------------------------

	void CGraphics::onActivate() {
		setVisible(true);
		_graphicsEntity->setTransform(_entity->getTransform());
	}//---------------------------------------------------------
	//onActivate


	void CGraphics::onTick(unsigned int msecs){
		_graphicsEntity->setTransform(_entity->getTransform());
	}//---------------------------------------------------------
	//onTick


	void CGraphics::changeScale(Vector3 newScale){
		_graphicsEntity->setScale(newScale);
	}//---------------------------------------------------------
	//changeScale

	void CGraphics::onDeactivate() {
		setVisible(false);
	}

} // namespace Logic

