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
			_particleName = entityInfo->getStringAttribute("particleName");

			if( entityInfo->hasAttribute("particleEmitterDirection") )
				_particleEmitterDirection = entityInfo->getVector3Attribute("particleEmitterDirection");
		}
		return true;

	} // spawn
	//---------------------------------------------------------

	
	void CParticle::onStart() {
		_scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		if(!_particleName.empty()){
			_particle = _scene->createParticle(_particleName, _entity->getPosition() + ( _particleOffset * _entity->getOrientation() ) );
		}
	} // onStart
	//---------------------------------------------------------
	
	void CParticle::onActivate() {
		

	} // activate
	//---------------------------------------------------------



	bool CParticle::accept(const std::shared_ptr<CMessage>& message) {
		
		return message->getMessageType() == Message::CREATE_PARTICLE;
		
	} // accept
	
	//---------------------------------------------------------

	void CParticle::process(const std::shared_ptr<CMessage>& message) {
		
		switch(message->getMessageType()) {
			case Message::CREATE_PARTICLE: {
				std::shared_ptr<CMessageCreateParticle> createParticleMsg = std::static_pointer_cast<CMessageCreateParticle>(message);

				// esta llamada puede devolver 0 en el caso de que no se pueda crear la particula
				_scene->createParticle(createParticleMsg->getParticle(), createParticleMsg->getPosition(),createParticleMsg->getDirectionWithForce());
				
				break;
			}
		}
	
	} // process
	//----------------------------------------------------------

	void CParticle::onTick(unsigned int msecs) {
		if(_particle)
			_particle->setPosition(_entity->getPosition() + _particleOffset);
	} // tick
	//----------------------------------------------------------

} // namespace Logic


