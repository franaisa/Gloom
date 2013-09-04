/**
@file CharacterName.cpp


@see Logic::CCharacterName
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "CharacterName.h"
#include "Graphics.h"
#include "AnimatedGraphics.h"
#include "BaseSubsystems/Euler.h"
#include "BaseSubsystems/Math.h"

#include "Graphics/Movable2dText.h"

#include "Application/BaseApplication.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Logic/GameNetPlayersManager.h"

#include "Input/Server.h"
#include "Input/PlayerController.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CCharacterName);

	//________________________________________________________________________

	CCharacterName::CCharacterName() : _text2d(NULL),
									   _color(1.0f, 1.0f, 1.0f),
									   _size(20.0f) {
		// Nada que hacer
	}

	//________________________________________________________________________

	CCharacterName::~CCharacterName() {
		if(_text2d != NULL) {
			delete _text2d;
		}
	}
	
	//________________________________________________________________________
	
	bool CCharacterName::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("textOffset") );
		assert( entityInfo->hasAttribute("textFont") );

		_offset = entityInfo->getVector3Attribute("textOffset");
		_font = entityInfo->getStringAttribute("textFont");

		if( entityInfo->hasAttribute("textSize") )
			_size = entityInfo->getFloatAttribute("textSize");

		if( entityInfo->hasAttribute("textColor") )
			_color = entityInfo->getVector3Attribute("textColor");

		return true;
	} // spawn

	//________________________________________________________________________

	void CCharacterName::setVisible(bool isVisible) {
		_text2d->setVisible(isVisible);

		if( isVisible && isSleeping() )
			wakeUp();
	}

	//________________________________________________________________________

	bool CCharacterName::isVisible() {
		return _text2d->isVisible();
	}

	//________________________________________________________________________

	void CCharacterName::onStart() {
		// Obtenemos la entidad que corresponde al player y comprobamos su tipo
		// Si se trata de un espectador, mostramos siempre el nombre.
		// En caso contrario, si estamos en tdm mostramos nuestro nombre si el 
		// player esta en nuestro equipo. Si el player no esta en nuestro equipo
		// playerInSight se encargara de hacer que nos mostremos encendidos
		CGameNetPlayersManager* playersMgr = CGameNetPlayersManager::getSingletonPtr();
		TeamFaction::Enum myTeam = playersMgr->getTeamUsingEntityId( _entity->getEntityID() );
		if(myTeam == TeamFaction::eRED_TEAM)
			_color = Vector3(1.0f, 0.0f, 0.0f);
		else if(myTeam == TeamFaction::eBLUE_TEAM)
			_color = Vector3(0.0f, 0.0f, 1.0f);
		 
		bool showName = false;
		CEntity* playerEntity = Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar();
		if(playerEntity != NULL) {
			if(playerEntity->getType() == "Spectator") {
				showName = true;
			}
			else {
				TeamFaction::Enum playerTeam = playersMgr->getTeamUsingEntityId( playerEntity->getEntityID() );
				
				if(myTeam == TeamFaction::eNONE || playerTeam == TeamFaction::eNONE)
					showName = false;
				else if(playerTeam == myTeam)
					showName = true;
			}
		}

		_text2d = new Graphics::CMovable2dText();
		// Debido a que el pivote del billboard tambien se modifica si colgamos la entidad
		// de una entidad grafica, creamos un nodo nuevo y actualizamos su posicion en el tick
		_text2d->load( _entity->getMap()->getScene(), NULL, _offset, _entity->getName(), _font, Vector4(_color.x, _color.y, _color.z, 1.0f), _size );
		// En funcion del equipo del player dibujamos el nombre o no
		_text2d->setVisible(showName);
	}

	//________________________________________________________________________

	void CCharacterName::onTick(unsigned int msecs) {
		if( _text2d->isVisible() )
			_text2d->setPosition( _entity->getPosition() + _offset );
		else
			putToSleep();
	}

} // namespace Logic
