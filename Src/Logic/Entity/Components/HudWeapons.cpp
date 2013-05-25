/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "HudWeapons.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


#include "Graphics/Camera.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Graphics/Overlay.h"

#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageHudDebugData.h"

#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

using namespace std;

namespace Logic {
	IMP_FACTORY(CHudWeapons);
	
	//---------------------------------------------------------

	CHudWeapons::CHudWeapons() : _currentWeapon(0), 
								 _graphicsEntities(0),
								 _playerIsWalking(false),
								 _playerIsLanding(false),
								 _landForce(0.0f),
								 _currentLandOffset(0.0f),
								 _landSpeed(-0.005f),
								 _landRecoverySpeed(0.01f),
								 _offset(Vector3::ZERO) {

		_runAnim.currentHorizontalPos = Math::HALF_PI;
		_runAnim.horizontalSpeed = 0.0075f;
		_runAnim.horizontalOffset = 0.06f;

		_runAnim.currentVerticalPos = Math::HALF_PI;
		_runAnim.verticalSpeed = 2 * _runAnim.horizontalSpeed;
		_runAnim.verticalOffset = 0.06f;

		_playerIsWalking = false;
	}

	//---------------------------------------------------------

	CHudWeapons::~CHudWeapons() {
		if(_graphicsEntities) {
			for(int i = 0; i < WeaponType::eSIZE; ++i) {
				//_scene->removeChild(_graphicsEntities[i]._graphicsEntity);
				delete _graphicsEntities[i].graphicsEntity;
				_graphicsEntities[i].graphicsEntity = NULL;

				delete _overlayWeapon3D[i];
			}
			delete _graphicsEntities;
		}

	} // ~CGraphics
	//---------------------------------------------------------
	
	void CHudWeapons::onActivate() {
		//Cuando activamos el componente solo tendremos visible el arma 0( arma melee)
		_currentWeapon = 0;
		_graphicsEntities[_currentWeapon].graphicsEntity->setVisible(true);
	} // activate
	//---------------------------------------------------------

	void CHudWeapons::onDeactivate() {
		//Cuando desactivamos el componente, desactivaremos el arma actual
		_overlayWeapon3D[_currentWeapon]->setVisible(false);
	} // deactivate
	//---------------------------------------------------------

	bool CHudWeapons::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();
			
		_graphicsEntities = new TGraphicsWeapon[WeaponType::eSIZE];
		
		// Por ahora leo a mano cada una de las armas que tiene el usuario
	
		for(int i = WeaponType::eHAMMER; i < WeaponType::eSIZE; ++i){
				
			WeaponType::Enum current = (WeaponType::Enum)i;
			std::string currentOnText = WeaponType::toString(current);
		
			std::stringstream aux;
			aux << "weapon" << currentOnText;
			std::string weapon = aux.str();
				
			//_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
			_graphicsEntities[current].yaw = _graphicsEntities[current].pitch = _graphicsEntities[current].roll = 0;
			if(entityInfo->hasAttribute(weapon+"ModelYaw"))
				_graphicsEntities[current].yaw = entityInfo->getFloatAttribute(weapon+"ModelYaw");
			if(entityInfo->hasAttribute(weapon+"ModelPitch"))
				_graphicsEntities[current].pitch = entityInfo->getFloatAttribute(weapon+"ModelPitch");
			if(entityInfo->hasAttribute(weapon+"ModelRoll"))
				_graphicsEntities[current].pitch = entityInfo->getFloatAttribute(weapon+"ModelRoll");
				
			//Esto puede petar si no esta, pero creo q es obligatorio
			if(!entityInfo->hasAttribute(weapon+"Offset"))
				assert("seguro que no tiene offset?");

			_graphicsEntities[current].offset = entityInfo->getVector3Attribute(weapon+"Offset");
				
			
			// Ahora voy a crear los overlays por cada arma en 3D

			Graphics::CServer *server = Graphics::CServer::getSingletonPtr();

			_overlayWeapon3D[current] = server->createOverlay( "_overlay3D"+currentOnText, _scene );
			std::string modelWeapon = entityInfo->getStringAttribute("weapon"+currentOnText+"Model");			
			
			_graphicsEntities[current].graphicsEntity = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon,_graphicsEntities[current].offset);
			assert(_graphicsEntities[current].graphicsEntity != 0 && "error al cargar la entidad grafica");
			//_weaponsEntities[current] = _overlayWeapon3D[current]->add3D(currentOnText, modelWeapon, &offsetPositionWeapon);

			
			Matrix4 transformModificado = _graphicsEntities[current].graphicsEntity->getTransform();
			Math::pitchYawRoll(_graphicsEntities[current].pitch, _graphicsEntities[current].yaw, _graphicsEntities[current].roll, transformModificado);

			
			_graphicsEntities[current].graphicsEntity->setTransform(transformModificado);
			//_graphicsEntities[current].graphicsEntity->setPosition(_graphicsEntities[current].offset);
			

