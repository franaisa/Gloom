//---------------------------------------------------------------------------
// OgreParticle.cpp
//---------------------------------------------------------------------------

/**
@file OgreParticle.cpp

Contiene la implementación de la clase que maneja el OgreParticle.

@see Graphics::COgreParticle

@author Pablo Terrado (Proyecto: El rayo de Zeus)
@modifedBy Antonio Jesus Narvaez
@date Enero, 2013
*/

#include "OgreParticle.h"
#include "Scene.h"
#include "Server.h"

//#include "BaseSubsystems/Server.h"


#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticle.h>
#include <OgreParticleEmitter.h>

#include <OgreParticleAffector.h>

namespace Graphics 
{
	//Constructor de la clase COgreParticle
	COgreParticle::COgreParticle(const std::string &ogreOgreParticleName, bool isOverlay)
	{
		static int counter = 0;
	
		
		CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//scene->createSceneNode(nameSceneNode);
		char num[5];
		sprintf(num, "%d", counter);
		std::string nameSceneNode = "SceneNode_"+ogreOgreParticleName + num;
		_nameOgreParticle = ogreOgreParticleName + num;	
		/*
		if(scene->getSceneMgr()->hasOgreParticleSystem(_nameOgreParticle)){
			_ogreOgreParticleSystem = scene->getSceneMgr()->getOgreParticleSystem(_nameOgreParticle);
			
			_sceneNode = scene->getSceneMgr()->getSceneNode(nameSceneNode + "_node");
		}else{
		*/
			_particleSystem = scene->getSceneMgr()->createParticleSystem(_nameOgreParticle, ogreOgreParticleName);
			
			if(isOverlay){
				_sceneNode = new Ogre::SceneNode(scene->getSceneMgr(), nameSceneNode);
			}else{
				_sceneNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(nameSceneNode + "_node");
			}
		
		
			_sceneNode->attachObject(_particleSystem);
		//}

		//_ogreOgreParticleSystem->createOgreParticle();

		++counter;
	} // COgreParticle
	//--------------------------------------------------------

	COgreParticle::~COgreParticle(){
	
		Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyParticleSystem(_particleSystem);
		//Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroySceneNode(_sceneNode);
	
		_particleSystem = NULL;
		//_sceneNode = NULL;
		
	} // ~COgreParticle
	//--------------------------------------------------------

	void COgreParticle::setPosition(const Vector3 &position){
		_sceneNode->setPosition(position);

	} // setPosition
	//--------------------------------------------------------

	Vector3 COgreParticle::getPosition(){
		return _sceneNode->getPosition();
	} // getPosition
	//--------------------------------------------------------
	
	void COgreParticle::setDirection(const Vector3 &directionWithForce){
		
		_particleSystem->getEmitter(0)->setDirection(directionWithForce*10);
				
	} // setDirection
	//--------------------------------------------------------

	void COgreParticle::setVisible(bool visible){
		_particleSystem->setVisible(visible);

	} // setVisible
	//--------------------------------------------------------
	
	void COgreParticle::activate()
	{
		_particleSystem->setEmitting(true);
		for(int i = 0 ; i < _particleSystem->getNumEmitters(); ++i){
			
			_particleSystem->getEmitter(i)->resetDimensions();
			/*
			_ogreOgreParticleSystem->getEmitter(i)->setEmitted(false);
			_ogreOgreParticleSystem->getEmitter(i)->setEmitted(true);
			*/			
		}

	} // activate
	//--------------------------------------------------------

	void COgreParticle::deactivate()
	{
		_particleSystem->setEmitting(false);
		_particleSystem->clear();

	} // deactivate
	//--------------------------------------------------------

	void COgreParticle::loadResources()
	{
		activate();
		this->setPosition(Vector3(0,-5000,0));
		deactivate();

	} // loadResources
	//--------------------------------------------------------

	bool COgreParticle::isEmitting(){
		//return _ogreOgreParticleSystem->getEmitting();
		//printf("\numOgreParticles: %d",_ogreOgreParticleSystem->getNumOgreParticles());
		return (_particleSystem->getNumParticles() > 0);

	} // isEmitting
	//--------------------------------------------------------
	
	

} // namespace Graphics
