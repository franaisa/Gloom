/**
@file StaticAudio.cpp


@see Logic::CStaticAudio
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#include "StaticAudio.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Audio/Server.h"

using namespace std;

namespace Logic {
	IMP_FACTORY(CStaticAudio);

	CStaticAudio::CStaticAudio() : _position(Vector3::ZERO) {
		// Nada que hacer
	}

	bool CStaticAudio::spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		assert( entityInfo->hasAttribute("staticAudio_name") );
		assert( entityInfo->hasAttribute("staticAudio_stream") );
		
		_audioName = entityInfo->getStringAttribute("staticAudio_name");
		_streamAudio = entityInfo->getBoolAttribute("staticAudio_stream");

		if( entityInfo->hasAttribute("staticAudio_pos") ) {
			_position = entityInfo->getVector3Attribute("staticAudio_pos");
		}

		return true;
	} // spawn

	void CStaticAudio::onActivate() {
		Vector3 audioPos = _position != Vector3::ZERO ? _position : _entity->getPosition();
		_channelIndex = Audio::CServer::getSingletonPtr()->playSound3D(_audioName, audioPos, Vector3::ZERO, true, _streamAudio);
	}

	void CStaticAudio::onDeactivate() {
		// Pedimos que se borre el sonido de ese canal porque siempre va a estar ocupado
		// ya que todos los sonidos ambientales siempre estan en loop
		Audio::CServer::getSingletonPtr()->stopSound(_channelIndex);
	}

} // namespace Logic


