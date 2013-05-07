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


#include <OgreCompositorManager.h>
#include <OgreMaterialManager.h>
#include "CompositorListener.h"


namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), 
		_staticGeometry(0), _directionalLight(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_name = name;

		_compositorManager = Ogre::CompositorManager::getSingletonPtr();

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
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
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);
		_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		/*
		_sceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));
		/*/
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.7f,0.7f,0.7f));
		/* */
		
		_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "Glow");
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Glow", true);

		/*_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "Motion Blur");
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Motion Blur", true);*/

		/*_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "Old TV");
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Old TV", true);

		_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "Old Movie");
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Old Movie", true);
		_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), "berserkCompositor");
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "berserkCompositor", true);
		*/
		_glowMaterialListener = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(_glowMaterialListener);


	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::tick(float secs)
	{	
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);
		
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

	CParticle * CScene::createParticle(const std::string &unicName, const std::string &particleName, const Vector3 &position){
	
		return createParticle(unicName,particleName, position,Vector3::ZERO);
	
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CParticle * CScene::createParticle(const std::string &unicName, const std::string &particleName, const Vector3 &position, const Vector3 &directionWithForce){

		CParticle *particle = new CParticle(unicName, particleName);

		particle->setPosition(position);

		
		if(!directionWithForce.isZeroLength())
			//particle->setDirection(directionWithForce);

		return particle;

	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::createCompositor(std::string &name){

		if(_compositorList.find(name)!=_compositorList.end())
			return;

		CCompositorListener* newListener = new CCompositorListener();
		_compositorManager->addCompositor(_camera->getOgreCamera()->getViewport(), name)->addListener(newListener);
		std::pair<std::string,CCompositorListener*> aux(name,newListener);

		//insert in the list for later access
		_compositorList.insert(aux);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::setCompositorVisible(std::string &name, bool visible){
		_compositorManager->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), name, visible);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::destroyCompositor(std::string &name){
		_compositorManager->removeCompositor(_camera->getOgreCamera()->getViewport(), name);
		//remove from our compositorList and delete listener
		CCompositorListener* listenerToDelete = _compositorList[name];
		_compositorList.erase(name);
		delete listenerToDelete;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScene::updateCompositorVariable(std::string name, std::string variable, float value){

		//if not created, we do nothing
		if(_compositorList.find(name)==_compositorList.end())
			return;

		_compositorList[name]->inputCompositor(variable, value);

	}

} // namespace Graphics
