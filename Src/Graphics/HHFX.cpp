/**
@file HHFX.cpp

Contains the HHFX wrapper implementation who allow to create
HHFX particles into Gloom

@see Graphics::HHFX

@author Rubén Mulero Guerrero
@date May, 2013
*/

#include "HHFX.h"
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include "BaseSubsystems/Server.h"


using namespace Ogre;

namespace Graphics{

	//HHFX singleton instance
	HHFX *HHFX::_instance = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////    CONSTRUCTOR/DESTRUCTOR    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	HHFX::HHFX():
		_hhfxScene(NULL)
	{
		_instance = this;
	}

	//-------------------------------------------------------------------------------------

	HHFX::~HHFX()
	{
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    INIT    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		new HHFX();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////    RELEASE    //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    OPEN    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::open()
	{

		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();


		if (_root == NULL)
		return false;

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		

		return true;

	} // open

	void HHFX::activate(){

		// Load resources
		loadResources();

		//create the frame listener, this allow us to render correctly the hhfx scene
		createFrameListener();

	}

	void HHFX::deactivate(){
		clearFFFXScene();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////    createFrameListener    /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::createFrameListener(void)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** SETTING UP THE HHFX FRAMELISTENER ***");
		_root->addFrameListener(this);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////    CLOSE    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::close()
	{
		clearFFFXScene();
		return true;
	} // open

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////    _IntersectGrid    ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::_IntersectGrid(Vector3& intersectPos)
	{
		return false;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////    _IntersectScene    //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	// simplistic collision with our ground-plane (200 units-wide)

	bool	HHFX::_IntersectScene(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport)
	{
		HHFX	*self = (HHFX*)arg;
		Ray	traceRay(start, direction);
		return false;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    loadResources    ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::loadResources()
	{
		/////////////////////////////////////////////////////////////////////////////////////////
		// preload the distortion texture with hardware gamma correction
		/////////////////////////////////////////////////////////////////////////////////////////

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] ---------- preload texture hardware gamma ----------");

		// ok then, load all the textures in the HellHeaven resource group
		String resourceGroup("HellHeaven");

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup);

		StringVectorPtr strVectorPtr = ResourceGroupManager::getSingleton().listResourceNames(resourceGroup);

		for (unsigned int i = 0; i < strVectorPtr->size(); ++i)
		{
			String texName = strVectorPtr->at(i);

			try
			{
				LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] Trying to load " + texName + " as texture...");

				// should load all the textures with gamma correction 
				// except the ones used for distortion post effect
				if (!texName.compare("ParticleDeformBlur_01.dds") ||
					!texName.compare("RainDeform_01.dds") ||
					!texName.compare("RainDropsDeform_01.dds"))
				{
					TexturePtr pTex = TextureManager::getSingleton().load(
																			texName, resourceGroup,
																			TEX_TYPE_2D, MIP_DEFAULT,
																			1.0f, false, PF_UNKNOWN, false);
				}
				else
				{
					// gamma corrected
					TexturePtr pTex = TextureManager::getSingleton().load(
																			texName, resourceGroup,
																			TEX_TYPE_2D, MIP_DEFAULT,
																			1.0f, false, PF_A8R8G8B8, true);
				}
			}
			catch (Ogre::Exception e) // texture not loaded because it surely is not one
			{
				continue;
			}
		}

		LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] ---------- done ----------");

		//////////////////////////////////////////////////////////////////////////////////////////
		//										hellheaven										//
		//////////////////////////////////////////////////////////////////////////////////////////

		// retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated.
		MovableObject	*fx = _sceneMgr->createMovableObject("HHFX");

		if (fx != NULL)
		{
			OgreAssert(dynamic_cast<IHHFXOgre*>(fx) != NULL, "object should be of type \"HHFX\"");
			IHHFXOgre	*hhFx = static_cast<IHHFXOgre*>(fx);
			_hhfxScene = &hhFx->GetHHFXScene();

			// we got the hh scene, destroy the effect
			_sceneMgr->destroyMovableObject(fx);
		}
		OgreAssert(_hhfxScene != NULL, "failed creating HHFx Scene !");

		// load a pack
		_HHFX_path = "media/packs/SampleParticlesOgre";

		bool hhfxPackLoaded = _hhfxScene->GetHHFXBase().LoadPack(_HHFX_path.c_str(), true);
		OgreAssert(hhfxPackLoaded, "hhfx pack did not load correctly or contains no effects !");

		// bind the collision callback
		_hhfxScene->SetCollisionCallback(this, &_IntersectScene);

		// get all the effects from this pack and add them to the hash table, where we can store
		// all the needed information to create them efficiently
		const std::vector<std::string>& fxNames = _hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetNames();
		for (unsigned int i = 0; i < fxNames.size(); ++i){
			NameValuePairList params;
			params["pack"] = "media/packs/SampleParticlesOgre";
			params["fx"] = _hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetEffects()[i];
			params["run"] = "yes";
			HHFXParticle particle(fxNames[i],params);
			_particles.insert(particle);
			LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] REGISTERING "+fxNames[i]);
		}

