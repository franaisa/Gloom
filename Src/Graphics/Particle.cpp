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
	//Constructor de la clase CParticle. Donde se crea un ParticleSet (de momento con 1 sólo Particle)
	CParticle::CParticle(const std::string &unicName, const std::string &particleName, int contador)
	{
		
		_nameParticle = unicName;
		
		
		CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//scene->createSceneNode(nameSceneNode);

		contador++;
		char num[5];
		sprintf(num, "%d", contador);
		std::string nameSceneNode = "SceneNode_"+unicName + num;

		_particleSystem = scene->getSceneMgr()->createParticleSystem(_nameParticle+num,"SmokeParticles");

		/*
		Ogre::SceneNode *sceneNode = scene->getSceneMgr()->getRootSceneNode();
		
		scene->createSceneNode(nameSceneNode+num);

		sceneNode->attachObject(_particleSystem);

	

		/*/
		scene->createSceneNode(nameSceneNode+num);
		Ogre::SceneNode *sceneNode = scene->getSceneNode(nameSceneNode+num);
		sceneNode->setPosition(0,5,10);
		//Ogre::SceneNode *sceneNode = scene->getSceneNode();
		sceneNode->attachObject(_particleSystem);

		scene->getSceneMgr()->getSceneNode("weaponSniper_node")->addChild(sceneNode);

		//scene->getSceneMgr()->getRootSceneNode()->getChild("David_node")->addChild(sceneNode);
		/* */

	} // CParticle
	
	//--------------------------------------------------------

	/*void CParticle::deactivateParticle(const std::string &name) 
	{
		Graphics::CScene* _scen = Graphics::CServer::getSingletonPtr()->getActiveScene();
		//_scen->deleteParticle(name);

	} // deactivateParticle
	*/


	//--------------------------------------------------------


} // namespace Graphics
