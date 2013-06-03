/**
@file HHFX.h

Contains the HHFX wrapper who allow to create
HHFX particles into Gloom

@see Graphics::HHFX

@author Rubén Mulero Guerrero
@date May, 2013
*/
#ifndef __HHFX_h_
#define __HHFX_h_

#include <IHHFXPublic.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>


#pragma warning(disable : 4481)	// nonstandard extension used: override specifier 'override'
namespace Ogre{
	class SceneNode;
}
namespace HellHeaven
{
	/**
	this is the info the hhfx plugin needs to compute the particles.
	This class must be passed as a parameter to hhfxscene->Render()
	by the update method from HHFX wrapper
	*/
	class CRendererSubView
	{
	public:
		CRendererSubView()
		:	m_hhfxScene(NULL)
		,	m_viewMatrix(Ogre::Matrix4::IDENTITY)
		,	m_sceneManager(NULL)
		,	m_customMaterial("")
		,	m_usePostFX(false)
		{}
		~CRendererSubView()
		{}

		inline const Ogre::Matrix4&	viewMatrix() const		{ return m_viewMatrix; }
		inline Ogre::SceneManager&	sceneManager() const	{ assert(m_sceneManager != NULL); return *m_sceneManager; }
		inline const Ogre::String&	customMaterial() const	{ return m_customMaterial; }
		inline bool					usePostFX() const		{ return m_usePostFX; }
		inline IHHFXScene&			hhfxScene() const		{ assert(m_hhfxScene != NULL); return *m_hhfxScene; }

		inline void					setViewMatrix(const Ogre::Matrix4 &view)				{ m_viewMatrix = view; }
		inline void					setSceneManager(Ogre::SceneManager& sceneManager)		{ m_sceneManager = &sceneManager; }
		inline void					setCustomMaterial(const Ogre::String& customMaterial)	{ m_customMaterial = customMaterial; }
		inline void					setUsePostFX(bool usePostFX)							{ m_usePostFX = usePostFX; }
		inline void					setHHFXScene(IHHFXScene& HHFXScene)						{ m_hhfxScene = &HHFXScene; }

	private:
		// the scene that called the render methods
		IHHFXScene*					m_hhfxScene;
		Ogre::Matrix4				m_viewMatrix;
		Ogre::SceneManager*			m_sceneManager;
		Ogre::String				m_customMaterial;
		bool						m_usePostFX;
	};
}
namespace Graphics{
	/**
	HHFX wrapper class. This class is a singleton class that contains all the
	needed functionality to create, destroy and manage the HHFX particles.

	The particles are preloaded and dispatched when we call the createParticle
	method. If we try to create a particle who has not been preloaded, that
	particle will not be created
	*/
	class HHFX : public Ogre::FrameListener, public Ogre::WindowEventListener, public IHHFX::IFXListener
	{
	public:
		/**
		Initialize the HHFX wrapper
		*/
		static bool Init();

		/**
		Releases the HHFX wrapper
		*/
		static void Release();

		/**
		HHFX wrapper object 2nd phase initialization
		*/
		bool close();

		/**
		HHFX wrapper object 2nd phase release
		*/
		bool open();

		/**
		Called when we want to deactivate HHFX
		*/
		void HHFX::deactivate();

		/**
		Called when we want to activate HHFX
		*/
		void HHFX::activate();

		/**
		Sets the HHFX camera where the objects will be orientated. The particles are
		simply textures, so if we want to display it correctly, they need to be camera
		orientated.
		*/
		void setCamera(Ogre::Camera* camera){_camera = camera;}
		/**
		Creates a new Particle with the name provided. If we don't have a particle
		named like this, we don't create the particle

		@param particle The particle name we are creating

		@returns A IHHFXOgre object we can attach to the scene or NULL if the object
		has not been created or the effect is the type fire-and-forget effects, such
		as an instantaneous explosion.
		*/
		IHHFXOgre* createParticle(const std::string &particle);

		/**
		Sets the ogre scene manager to the wrapper

		@param sceneMgr the ogre scene manager in which we are creating the particles
		*/
		void setSceneMgr(Ogre::SceneManager* sceneMgr){_sceneMgr = sceneMgr;}

		/**
		Gets the instance of de HHFX wrapper
		*/
		static HHFX* getSingletonPtr(){return _instance;}

	protected:
		
		/**
		This method adds the wrapper to the ogre frame listener list
		*/
		virtual void createFrameListener(void);

		/**
		Cleans up the HHFX scene from particles
		*/
		virtual void clearFFFXScene(void);

		/**
		preloads all the resources needed by the particles
		*/
		virtual void loadResources(void);
		

		// ======================================================================= //
		//              INHERITED METHODS FROM OGRE::FRAMELISTENER				   //
		// ======================================================================= //
		virtual bool				frameStarted(const Ogre::FrameEvent& evt);
		virtual bool				frameRenderingQueued(const Ogre::FrameEvent& evt);

		// ======================================================================= //
		//              INHERITED METHODS FROM IHHFX::IFXListener				   //
		// ======================================================================= //
		/**
		called when an HHFX effect has been created. This method allow to
		treat a particle before it gets rendered the first time.

		@param particle The particle who has been created.
		*/
		virtual void				OnFXStarted(IHHFX *particle) override;

		/**
		Called when a particle has stopped by itself or has been destroyed 
		with hhfx->clearscene(). Usefull to delete the scenenode we attached
		at the creation or any effect we created for a concrete
		particle

		@param particle The particle who has been stopped.
		*/
		virtual void				OnFXStopped(IHHFX *particle) override;
		

	private:

		// ======================================================================= //
		//						CONSTRUCTOR AND DESTRUCTOR						   //
		// ======================================================================= //
		HHFX();
		virtual	~HHFX();

		// ======================================================================= //
		//						  INTERNAL HELPER METHODS						   //
		// ======================================================================= //
		/**
		Physic methods where we can do special effects and treatments
		*/
		bool						_IntersectGrid(Ogre::Vector3& intersectPos);
		static bool					_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);
		void						_AnimateLastSpawnedNode(float elapsedTime);

		// HellHeaven's elements
		IHHFXScene*					_hhfxScene;

		/**
		ogre root
		*/
		Ogre::Root *_root;

		/**
		actual scene manager
		*/
		Ogre::SceneManager* _sceneMgr;

		/**
		path to hhfx resources
		*/
		std::string _HHFX_path;

		/**
		rendering camera
		*/
		Ogre::Camera* _camera;

		/**
		Singleton instance
		*/
		static HHFX* _instance;

		/**
		Hash map where the initialization of the particles are stored
		*/
		std::map<std::string, Ogre::NameValuePairList> _particles;
		typedef std::pair<std::string, Ogre::NameValuePairList>HHFXParticle;
	};
}

#endif