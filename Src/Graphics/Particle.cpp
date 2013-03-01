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
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OgreParticle.h>

namespace Graphics 
{
	//Constructor de la clase CParticle
	CParticle::CParticle(const std::string &unicName, const std::string &particleName)
	{
		static int counter = 0;
		_nameParticle = unicName;	
		
		CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//scene->createSceneNode(nameSceneNode);
		char num[5];
		sprintf(num, "%d", counter);
		std::string nameSceneNode = "SceneNode_"+_nameParticle + num;

		_particleSystem = scene->getSceneMgr()->createParticleSystem(_nameParticle+num, particleName);
		_sceneNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(nameSceneNode + "_node");
		_sceneNode->attachObject(_particleSystem);

		counter++;
	} // CParticle

	void CParticle::setPosition(const Vector3 *position){
		_sceneNode->setPosition(*position);

	} // setPosition

	Vector3 CParticle::getPosition(){
		return _sceneNode->getPosition();
	}
	
	//--------------------------------------------------------
	/*
	void CParticle::deactivateParticle(const std::string &name) 
	{
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_scen->deleteParticle(name);

	} // deactivateParticle
	*/

	//--------------------------------------------------------
	
	CParticle::~CParticle(){
	
		delete _particleSystem;
		delete _sceneNode;
	}

} // namespace Graphics
