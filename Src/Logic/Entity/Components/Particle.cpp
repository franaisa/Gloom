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
#include "Graphics/PoolParticle.h"
#include <OgreSceneManager.h>
#include "Logic/Server.h"
#include "Logic/Messages/MessageCreateParticle.h"
#include "Logic/Messages/MessageParticleVisibility.h"

namespace Logic 
{
	IMP_FACTORY(CParticle);

	//---------------------------------------------------------

	CParticle::CParticle(){
		// Nada que hacer
	}

	//---------------------------------------------------------
	
	CParticle::~CParticle() {
		// nada que hacer ya que el vector llamara a todos los desctructores de las particulas
	}

	//---------------------------------------------------------
	
	bool CParticle::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;
		
		std::string particleName;
		Vector3 particleOffset;
		Vector3 particleEmitterDirection;
		bool particleVisible;
		if ( entityInfo->hasAttribute("particleName")){
			
			TParticle aux;
			particleName = entityInfo->getStringAttribute("particleName");
			aux._particleName = particleName;

			if( entityInfo->hasAttribute("particleOffset") ){
				particleOffset = entityInfo->getVector3Attribute("particleOffset");
				aux._particleOffset = particleOffset;
			}

			if( entityInfo->hasAttribute("particleEmitterDirection") ){
				particleEmitterDirection = entityInfo->getVector3Attribute("particleEmitterDirection");
				aux._particleEmitterDirection = particleEmitterDirection;
			}

			if( entityInfo->hasAttribute("particleVisible") ){
					particleVisible = entityInfo->getBoolAttribute("particleVisible");
					aux._particleVisible = particleVisible;
				}

			_particles.push_back(aux);
			//_particles;
		}else{
			
			unsigned int i = 0;
			std::stringstream findParticle;
			findParticle << "particle" << i;
			while ( entityInfo->hasAttribute(findParticle.str()+"Name")){
	
				TParticle aux;
				particleName = entityInfo->getStringAttribute(findParticle.str()+"Name");
				aux._particleName = particleName;

				if( entityInfo->hasAttribute(findParticle.str()+"Offset") ){
					particleOffset = entityInfo->getVector3Attribute(findParticle.str()+"Offset");
					aux._particleOffset = particleOffset;
				}

				if( entityInfo->hasAttribute(findParticle.str()+"EmitterDirection") ){
					particleEmitterDirection = entityInfo->getVector3Attribute(findParticle.str()+"EmitterDirection");
					aux._particleEmitterDirection = particleEmitterDirection;
				}

				if( entityInfo->hasAttribute(findParticle.str()+"Visible") ){
					particleVisible = entityInfo->getBoolAttribute(findParticle.str()+"Visible");
					aux._particleVisible = particleVisible;
				}
								
				_particles.push_back(aux);
			}

		}

		
		return true;

	} // spawn
	//---------------------------------------------------------

	
	void CParticle::onStart() {
		_scene = Graphics::CServer::getSingletonPtr()->getActiveScene();

		if(!_particles.empty()){
			for(auto it = _particles.begin(); it < _particles.end(); ++it){
				(*it)._particle = _scene->createParticle((*it)._particleName, _entity->getPosition() + ( (*it)._particleOffset * (_entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z) ), (*it)._particleEmitterDirection );
				// tengo q hacer esto para la habilidad del hound
				(*it)._particle->setVisible((*it)._particleVisible);
			}

		}
	} // onStart
	//---------------------------------------------------------
	
	void CParticle::onActivate() {
		
	} // activate
	//---------------------------------------------------------



	bool CParticle::accept(const std::shared_ptr<CMessage>& message) {
		
		return message->getMessageType() == Message::CREATE_PARTICLE ||
			message->getMessageType() == Message::PARTICLE_VISIBILITY;
		
	} // accept
	
	//---------------------------------------------------------

	void CParticle::process(const std::shared_ptr<CMessage>& message) {
		
		switch(message->getMessageType()) {
			case Message::CREATE_PARTICLE: {
				std::shared_ptr<CMessageCreateParticle> createParticleMsg = std::static_pointer_cast<CMessageCreateParticle>(message);

				// las particulas que se crean mediante mensajes, paso de ellas y no las almaceno.
				_scene->createParticle(createParticleMsg->getParticle(), createParticleMsg->getPosition(),createParticleMsg->getDirectionWithForce());
				
				break;
			}
			case Message::PARTICLE_VISIBILITY: {
				auto ParticleMsg = std::static_pointer_cast<CMessageParticleVisibility>(message);
				_scene->changeVisibilityParticle(ParticleMsg->getNameParticle(),ParticleMsg->getVisibility());
			break;
			}
		}
	
	} // process
	//----------------------------------------------------------

	void CParticle::onTick(unsigned int msecs) {
		if(!_particles.empty()){
			for(auto it = _particles.begin(); it < _particles.end(); ++it){
				Vector3 orientation = _entity->getOrientation() *Vector3::NEGATIVE_UNIT_Z;
				orientation.normalise();
				(*it)._particle->setPosition(_entity->getPosition() + ( (*it)._particleOffset * orientation ));
			}
		}
	} // tick
	//----------------------------------------------------------

} // namespace Logic


