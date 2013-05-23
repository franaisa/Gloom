//David Llanso Tutoria
/*

//Constructor
CParticle(name){

	_bbset = CServer::getSingletonPtr()->getActiveScene()->createParticle(name);
}

// Destructor 

*/
//metodos de setDimensions....activate, deactivate, setcoord...
//Dentro se utilizaria. _bbset->getParticle(0) para acceder al primer Particle del conjunto de Particles.

//---------------------------------------------------------------------------
// Particle.cpp
//---------------------------------------------------------------------------

/**
@file Particle.cpp

Contiene la implementación de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@modifedBy Antonio Jesus Narvaez
@date Enero, 2013
*/

#include "Particle.h"
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
	//Constructor de la clase CParticle
	CParticle::CParticle(const std::string &particleName, bool isOverlay)
	{
		static int counter = 0;
	
		
		CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//scene->createSceneNode(nameSceneNode);
		char num[5];
		sprintf(num, "%d", counter);
		std::string nameSceneNode = "SceneNode_"+particleName + num;
		_nameParticle = particleName + num;	
		/*
		if(scene->getSceneMgr()->hasParticleSystem(_nameParticle)){
			_particleSystem = scene->getSceneMgr()->getParticleSystem(_nameParticle);
			
			_sceneNode = scene->getSceneMgr()->getSceneNode(nameSceneNode + "_node");
		}else{
		*/
			_particleSystem = scene->getSceneMgr()->createParticleSystem(_nameParticle, particleName);

			if(isOverlay){
				_sceneNode = new Ogre::SceneNode(scene->getSceneMgr(), nameSceneNode);
			}else{
				_sceneNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(nameSceneNode + "_node");
			}
		
		
			_sceneNode->attachObject(_particleSystem);
		//}

		//_particleSystem->createParticle();

		++counter;
	} // CParticle
	//--------------------------------------------------------

	CParticle::~CParticle(){
	
		Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyParticleSystem(_particleSystem);
		//Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroySceneNode(_sceneNode);
	
		_particleSystem = NULL;
		//_sceneNode = NULL;
		
	} // ~CParticle
	//--------------------------------------------------------

	void CParticle::setPosition(const Vector3 &position){
		_sceneNode->setPosition(position);

	} // setPosition
	//--------------------------------------------------------

	Vector3 CParticle::getPosition(){
		return _sceneNode->getPosition();
	} // getPosition
	//--------------------------------------------------------
	
	void CParticle::setDirection(const Vector3 &directionWithForce){
		
		_particleSystem->getEmitter(0)->setDirection(directionWithForce*10);
				
	} // setDirection
	//--------------------------------------------------------
	
	void CParticle::activate()
	{
		_particleSystem->setEmitting(false);

	} // activate
	//--------------------------------------------------------

	void CParticle::deactivate()
	{
		_particleSystem->setEmitting(false);

	} // deactivate
	//--------------------------------------------------------

	void CParticle::loadResources()
	{
		activate();
		this->setPosition(Vector3(0,-5000,0));
		deactivate();

	} // loadResources
	//--------------------------------------------------------

	bool CParticle::isEmitting(){
		return _particleSystem->getEmitting();

	} // isEmitting
	//--------------------------------------------------------
	
	

} // namespace Graphics