			_overlayWeapon3D[current]->setVisible(false);

			
		}
		_overlayWeapon3D[WeaponType::eHAMMER]->setVisible(true);
		if(!_graphicsEntities)
			return false;
		
		return true;

	} // spawn

	void CHudWeapons::onStart() {
		for(int i = 0; i < WeaponType::eSIZE; ++i) {
			_graphicsEntities[i].defaultPos = _graphicsEntities[i].graphicsEntity->getTransform().getTrans();
			_graphicsEntities[i].offset = Vector3::ZERO;
		}
	}
	
	//---------------------------------------------------------

	bool CHudWeapons::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_TRANSFORM				||
			   msgType == Message::CHANGE_WEAPON_GRAPHICS;
	} // accept
	
	//---------------------------------------------------------

	void CHudWeapons::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON_GRAPHICS: {
				std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnMsg = std::static_pointer_cast<CMessageChangeWeaponGraphics>(message);
				changeWeapon( chgWpnMsg->getWeapon() );
				break;
			}
		}
	} // process
	//---------------------------------------------------------

	void CHudWeapons::changeWeapon(int newWeapon) {
		_overlayWeapon3D[_currentWeapon]->setVisible(false);
		_overlayWeapon3D[newWeapon]->setVisible(true);
		_currentWeapon = newWeapon;
	}

	//---------------------------------------------------------

	void CHudWeapons::onFixedTick(unsigned int msecs) {
		if(_playerIsLanding)
			landAnim(msecs);
		else if(_playerIsWalking)
			walkAnim(msecs);
	}

	//---------------------------------------------------------

	void CHudWeapons::landAnim(unsigned int msecs) {
		_currentLandOffset += _landRecoverySpeed * msecs;
		_offset.y = sin(_currentLandOffset) * _landForce;
		if(_offset.y >= 0.0f) {
			_offset.y = _currentLandOffset = _landForce = 0;
			_playerIsLanding = false;
		}
		
		_graphicsEntities[_currentWeapon].graphicsEntity->setPosition(_graphicsEntities[_currentWeapon].defaultPos + _offset);
	}

	//---------------------------------------------------------

	void CHudWeapons::offsetRecovery(unsigned int msecs) {
		_offset *= Vector3(0.85f, 0.85f, 0.85f);
		_runAnim.currentHorizontalPos *= 0.85f;
		_runAnim.currentVerticalPos *= 2 * 0.85f;
		_graphicsEntities[_currentWeapon].graphicsEntity->setPosition(_graphicsEntities[_currentWeapon].defaultPos + _offset);
	}

	//---------------------------------------------------------

	void CHudWeapons::playerIsWalking(bool walking, int direction) { 
		_playerIsWalking = walking;
		if(_playerIsWalking) {
			//_walkAnim.currentStrafingDir = _strafingDir;
			//_strafingDir = direction;
		}
		else {
			//_strafingDir = 0;
		}
	}

	//---------------------------------------------------------

	void CHudWeapons::playerIsLanding(float hitForce, float estimatedLandingTime) {
		_playerIsLanding = true;
		_landForce = hitForce * 0.2f;
		_landRecoverySpeed = Math::PI / estimatedLandingTime;
	}

	//---------------------------------------------------------

	void CHudWeapons::walkAnim(unsigned int msecs) {
		// Obtenemos la posicion del arma
		Matrix4 weaponTransform = _graphicsEntities[_currentWeapon].graphicsEntity->getTransform();
		Math::yaw(Math::HALF_PI, weaponTransform);
		_offset = Math::getDirection(weaponTransform);

		_runAnim.currentHorizontalPos += _runAnim.horizontalSpeed * msecs;
		if(_runAnim.currentHorizontalPos > ((2 * Math::PI) + Math::HALF_PI)) _runAnim.currentHorizontalPos = Math::HALF_PI;

		// Multiplicamos el vector horizontal normalizado por el desplazamiento y lo sumamos al offset
		float horizontalFactor = sin(_runAnim.currentHorizontalPos) * _runAnim.horizontalOffset;
		_offset *= horizontalFactor * Vector3(1.0f, 0.0f, 1.0f);

		_runAnim.currentVerticalPos += _runAnim.verticalSpeed * msecs;
		if(_runAnim.currentVerticalPos > ((2 * Math::PI) + Math::HALF_PI)) _runAnim.currentVerticalPos = Math::HALF_PI; 

		_offset.y = sin(_runAnim.currentVerticalPos) * _runAnim.verticalOffset;

		_graphicsEntities[_currentWeapon].graphicsEntity->setPosition(_graphicsEntities[_currentWeapon].defaultPos + _offset);
	}
	//---------------------------------------------------------

} // namespace Logic

