/**
@file ParticleSystem.cpp


@see Logic::CParticleSystem
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "ParticleSystem.h"

//#include "Logic/Entity/Entity.h"
//#include "Logic/Maps/Map.h"

// Arquitectura
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

// Mensajes
#include "Logic/Messages/MessageCreateParticle.h"
#include "Logic/Messages/MessageParticleVisibility.h"

using namespace std;

namespace Logic {
	
	IMP_FACTORY(CParticleSystem);

	//______________________________________________________________________________

	CParticleSystem::CParticleSystem() : _particleSystem(NULL),
									     _emitting(false) {
		// Nada que hacer
	}

	//______________________________________________________________________________
	
	CParticleSystem::~CParticleSystem() {
		if(_particleSystem != NULL)
			delete _particleSystem;
	}

	//______________________________________________________________________________
	
	bool CParticleSystem::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;
		
		assert( entityInfo->hasAttribute("particle_scriptName") );
		assert( entityInfo->hasAttribute("particle_on") );

		// Nombre del script de particulas (nombre contenido dentro del fichero .pu)
		_scriptName = entityInfo->getStringAttribute("particle_scriptName");
		// true si queremos que las particulas emitan nada mas crearse
		_emitting = entityInfo->getBoolAttribute("particle_on");

		if( entityInfo->hasAttribute("rotation") )
			_direction = -entityInfo->getQuaternionAttribute("rotation").zAxis();
		else if( entityInfo->hasAttribute("direction") )
			_direction = entityInfo->getVector3Attribute("direction");

		if( entityInfo->hasAttribute("particle_destroyEntityOnExpiry") )
			_destroyEntityOnExpiry = entityInfo->getBoolAttribute("particle_destroyEntityOnExpiry");

		return true;
	} // spawn
	
	//______________________________________________________________________________
	
	void CParticleSystem::onStart() {
		_particleSystem = new(nothrow) Graphics::PUParticle(_scriptName);
		_particleSystem->addObserver(this); // No es necesario hacer el remove
		_particleSystem->setPosition( _entity->getPosition() );
		_particleSystem->setOrientation( _entity->getOrientation() );

		if(_emitting)
			_particleSystem->start();
	} // onStart

	//______________________________________________________________________________

	void CParticleSystem::onParticlesExpired() {
		if(_destroyEntityOnExpiry)
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity, false);
	}

} // namespace Logic


