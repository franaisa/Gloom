/**
@file HHFXParticle.cpp

Contiene la implementación de la clase que maneja el Particle.

@see Graphics::HHFXParticle

@author Rubén Mulero Guerrero
@author Antonio Jesus Narvaez
@date May, 2013
*/

#include "HHFXParticle.h"
#include "Scene.h"
#include "Server.h"
#include <assert.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	//Constructor de la clase CParticle
	HHFXParticle::HHFXParticle(const std::string &particleName,Vector3 position, bool startEmitting)
	{
		_particleSystem = HHFX::getSingletonPtr();

		_particle = _particleSystem->createParticle(particleName);

		if(_particle){
			_particleNode = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
			_particleNode->attachObject(_particle);
			_particleNode->setPosition(position);
		}

		if(!startEmitting){
			this->deactivate();
		}

	} // CParticle
	//--------------------------------------------------------

	HHFXParticle::~HHFXParticle(){
		_particleSystem = NULL;
		//_sceneNode = NULL;
		
	} // ~CParticle
	//--------------------------------------------------------

	void HHFXParticle::setPosition(const Vector3 &position){
		_particleNode->setPosition(position);

	} // setPosition
	//--------------------------------------------------------

	Vector3 HHFXParticle::getPosition(){
		return _particleNode->getPosition();
	} // getPosition
	//--------------------------------------------------------
	
	void HHFXParticle::setDirection(const Vector3 &directionWithForce){
		
		_particleNode->setDirection(directionWithForce*10);
	} // setDirection
	//--------------------------------------------------------

	void HHFXParticle::setVisible(bool visible){
		_particle->setVisible(visible);

	} // setVisible
	//--------------------------------------------------------
	
	void HHFXParticle::activate()
	{
		_particle->RunFX();

	} // activate
	//--------------------------------------------------------

	void HHFXParticle::deactivate()
	{
		_particle->StopFX();

	} // deactivate
	//--------------------------------------------------------

	void HHFXParticle::loadResources()
	{
		activate();
		this->setPosition(Vector3(0,-5000,0));
		deactivate();

	} // loadResources
	//--------------------------------------------------------

	bool HHFXParticle::isEmitting(){
		//return _particleSystem->getEmitting();
		//printf("\numParticles: %d",_particleSystem->getNumParticles());
		return _particle->isVisible();

	} // isEmitting
	//--------------------------------------------------------
	
	void HHFXParticle::setOrientation(const Matrix3 &orientation)
	{
		_particleNode->setOrientation(orientation);
	} // setOrientatio

} // namespace Graphics
