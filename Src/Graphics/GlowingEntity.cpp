//---------------------------------------------------------------------------
// GlowingEntity.cpp
//---------------------------------------------------------------------------

/**
@file GlowingEntity.cpp

@author David Llansó
@date Julio, 2010
*/

#include "GlowingEntity.h"
#include "Scene.h"
#include <OgreEntity.h>

namespace Graphics {
	CGlowingEntity::CGlowingEntity(const std::string &name, const std::string &mesh) : CEntity(name, mesh) {
		
	} // CGlowingEntity

	//--------------------------------------------------------

	CGlowingEntity::~CGlowingEntity() {
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
		delete _stencilOpFrameListener;
	} // ~CGlowingEntity

	//--------------------------------------------------------

	bool CGlowingEntity::load() {
		if( !CEntity::load() ) return false;

		createFrameListener();

		// outline glow entity
		_entity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_OBJECTS);

		// outline glow entity actual glow
		Ogre::Entity* actualOutlineGlowEntity = _entity->clone(_entity->getName() + "_glow"); 
		actualOutlineGlowEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_GLOWS); 
		actualOutlineGlowEntity->setMaterialName("cg/glow"); 
		Ogre::SceneNode* actualOutlineGlowNode = _entity->getParentSceneNode()->createChildSceneNode(); 
		actualOutlineGlowNode->attachObject(actualOutlineGlowEntity); 

		return true;
	}

	//--------------------------------------------------------

	void CGlowingEntity::createFrameListener() {
		_stencilOpFrameListener = new StencilOpQueueListener();
		_scene->getSceneMgr()->addRenderQueueListener(_stencilOpFrameListener);
	}

} // namespace Graphics
