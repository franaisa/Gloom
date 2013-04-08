//---------------------------------------------------------------------------
// GlowingEntity.h
//---------------------------------------------------------------------------

/**
@file GlowingEntity.h

@see Graphics::CGlowingEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_GlowingEntity_H
#define __Graphics_GlowingEntity_H

#include "BaseSubsystems/Math.h"
#include "Entity.h"

#include <OgreRenderQueueListener.h>
#include <OgreRoot.h>

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Ogre {
	class Entity;
}

namespace Graphics {
	/**
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CGlowingEntity : public CEntity {
	public:

		class StencilOpQueueListener;

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CGlowingEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CGlowingEntity();

	protected:

		virtual bool load();

		StencilOpQueueListener* _stencilOpFrameListener;

		void createFrameListener();

	}; // class CEntity

	// render queues
	#define RENDER_QUEUE_OUTLINE_GLOW_OBJECTS	Ogre::RenderQueueGroupID::RENDER_QUEUE_MAIN + 1
	#define RENDER_QUEUE_OUTLINE_GLOW_GLOWS		Ogre::RenderQueueGroupID::RENDER_QUEUE_MAIN + 2
	#define RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW	Ogre::RenderQueueGroupID::RENDER_QUEUE_MAIN + 3
	#define RENDER_QUEUE_FULL_GLOW_GLOW			Ogre::RenderQueueGroupID::RENDER_QUEUE_MAIN + 4
	#define LAST_STENCIL_OP_RENDER_QUEUE		RENDER_QUEUE_FULL_GLOW_GLOW

	// stencil values
	#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
	#define STENCIL_VALUE_FOR_FULL_GLOW 2
	#define STENCIL_FULL_MASK 0xFFFFFFFF

	class CGlowingEntity::StencilOpQueueListener : public Ogre::RenderQueueListener { 
	public: 
		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation) { 
			if (queueGroupId == RENDER_QUEUE_OUTLINE_GLOW_OBJECTS) { // outline glow object 
				Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 

				rendersys->clearFrameBuffer(Ogre::FBT_STENCIL); 
				rendersys->setStencilCheckEnabled(true); 
				rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
					STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, 
					Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
			} 
			if (queueGroupId == RENDER_QUEUE_OUTLINE_GLOW_GLOWS) { // outline glow
				Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
				rendersys->setStencilCheckEnabled(true); 
				rendersys->setStencilBufferParams(Ogre::CMPF_NOT_EQUAL,
					STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, 
					Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
			} 
			if (queueGroupId == RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW) { // full glow - alpha glow
				Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
				rendersys->setStencilCheckEnabled(true); 
				rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,
					STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK, 
					Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       
			} 

			if (queueGroupId == RENDER_QUEUE_FULL_GLOW_GLOW) { // full glow - glow
				Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
				rendersys->setStencilCheckEnabled(true); 
				rendersys->setStencilBufferParams(Ogre::CMPF_EQUAL,
					STENCIL_VALUE_FOR_FULL_GLOW,STENCIL_FULL_MASK, 
					Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_ZERO,false);       
			}
		} 

		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation) { 
			if (queueGroupId == LAST_STENCIL_OP_RENDER_QUEUE) {
				Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
				rendersys->setStencilCheckEnabled(false); 
				rendersys->setStencilBufferParams(); 
			}
		}
	}; 

} // namespace Graphics

#endif // __Graphics_Entity_H
