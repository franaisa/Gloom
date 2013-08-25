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
		_text2d = new Graphics::CMovable2dText();
		// Debido a que el pivote del billboard tambien se modifica si colgamos la entidad
		// de una entidad grafica, creamos un nodo nuevo y actualizamos su posicion en el tick
		_text2d->load( _entity->getMap()->getScene(), NULL, _offset, _entity->getName(), _font, Vector4(_color.x, _color.y, _color.z, 1.0f), _size );
		
		// @todo
		// Comprobamos si tenemos equipo, si no lo tenemos, directamente no mostramos el nombre
		// porque todos somos enemigos. Si estamos en el mismo equipo del player, mostramos nuestro
		// nombre con el color que corresponda, y si no lo estamos configuramos el color pero
		// no mostramos el nombre
		_text2d->setVisible(false);
	}

	//________________________________________________________________________

	void CCharacterName::onTick(unsigned int msecs) {
		if( _text2d->isVisible() )
			_text2d->setPosition( _entity->getPosition() + _offset );
		else
			putToSleep();
	}

} // namespace Logic
