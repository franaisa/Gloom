#include "Light.h"
#include "Scene.h"
#include "Server.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics {

//////////////////////////////////////////////////////////////////////////////////////////////////////

	CLight::CLight(LightType::Enum lightType, const std::string& lightName, const Vector3& position, const Vector3& direction) {
		// Creamos una luz a través del gestor de escena
		_light = CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->createLight(lightName);

		switch(lightType) {
			case LightType::eDIRECTIONAL_LIGHT:
				_light->setType(Ogre::Light::LT_DIRECTIONAL);
				break;

			case LightType::ePOINT_LIGHT:
				_light->setType(Ogre::Light::LT_POINT);
				break;

			case LightType::eSPOT_LIGHT:
				_light->setType(Ogre::Light::LT_SPOTLIGHT);
				break;
		}

		// Asignamos solo los atributos esenciales, posicion y direccion (si es que la tienen)
		_light->setPosition(position);
		_light->setDirection(direction);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	CLight::~CLight() {
		if(_light != NULL) {
			CServer::getSingletonPtr()->getActiveScene()->getSceneMgr()->destroyLight(_light);
			_light = NULL;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setPosition(const Vector3& position) {
		_light->setPosition(position);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setDirection(const Vector3& direction) {
		_light->setDirection(direction);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setOrientation(const Quaternion& rotation) {
		_light->setDirection( -rotation.zAxis() );
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setCastShadows(bool enabled){
		_light->setCastShadows(enabled);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setColor(float r, float g, float b){
		_light->setDiffuseColour(r, g, b);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setAttenuation(float Range, float Kc, float Kl, float Kq) {
		_light->setAttenuation(Range, Kc, Kl, Kq);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	void CLight::setSpotLightParams(float innerAngle, float outerAngle) {
		_light->setSpotlightInnerAngle( Ogre::Radian(innerAngle) );
		_light->setSpotlightOuterAngle( Ogre::Radian(outerAngle) );
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector3 CLight::getDirection() {
		return _light->getDirection();
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	Vector3 CLight::getPosition() {
		return _light->getDirection();
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string CLight::getName() {
		return _light->getName();
	}
}