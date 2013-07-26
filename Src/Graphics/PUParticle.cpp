//---------------------------------------------------------------------------
// PUParticle.cpp
//---------------------------------------------------------------------------

/**
@file PUParticle.cpp

Contiene la implementación de la clase que maneja el PUParticle.

@see Graphics::PUParticle

@author Rubén Mulero Guerrero
@author Antonio Jesús Narváez Corrales

@date Enero, 2013
*/

#include "PUParticle.h"
#include "Scene.h"
#include "Server.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <ParticleUniverse\ParticleUniverseSystemManager.h>

#include <OgreParticleAffector.h>

namespace Graphics 
{
	//Constructor de la clase PUParticle
	PUParticle::PUParticle(const std::string &particleName, bool isOverlay)
	{
		static int counter = 0;
	
		Ogre::SceneManager *scene = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();

		char num[5];
		sprintf(num, "%d", counter++);

		_name = particleName + num;	

		_particleSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(_name, particleName, scene);
		
		_sceneNode = scene->getRootSceneNode()->createChildSceneNode();
		
		_sceneNode->attachObject(_particleSystem);
	} // PUParticle
	//--------------------------------------------------------

	PUParticle::~PUParticle(){
	
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(_name,Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr());
	
		_particleSystem = NULL;
		//_sceneNode = NULL;
		
	} // ~PUParticle
	//--------------------------------------------------------

	void PUParticle::setPosition(const Vector3 &position){
		_sceneNode->setPosition(position);

	} // setPosition
	//--------------------------------------------------------

	Vector3 PUParticle::getPosition(){
		return _sceneNode->getPosition();
	} // getPosition
	//--------------------------------------------------------
	
	void PUParticle::setDirection(const Vector3 &directionWithForce){
		
		_sceneNode->setDirection(directionWithForce);
				
	} // setDirection
	//--------------------------------------------------------

	void PUParticle::setVisible(bool visible){
		_particleSystem->setVisible(visible);

	} // setVisible
	//--------------------------------------------------------
	
	void PUParticle::activate()
	{
		_particleSystem->start();

	} // activate
	//--------------------------------------------------------

	void PUParticle::deactivate()
	{
		_particleSystem->stop();
		//_particleSystem->clear();

	} // deactivate
	//--------------------------------------------------------

	void PUParticle::loadResources()
	{
		activate();
		this->setPosition(Vector3(0,-5000,0));
		deactivate();

	} // loadResources
	//--------------------------------------------------------

	bool PUParticle::isEmitting(){
		return (_particleSystem->isEnabled());

	} // isEmitting
	//--------------------------------------------------------
	
	bool PUParticle::isLoaded(){
		return _particleSystem->isInScene();
	}

} // namespace Graphics
