/**
@file Particle.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CParticle
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#include "Particle.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Particle.h"
#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include <OgreSceneManager.h>
#include "Logic/Server.h"

namespace Logic 
{
	IMP_FACTORY(CParticle);

	//---------------------------------------------------------

	CParticle::CParticle() : _particleOffset(Vector3::ZERO),
							 _particleEmitterDirection(Vector3::ZERO) {
		// Nada que hacer
	}

	//---------------------------------------------------------
	
	CParticle::~CParticle() {
		delete _particle;
	}

	//---------------------------------------------------------
	
	bool CParticle::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		if( entityInfo->hasAttribute("particleOffset") )
			_particleOffset = entityInfo->getVector3Attribute("particleOffset");

		if( entityInfo->hasAttribute("particleEmitterDirection") )
			_particleEmitterDirection = entityInfo->getVector3Attribute("particleEmitterDirection");

		assert( entityInfo->hasAttribute("particleName") && "Error: No se ha establecido un nombre de particula" );
		std::string particleName = entityInfo->getStringAttribute("particleName");

		_particle = new Graphics::CParticle( _entity->getName(), particleName );
		_particle->setPosition( _entity->getPosition() + ( _particleOffset * _entity->getOrientation() ) );
		_particle->setDirection( _particleEmitterDirection * _entity->getOrientation() );

		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CParticle::activate() {
		IComponent::activate();

	} // activate
	//---------------------------------------------------------



	bool CParticle::accept(CMessage *message) {
		return false;
		//return message->getMessageType() == Message::DAMAGED;
		
	} // accept
	
	//---------------------------------------------------------

	void CParticle::process(CMessage *message) {
		/*
		switch(message->getMessageType())
		{

		}
		*/

	} // process
	//----------------------------------------------------------

	void CParticle::tick(unsigned int msecs) {
		IComponent::tick(msecs);

		_particle->setPosition(_entity->getPosition() + _particleOffset);
	} // tick
	//----------------------------------------------------------



} // namespace Logic


