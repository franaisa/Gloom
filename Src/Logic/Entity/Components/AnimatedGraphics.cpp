/**
@file AnimatedGraphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de una entidad estática.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AnimatedGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;

		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
			_animatedGraphicsEntity->setObserver(this);
		}

		//cargamos los modelos de las armas para poder ponerselas en la mano conforme los jugadores cambien de arma

		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
			
			//_weapons[numWeapons];

			// Por ahora leo a mano cada una de las armas que tiene el usuario

			std::string armas[] = {"Hammer","MiniGun","ShotGun"};

			
			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << armas[i];
				std::string weapon = aux.str();
				
				//creamos la entidad gráfica del arma para poder atacharla al monigote
				_weapons = new Graphics::CEntity(weapon,entityInfo->getStringAttribute(weapon+"Model")); 
				
			}
			if(!_weapons)
				return NULL;
		}
		
		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	void CAnimatedGraphics::activate()
	{
		CGraphics::activate();
		_animatedGraphicsEntity->attachWeapon(*_weapons);
	}


	bool CAnimatedGraphics::accept(CMessage *message)
	{
		return CGraphics::accept(message) ||
			message->getMessageType() == Message::SET_ANIMATION ||
			message->getMessageType() == Message::STOP_ANIMATION;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(CMessage *message)
	{
		
		CGraphics::process(message);

		switch(message->getMessageType())
		{
		case Message::SET_ANIMATION:
			// Paramos todas las animaciones antes de poner una nueva.
			// Un control más sofisticado debería permitir interpolación
			// de animaciones. Galeon no lo plantea.
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation(((CMessageSetAnimation*)message)->getString(),((CMessageSetAnimation*)message)->getBool());
			break;
		case Message::STOP_ANIMATION:
			_animatedGraphicsEntity->stopAnimation(((CMessageStopAnimation*)message)->getString());
			break;
		}

	} // process
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// Si acaba una animación y tenemos una por defecto la ponemos
		_animatedGraphicsEntity->stopAllAnimations();
		_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
	}

} // namespace Logic

