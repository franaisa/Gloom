/**
@file PlayerClass.cpp

Contiene la implementación del componente que define
el comportamiento general de las habilidades de cada
clase.
 
@see Logic::CCharacterSound
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "CharacterSound.h"
#include "PhysicController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Audio/Server.h"

using namespace std;

namespace Logic {
	IMP_FACTORY(CCharacterSound);
	
	CCharacterSound::CCharacterSound() : _audioServer(NULL),
										 _footstepSound("step1.wav"),
										 _playerIsWalking(false),
										 _footstepTimeStep(365),
										 _footstepTimer(_footstepTimeStep) {
		
		// No hay memoria dinamica que reservar
	}

	//__________________________________________________________________

	CCharacterSound::~CCharacterSound() {
		// No hay memoria dinamica que liberar
	}

	//__________________________________________________________________

	bool CCharacterSound::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("physic_radius") );
		assert( entityInfo->hasAttribute("physic_height") );

		_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		_feetToNeckDist = ( 2 * entityInfo->getFloatAttribute("physic_height") ) - _capsuleRadius;

		return true;
	} // spawn

	//__________________________________________________________________

	void CCharacterSound::onStart() {
		// Nos quedamos con el puntero al servidor de audio para evitar sobrecarga
		_audioServer = Audio::CServer::getSingletonPtr();
		// Nos quedamos con el componente de movimiento para evitar sobrecarga
		// y nos registramos como observadores de el
		_avatarController = _entity->getComponent<CAvatarController>("CAvatarController");
		assert(_avatarController != NULL && "Error: Es necesario disponer del componente de movimiento");
		_avatarController->addObserver(this);

		// Nos registramos como observadores de la capsula fisica tambien
		CPhysicController* physicCont = _entity->getComponent<CPhysicController>("CPhysicController");
		assert(physicCont != NULL && "Error: Se necesita tener un controlador fisico");
		physicCont->addObserver(this);
	}

	//__________________________________________________________________

	void CCharacterSound::onTick(unsigned int msecs) {
		if(_playerIsWalking) {
			_footstepTimer -= msecs;
			if(_footstepTimer < 0) {
				_audioServer->playSound(_footstepSound, false, false);
				_footstepTimer = _footstepTimeStep;
			}
		}
	}

	//__________________________________________________________________

	void CCharacterSound::onActivate() {
		_playerIsWalking = false;
	}

	//__________________________________________________________________

	void CCharacterSound::onLand() {
		float landForce = _avatarController->getMomentum().y;

		// Sonido de aterrizar
		if(landForce < -0.7f)
			_audioServer->playSound("land.wav", false, false);

		// Sonido de hacernos daño al caer
		if(landForce < -2.0f)
			_audioServer->playSound("landingMaleGrunt.wav", false, false);
	}

	//__________________________________________________________________

	void CCharacterSound::onWalk() {
		_playerIsWalking = true;
		_footstepTimer = _footstepTimeStep;
	}

	//__________________________________________________________________

	void CCharacterSound::onJump() {
		_audioServer->playSound("jump.wav", false, false);
	}

	//__________________________________________________________________

	void CCharacterSound::onDodge() {
		_audioServer->playSound("sidejump.wav", false, false);
	}

	//__________________________________________________________________

	void CCharacterSound::onIdle() {
		_playerIsWalking = false;
	}

	//__________________________________________________________________

	void CCharacterSound::onAir() {
		_playerIsWalking = false;
	}

	//__________________________________________________________________

	void CCharacterSound::onShapeHit(IPhysics* otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) {
		if(otherComponent->getEntity()->getType() == "World") {
			// Como el mundo se carga desde fichero, podemos garantizar que sus
			// colliders tienen nombre asignado
			string material = otherComponent->getPhysicName();

			// Calculamos la distancia del punto de impacto a los pies para saber
			// si la colision ha sido contra la cabeza, el costado o los pies
			Vector3 vectorToFeet = colisionPos - _entity->getPosition();
			float distanceToFeet = vectorToFeet.length();

			// Colision con los pies
			if(distanceToFeet < _capsuleRadius) {
				// Actualizamos el sonido que debe reproducirse al pisar por si acaso
				// pisamos otro tipo de superficie
				_footstepSound = "footsteps/" + material + ".wav";
			}
			// Colision con la chola
			else if(distanceToFeet > _feetToNeckDist) {
				Vector3 momentum = _avatarController->getMomentum();

				if(momentum.length() > 1.5f) {
					//@todo
					// Reproducimos el sonido de impacto contra la chola
				}
			}
			// Colision lateral
			else {
				Vector3 momentum = _avatarController->getMomentum();
				Vector2 momentum2d(momentum.x, momentum.z);

				if(momentum2d.length() > 1.5f) {
					//@todo
					// Reproducimos el sonido de impacto contra el costado
				}
			}
		}
	}

} // namespace Logic
