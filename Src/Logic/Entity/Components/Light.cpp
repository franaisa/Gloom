/**
@file Light.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLight
@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Marzo, 2013
*/

#include "Light.h"

#include "Application/BaseApplication.h"

#include "Map/MapEntity.h"

#include "Graphics/Light.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CLight);

	CLight::CLight() : _light(NULL),
					   _position(Vector3::ZERO),
					   _direction(Vector3::NEGATIVE_UNIT_Y),
					   _color(Vector3::ZERO),
					   _attenuation(Vector3::ZERO),
					   _innerAngle(0.0f),
					   _outerAngle(0.0f) {
		
		// Nada que hacer
	}

	//---------------------------------------------------------

	CLight::~CLight() {
		if(_light != NULL)
			delete _light;
	}
	
	//---------------------------------------------------------
	
	bool CLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// ATRIBUTOS OBLIGATORIOS
		assert( entityInfo->hasAttribute("position") );

		_position = entityInfo->getVector3Attribute("position");

		// ATRIBUTOS OPCIONALES
		if( entityInfo->hasAttribute("direction") )
			_direction = entityInfo->getVector3Attribute("direction");

		if( entityInfo->hasAttribute("color") )
			_color = entityInfo->getVector3Attribute("color");

		if( entityInfo->hasAttribute("attenuation") )
			_attenuation = entityInfo->getVector3Attribute("attenuation");

		if( entityInfo->hasAttribute("innerAngle") )
			_innerAngle = entityInfo->getFloatAttribute("innerAngle");

		if( entityInfo->hasAttribute("outerAngle") )
			_outerAngle = entityInfo->getFloatAttribute("outerAngle");

		return true;

	} // spawn

	void CLight::onStart() {
		_light = new(nothrow) Graphics::CLight(_entity->getName(), _position, _direction);

		if( _color != Vector3::ZERO ) {
			_light->setColor(_color.x, _color.y, _color.z);
		}
		if( _attenuation != Vector3::ZERO ) {
			// De momento ignoramos el rango en los shaders
			_light->setAttenuation(0.0f, _attenuation.x, _attenuation.y, _attenuation.z);
		}
		if( _innerAngle != 0.0f || _outerAngle != 0.0f ) {
			_light->setSpotLightParams(_innerAngle, _outerAngle);
		}
	}

	/*void CLight::onTick(unsigned int msecs) {
		_light->setPosition( _entity->getPosition() );
		
		/*Matrix3 rotation;
		_entity->getOrientation().ToRotationMatrix(rotation);
		_light->setDirection( Math::getDirection( Matrix4(rotation) ) );*/
	//}

} // namespace Logic


