/**
@file GlowingGraphics.cpp

@see Logic::CGlowingGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "GlowingGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/GlowingEntity.h"

#include <OgreSceneManager.h>
#include "OgreEntity.h"

namespace Logic {
	
	IMP_FACTORY(CGlowingGraphics);

	//---------------------------------------------------------
	
	CGlowingGraphics::CGlowingGraphics() : _glowingGraphicsEntity(NULL) {
		// Nada que hacer
	}

	//---------------------------------------------------------

	CGlowingGraphics::~CGlowingGraphics() {
		if(_glowingGraphicsEntity)
		{
			_scene->removeEntity( _glowingGraphicsEntity );
			_glowingGraphicsEntity = NULL;
		}

	} // ~CGlowingGraphics
	
	//---------------------------------------------------------

	Graphics::CEntity* CGlowingGraphics::createGraphicsEntity(const Map::CEntity *entityInfo) {

		CAnimatedGraphics::createGraphicsEntity(entityInfo);
		bool isStatic = false;
		if(entityInfo->hasAttribute("static"))
			isStatic = entityInfo->getBoolAttribute("static");

		if(isStatic) {
			//return CAnimatedGraphics::createGraphicsEntity(entityInfo);
		}
		else {
			_glowingGraphicsEntity = new Graphics::CGlowingEntity(_entity->getName(),_model);
			if( !_scene->addEntity(_glowingGraphicsEntity) )
				return NULL;
		}

		_glowingGraphicsEntity->setTransform( _entity->getTransform() );
		
		return _glowingGraphicsEntity;

	} // createGraphicsEntity

} // namespace Logic

