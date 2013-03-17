//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
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

#include <OgreCompositorManager.h>
#include <OgreMaterialManager.h>


namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), 
		_staticGeometry(0), _directionalLight(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_name = name;

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);

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

		//_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		/*
		_sceneMgr->setAmbientLight(Ogre::ColourValue(1,1,1));
		/*/
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.4,0.4,0.4));
		/* */
		
		
		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_camera->getOgreCamera()->getViewport(), "Glow");

		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_camera->getOgreCamera()->getViewport(), "Glow", true);
		
		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

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

	void CScene::createSceneNode(const std::string &nameSceneNode){

		Ogre::SceneNode *sceneNode = getSceneNode(nameSceneNode);
		if(!sceneNode)
			sceneNode = _sceneMgr->createSceneNode(nameSceneNode);
		std::pair<std::string,Ogre::SceneNode*> aux(nameSceneNode,sceneNode);
		_sceneNodes.insert(aux);
		
	}

	Ogre::SceneNode* CScene::getSceneNode(const std::string &nameSceneNode){
		
		std::map<std::string, Ogre::SceneNode*>::iterator aux = _sceneNodes.find(nameSceneNode); 
		//Si cierto, significa que no existe dicha SceneNode
		if (aux == _sceneNodes.end())
			return 0;
		return aux->second;

	}
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
	CParticle * CScene::createParticle(const std::string &unicName, const std::string &particleName, const Vector3 &position, Vector3 *direction){

		CParticle *particle = new CParticle(unicName, particleName);

		particle->setPosition(position);

		//if(direction)
			//particle->setDirection(*direction);

		return particle;

	}

} // namespace Graphics
