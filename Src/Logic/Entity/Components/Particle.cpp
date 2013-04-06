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
#include "Logic/Messages/MessageCreateParticle.h"

namespace Logic 
{
	IMP_FACTORY(CParticle);

	//---------------------------------------------------------

	CParticle::CParticle() : _particleOffset(Vector3::ZERO),
							 _particleEmitterDirection(Vector3::ZERO),
							 _particle(0){
		// Nada que hacer
	}

	//---------------------------------------------------------
	
	CParticle::~CParticle() {
		if(_particle)
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

		if ( entityInfo->hasAttribute("particleName")){
			std::string particleName = entityInfo->getStringAttribute("particleName");

			_particle = new Graphics::CParticle( _entity->getName(), particleName );
			_particle->setPosition( _entity->getPosition() + ( _particleOffset * _entity->getOrientation() ) );

			if( entityInfo->hasAttribute("particleEmitterDirection") )
				_particle->setDirection( _particleEmitterDirection * _entity->getOrientation() );
		}
		return true;

	} // spawn
	
	//---------------------------------------------------------


	
	void CParticle::activate() {
		IComponent::activate();

	} // activate
	//---------------------------------------------------------



	bool CParticle::accept(CMessage *message) {
		
		return message->getMessageType() == Message::CREATE_PARTICLE;
		
	} // accept
	
	//---------------------------------------------------------

	void CParticle::process(CMessage *message) {
		
		switch(message->getMessageType())
		{

		case Message::CREATE_PARTICLE:
			CMessageCreateParticle *msg = static_cast<CMessageCreateParticle*>(message);
			Graphics::CParticle *particle = Graphics::CServer::getSingletonPtr()->getActiveScene()->createParticle(
						_entity->getName(),msg->getParticle(), msg->getPosition());
			break;

		}
		

	} // process
	//----------------------------------------------------------

	void CParticle::tick(unsigned int msecs) {
		IComponent::tick(msecs);
		if(_particle)
			_particle->setPosition(_entity->getPosition() + _particleOffset);
	} // tick
	//----------------------------------------------------------



} // namespace Logic


