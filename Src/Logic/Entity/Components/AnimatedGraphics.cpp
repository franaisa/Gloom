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

#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Messages/MessageSetAnimation.h"
#include "Logic/Messages/MessageStopAnimation.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageChangeMaterial.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);

	CAnimatedGraphics::~CAnimatedGraphics(){
		delete _weapons;
	}
	
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
			_animatedGraphicsEntity->setObserver(this);
		}

		//cargamos los modelos de las armas para poder ponerselas en la mano conforme los jugadores cambien de arma

		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
		_weapons = new  Graphics::CEntity*[numWeapons];	
			//_weapons[numWeapons];

			// Por ahora leo a mano cada una de las armas que tiene el usuario

			std::string armas[] = {"Hammer","Sniper","ShotGun","MiniGun", "GrenadeLauncher", "RocketLauncher"};

			
			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << armas[i];
				std::string weapon = aux.str();
				
				//creamos la entidad gráfica del arma para poder atacharla al monigote
				_weapons[i] = new Graphics::CEntity(weapon,entityInfo->getStringAttribute(weapon+"Model")); 
				
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

		//Habria que quitare el string que se pasa por parametro porque no tiene sentido
		animationFinished("random");
		_animatedGraphicsEntity->attachWeapon(*_weapons[0], _entity->getEntityID());
	}
	//---------------------------------------------------------

	void CAnimatedGraphics::deactivate()
	{
		CGraphics::deactivate();

		//En verdad hay que dejar la animación de morir porque desactivaremos al morir
		//_animatedGraphicsEntity->stopAllAnimations();
		
	}
	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(CMessage *message)
	{
		return CGraphics::accept(message) ||
			message->getMessageType() == Message::SET_ANIMATION				||
			message->getMessageType() == Message::STOP_ANIMATION			||
			message->getMessageType() == Message::CHANGE_WEAPON_GRAPHICS	|| 
			message->getMessageType() == Message::PLAYER_DEAD				||
			message->getMessageType() == Message::DAMAGED					||
			message->getMessageType() == Message::HUD_SPAWN;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(CMessage *message)
	{

		switch(message->getMessageType())
		{
		case Message::SET_TRANSFORM: {
			Matrix4 transform = static_cast<CMessageTransform*>(message)->getTransform();
			Math::setYaw(Math::getYaw(transform), transform);

			_graphicsEntity->setTransform( transform );
			break;
		}
		case Message::ACTIVATE: {
			setVisible(true);
			break;
		}
		case Message::DEACTIVATE: {
			setVisible(false);
			break;
		}
		case Message::CHANGE_MATERIAL: {
			changeMaterial( static_cast<CMessageChangeMaterial*>(message)->getMaterialName() );
			break;
		}

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
		case Message::CHANGE_WEAPON_GRAPHICS:
			changeWeapon( ((CMessageChangeWeaponGraphics*)message)->getWeapon() );
			break;
		case Message::PLAYER_DEAD:
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation("Death",false);
			break;
		case Message::DAMAGED:
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation("Damage",false);
			break;
		//Por si en redes se utilizara para algo hay que cambiarlo porque es un nonsense
		/*case Message::HUD_SPAWN:
			_animatedGraphicsEntity->stopAllAnimations();
			_animatedGraphicsEntity->setAnimation("Idle",true);
			break;*/
		}

	} // process
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// Al acabar una animación ponemos la de por defecto en loop
		_animatedGraphicsEntity->stopAllAnimations();
		_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
	}


	void CAnimatedGraphics::changeWeapon(int newWeapon){
		_animatedGraphicsEntity->attachWeapon(*_weapons[newWeapon], _entity->getEntityID());
	}

} // namespace Logic