		LogManager::getSingleton().logMessage(LML_NORMAL, "[HHFX] ---------- LOAD FINISHED ----------");
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    clearFFFXScene    //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::clearFFFXScene()
	{
		// clear the scene before shutting down ogre since the hhfx ogre implementation holds some Ogre objects.
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////    frameStarted    ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::frameStarted(const Ogre::FrameEvent& evt)
	{
		// update the hhfx scene
		_hhfxScene->Update(evt.timeSinceLastFrame);
		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////     _AnimateLastSpawnedNode    ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::_AnimateLastSpawnedNode(float elapsedTime)
	{
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////     CREATEPARTICLE    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	IHHFXOgre* HHFX::createParticle(const std::string &particle){

		auto it = _particles.find(particle);

		if(it==_particles.end())
			return NULL;

		NameValuePairList params = it->second;
		

		MovableObject	*fx = _sceneMgr->createMovableObject("HHFX", &params);
		if (fx != NULL)
		{
			IHHFXOgre	*hhfx = static_cast<IHHFXOgre*>(fx);
			// Here, check if the effect is still active after we've run it.
			// It might not be active anymore for fire-and-forget effects, such as an 
			// instantaneous explosion.
			// For such effects, don't bother creating a parent scene node.
			if (hhfx->IsFXActive())
			{
				//set this class to listen to the fx, to be notified when its created and 
				//destroyed in OnFXStarted and OnFXStopped
				hhfx->SetFXListener(this);
				return hhfx;
			}
		}
		return NULL;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////     frameRenderingQueued    ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	bool HHFX::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		// setting render params for the particles renderers
		HellHeaven::CRendererSubView view;

		Matrix4 worldTransforms;

		const Vector3& camPos = _camera->getPosition();
		const Quaternion& camOri = _camera->getOrientation();

		worldTransforms.makeTransform(camPos, Vector3::UNIT_SCALE, camOri);
		worldTransforms = worldTransforms.transpose();

		view.setHHFXScene(*_hhfxScene);
		view.setViewMatrix(worldTransforms);
		view.setSceneManager(*_sceneMgr);

		_hhfxScene->Render(view, camPos);

		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////     OnFXStarted    ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	void HHFX::OnFXStarted(IHHFX* obj)
	{
		// create a light under the ElectricOrb effect
		if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
		{
			IHHFXOgre* fx = static_cast<IHHFXOgre*>(obj);
			const Vector3& fxPosition = fx->getParentSceneNode()->getPosition();
			Light* pointLight = _sceneMgr->createLight("pointLight" + StringConverter::toString((unsigned int)(obj)));
			pointLight->setType(Light::LT_POINT);
			pointLight->setPosition(fxPosition + Vector3::UNIT_Y * 0.8f);
			pointLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
			pointLight->setSpecularColour(0.8f, 0.8f, 1.0f);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////    OnFXStopped    ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

	// called when an effect stopped by itself or when the hhfx scene is cleared
	void HHFX::OnFXStopped(IHHFX* obj)
	{
		// destroy the light created under ElectricOrb
		if (strstr(obj->GetPath(), "ElectricOrb.hfx") != NULL)
			_sceneMgr->destroyLight("pointLight" + StringConverter::toString((unsigned int)(obj)));

		// destroy the created node's effect parent and the effect
		IHHFXOgre	*fx = static_cast<IHHFXOgre*>(obj);
		SceneNode	*parentNode = fx->getParentSceneNode();

		if (parentNode != NULL)
			_sceneMgr->destroySceneNode(parentNode);
		_sceneMgr->destroyMovableObject(fx);
	}

	//-------------------------------------------------------------------------------------
}