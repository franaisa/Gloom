//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author Antonio Jesús Narvaez Corrales
@author Rubén Mulero Guerrero
@date February, 2010
*/

#include "Particle.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "HHFX.h"
#include "Server.h"
#include "StaticEntity.h"
#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"
#include "Graphics/Entity.h"

#include <assert.h>
#include <map>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>
#include <OgreSceneNode.h>
#include <OgreParticleSystem.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>
#include <OgreCompositorChain.h>


#include <OgreCompositorManager.h>
#include <OgreMaterialManager.h>

#include "CompositorListener.h"
#include "PoolParticle.h"


namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), 
		_staticGeometry(0), _directionalLight(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_name = name;
		_sceneMgr->getRootSceneNode()->setVisible(false);
		_compositorManager = Ogre::CompositorManager::getSingletonPtr();
		_poolParticle = new CPoolParticle();

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);
		
		
		Ogre::MaterialManager::getSingletonPtr()->destroyAllResourcePools();
		Ogre::MaterialManager::getSingletonPtr()->removeAll();
		_compositorManager->destroyAllResourcePools();
		_compositorManager->removeAll();
	} // ~CScene

	//--------------------------------------------------------

	bool CScene::addEntity(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	//--------------------------------------------------------

	bool CScene::addStaticEntity(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::removeEntity(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);

	} // addEntity

	//--------------------------------------------------------

	void CScene::removeStaticEntity(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry();
		// HACK en pruebas
		try
		{
			_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
			->addViewport(_camera->getOgreCamera());
		}
		catch(std::exception e)
		{
			_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->getViewport(0);
			_viewport->setCamera(_camera->getOgreCamera());
		}
		
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);
		_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		/*
		_sceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));
		/*/
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.3f,0.3f,0.3f));
		/* */
		
		if(_name != "dummy_scene"){
			_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "Glow");
			_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Glow", true);

			_glowMaterialListener = new GlowMaterialListener();
			Ogre::MaterialManager::getSingletonPtr()->addListener(_glowMaterialListener);

			_poolParticle->activate();

			_directionalLight = _sceneMgr->createLight("HackLight");
			_directionalLight->setPosition(Vector3(50,50,50));
			_directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
			_directionalLight->setDirection( Vector3::ZERO - Vector3(50,50,50) );
			//_directionalLight->setAttenuation( 100.0f, 1.0f, 1.0f, 1.0f );
			_directionalLight->setCastShadows(true);
			_directionalLight->setDiffuseColour(1,1,1);
			_directionalLight->setSpecularColour(1,1,1);

			//HHFX::getSingletonPtr()->setSceneMgr(_sceneMgr);
			//HHFX::getSingletonPtr()->setCamera(_camera->getOgreCamera());
			//HHFX::getSingletonPtr()->activate();
		}
		_sceneMgr->getRootSceneNode()->setVisible(true);
	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		/*if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}*/

		if(_name != "dummy_scene"){
			
			//HHFX::getSingletonPtr()->deactivate();
		}

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::tick(float secs)
	{	
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);
		_poolParticle->tick(secs);
		
	} // tick

	//--------------------------------------------------------

	void CScene::changeAmbientLight(Vector3 Light){_sceneMgr->setAmbientLight(Ogre::ColourValue(Light.x,Light.y,Light.z));};


	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntityList::const_iterator it = _staticEntities.begin();
			TStaticEntityList::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::createSceneNode(const std::string &nameSceneNode){

		Ogre::SceneNode *sceneNode = getSceneNode(nameSceneNode);
		if(!sceneNode)
			sceneNode = _sceneMgr->createSceneNode(nameSceneNode);
		std::pair<std::string,Ogre::SceneNode*> aux(nameSceneNode,sceneNode);
		_sceneNodes.insert(aux);
		
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Ogre::SceneNode* CScene::getSceneNode(const std::string &nameSceneNode){
		
		std::map<std::string, Ogre::SceneNode*>::iterator aux = _sceneNodes.find(nameSceneNode); 
		//Si cierto, significa que no existe dicha SceneNode
		if (aux == _sceneNodes.end())
			return 0;
		return aux->second;

	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
	bool CScene::addEntityToSceneNode(std::string &nameSceneNode, CEntity* entity){
		Ogre::SceneNode *sceneNode = _sceneNodes.find(nameSceneNode)->second;
		sceneNode->attachObject((Ogre::MovableObject*)entity->getEntity());
		return true;
	}

	void CScene::removeEntityToSceneNode(std::string &nameSceneNode, CEntity* entity){
		Ogre::SceneNode *sceneNode = _sceneNodes.find(nameSceneNode)->second;
		sceneNode->removeChild(entity->getName());
	}
*/

	CParticle * CScene::createParticle(const std::string &particleName, const Vector3 &position){
	
		return createParticle(particleName, position,Vector3::ZERO);
	
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CParticle * CScene::createParticle(const std::string &particleName, const Vector3 &position, const Vector3 &directionWithForce){

		CParticle *particle  = _poolParticle->getParticle(particleName);
		//CParticle *particle = new CParticle(particleName);
		if(!particle)
			return 0;

		particle->setPosition(position);
		
		if(!directionWithForce.isZeroLength())
			particle->setDirection(directionWithForce);

		return particle;

	} // createParticle

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::changeVisibilityParticle(const std::string nameParticle, bool visibility){
		_poolParticle->setVisible(nameParticle, visibility);

	} // changeVisibilityParticle
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::createCompositor(const std::string &name){

		if(_compositorList.find(name)!=_compositorList.end())
			return;

		CCompositorListener* newListener = new CCompositorListener();
		_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), name)->addListener(newListener);
		std::pair<std::string,CCompositorListener*> aux(name,newListener);

		//insert in the list for later access
		_compositorList.insert(aux);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::setCompositorVisible(const std::string &name, bool visible){
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), name, visible);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::destroyCompositor(const std::string &name){
		_compositorManager->removeCompositor(_camera->getOgreCamera()->getViewport(), name);
		//remove from our compositorList and delete listener
		CCompositorListener* listenerToDelete = _compositorList[name];
		_compositorList.erase(name);
		delete listenerToDelete;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::updateCompositorVariable(const std::string &name, const std::string &variable, const float &value){

		//if not created, we do nothing
		if(_compositorList.find(name)==_compositorList.end())
			return;

		_compositorList[name]->inputCompositor(variable, value);

	}

	bool CScene::getCompositorVisible(const std::string &name){
		return _compositorManager->getCompositorChain(_camera->getOgreCamera()->getViewport())->getCompositor(name)->getEnabled();
	}

} // namespace Graphics
