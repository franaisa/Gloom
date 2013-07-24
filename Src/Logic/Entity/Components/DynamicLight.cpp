/**
@file DynamicLight.cpp


@see Logic::CDynamicLight
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#include "DynamicLight.h"
#include "Logic/LightManager.h"

#include "Application/BaseApplication.h"
#include "Map/MapEntity.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CDynamicLight);

	CDynamicLight::CDynamicLight() : _timer(0.0f) {
		// Nada que hacer
		_isStatic = false;
	}

	//________________________________________________________________________

	CDynamicLight::~CDynamicLight() {
		// Nada que hacer
	}

	//________________________________________________________________________
	
	void CDynamicLight::turnOn(const Vector3& offset, float timeToLive) {
		// El tiempo dado esta en segundos, lo pasamos a msecs
		_timer = timeToLive * 1000;
		// Offset que la luz va a tener
		_offset = offset;

		// Creamos la luz con la información del arquetipos y la posicion de la entidad
		_light = CLightManager::getSingletonPtr()->createLight(_lightType, _entity->getName(), _isStatic, _entity->getPosition(), -_entity->getOrientation().zAxis());

		if(_light != NULL) {
			if( _color != Vector3::ZERO ) {
				_light->setColor(_color.x, _color.y, _color.z);
			}
			if( _attenuation != Vector3::ZERO ) {
				// De momento ignoramos el rango en los shaders
				_light->setAttenuation(_range != 0.0f ? _range : 0xFFFFFFFF, _attenuation.x, _attenuation.y, _attenuation.z);
			}
			if( _innerAngle != 0.0f || _outerAngle != 0.0f ) {
				_light->setSpotLightParams(_innerAngle, _outerAngle);
			}
		}
	}

	//________________________________________________________________________

	void CDynamicLight::turnOff() {
		// Destruimos la luz
		CLightManager::getSingletonPtr()->destroyLight(_light, _isStatic);
		_light = NULL;
	}

	//________________________________________________________________________

	void CDynamicLight::onTick(unsigned int msecs) {
		if(_light == NULL) return;

		_light->setPosition( _entity->getPosition() + _offset );
		_light->setOrientation( _entity->getOrientation() );

		if(_timer != 0.0f) {
			_timer -= msecs;
			if(_timer <= 0.0f) {
				turnOff();
			}
		}
	}

} // namespace Logic
