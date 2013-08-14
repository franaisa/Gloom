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

#include "Audio/Server.h"

namespace Logic {
	IMP_FACTORY(CCharacterSound);
	
	CCharacterSound::CCharacterSound() : _audioServer(NULL),
										 _footstepTimeStep(1000),
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

		return true;
	} // spawn
	
	//__________________________________________________________________

	bool CCharacterSound::accept(const std::shared_ptr<CMessage>& message) {
		return false;
	} // accept

	//__________________________________________________________________

	void CCharacterSound::process(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

	} // process

	//__________________________________________________________________

	void CCharacterSound::onStart() {
		// Nos quedamos con el puntero al servidor de audio para evitar sobrecarga
		_audioServer = Audio::CServer::getSingletonPtr();
		// Nos quedamos con el componente de movimiento para evitar sobrecarga
		_avatarController = _entity->getComponent<CAvatarController>("CAvatarController");
		assert(_avatarController != NULL && "Error: Es necesario disponer del componente de movimiento");
		// Añadimos el observador (no es necesario eliminarlo mas tarde)
		_avatarController->addObserver(this);
	}

	//__________________________________________________________________

	void CCharacterSound::onTick(unsigned int msecs) {
		_footstepTimer -= msecs;
		if(_footstepTimer < 0) {
			//_audioServer->playSound("footsteps", false, false);
			_footstepTimer = _footstepTimeStep;
		}
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
		std::cout << "Ando" << std::endl;
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
		std::cout << "Paro" << std::endl;
	}

	//__________________________________________________________________

	void CCharacterSound::onAir() {
		std::cout << "En el aire" << std::endl;
	}

} // namespace Logic
