//---------------------------------------------------------------------------
// PUParticle.cpp
//---------------------------------------------------------------------------

/**
@file PUParticle.cpp

Contiene la implementación de la clase que maneja el PUParticle.

@see Graphics::PUParticle

@author Rubén Mulero Guerrero
@author Antonio Jesús Narváez Corrales
@author Francisco Aisa García

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

using namespace std;

namespace Graphics {
	
	unsigned int PUParticle::_counter = 0;

	//______________________________________________________________________________

	//Constructor de la clase PUParticle
	PUParticle::PUParticle(const std::string& scriptName) {
		// Obtenemos los punteros a los singleton
		Ogre::SceneManager* sceneMgr = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();
		ParticleUniverse::ParticleSystemManager* particleMgr = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

		// Creamos un nombre unico (por si las moscas)
		std::stringstream particleName;
		particleName << scriptName;
		particleName << _counter++;

		// Nos quedamos con el nombre que asignamos al sistema
		// de particulas
		_name = particleName.str();
		
		// Le pedimos a particle universe que nos cree
		// un sistema de particulas a partir del script dado
		// por parametro
		_particleSystem = particleMgr->createParticleSystem(_name, scriptName, sceneMgr);

		// Atachamos el sistema de particulas a la escena
		_sceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		_sceneNode->attachObject(_particleSystem);

		// Realizamos la carga del sitema de particulas aqui
		// para evitar caidas en el frame rate
		_particleSystem->prepare();

		// Nos registramos como observadores del sistema de particulas creado
		// para que se nos notifique ante ciertos eventos
		_particleSystem->addParticleSystemListener(this); // En general no vamos a necesitar hacer el remove
	}
	
	//______________________________________________________________________________

	PUParticle::~PUParticle() {
		// Obtenemos los punteros a los singleton
		Ogre::SceneManager* sceneMgr = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();
		ParticleUniverse::ParticleSystemManager* particleMgr = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		
		// Paramos el sistema de particulo por si acaso se estaba
		// ejecutando
		_particleSystem->stop();

		// Desatachamos el sistema de particulas del nodo de escena
		// al que pertenece
		//_sceneNode->detachObject(_particleSystem);
		
		// Nos desregistramos como listeners
		_particleSystem->removeParticleSystemListener(this);

		// Pedimos a particle universe que destruya el sistema de particulas
		particleMgr->destroyParticleSystem(_name, sceneMgr);
	}

	//______________________________________________________________________________

	void PUParticle::handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem,
											   ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent) {

		using namespace ParticleUniverse;

		switch(particleUniverseEvent.eventType) {
			// Cuando el emisor comienza a emitir
			case PU_EVT_EMITTER_STARTED: {
				for(auto it = _observers.begin(); it != _observers.end(); ++it)
					(*it)->onEmitterStart();

				break;
			}
			// Cuando el emisor para de emitir
			case PU_EVT_EMITTER_STOPPED: {
				for(auto it = _observers.begin(); it != _observers.end(); ++it)
					(*it)->onEmitterStop();

				break;
			}
			// Cuando se acaban las particulas
			case PU_EVT_NO_PARTICLES_LEFT: {
				for(auto it = _observers.begin(); it != _observers.end(); ++it)
					(*it)->onParticlesExpired();

				break;
			}
		}
	}

	//______________________________________________________________________________

	void PUParticle::start() {
		_particleSystem->start();
	}

	//______________________________________________________________________________

	void PUParticle::start(float stopTime) {
		_particleSystem->start(stopTime);
	}

	//______________________________________________________________________________

	void PUParticle::startAndStopFade(float stopTime) {
		_particleSystem->startAndStopFade(stopTime);
	}

	//______________________________________________________________________________
		
	void PUParticle::stop() {
		_particleSystem->stop();
	}

	//______________________________________________________________________________

	void PUParticle::stop(float stopTime) {
		_particleSystem->stop(stopTime);
	}

	//______________________________________________________________________________

	void PUParticle::stopFade(float stopTime) {
		_particleSystem->stopFade(stopTime);
	}

	//______________________________________________________________________________

	void PUParticle::pause() {
		_particleSystem->pause();
	}

	//______________________________________________________________________________

	void PUParticle::pause(float pauseTime) {
		_particleSystem->pause(pauseTime);
	}

	//______________________________________________________________________________

	void PUParticle::resume() {
		_particleSystem->resume();
	}
	
	//______________________________________________________________________________

	void PUParticle::setPosition(const Ogre::Vector3 &position) {
		_sceneNode->setPosition(position);
	} // setPosition
	
	//______________________________________________________________________________

	Ogre::Vector3 PUParticle::getPosition() {
		return _sceneNode->getPosition();
	} // getPosition
	
	//______________________________________________________________________________
	
	void PUParticle::setDirection(const Ogre::Vector3 &direction) {
		_sceneNode->setDirection(direction);
	} // setDirection

	//______________________________________________________________________________

	void PUParticle::setOrientation(const Ogre::Quaternion &orientation) {
		_sceneNode->setOrientation(orientation);
	}
	
	//______________________________________________________________________________

	void PUParticle::setVisible(bool visible) {
		_particleSystem->setVisible(visible);
	} // setVisible

	//______________________________________________________________________________

	void PUParticle::addObserver(IObserver* observer) {
		_observers.push_back(observer);
	}

	//______________________________________________________________________________

	void PUParticle::removeObserver(IObserver* observer) {
		for(auto it = _observers.begin(); it != _observers.end(); ++it) {
			if(*it == observer) {
				_observers.erase(it);
				break;
			}
		}
	}

} // namespace Graphics
