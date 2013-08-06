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

// Logica
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Map/MapEntity.h"

// Graficos
#include "Graphics/Camera.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Graphics/Overlay.h"

// Mensajes
#include "Logic/Messages/MessageTransform.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageHudDebugData.h"

#include "Graphics/HHFXParticle.h"

#include <stdio.h>
#include <windows.h>


using namespace std;

namespace Logic {
	IMP_FACTORY(CHudWeapons);
	
	//________________________________________________________________________
	
	CHudWeapons::CHudWeapons() : _currentWeapon(0), 
								 _graphicsEntities(0),
								 _playerIsWalking(false),
								 _playerIsLanding(false),
								 _loadingWeapon(false),
								 _continouslyShooting(false),
								 _playerIsFalling(false) {

		// Valores de configuracion de la animacion de correr
		_runAnim.currentHorizontalPos = Math::HALF_PI;
		_runAnim.horizontalSpeed = 0.0075f;
		_runAnim.horizontalOffset = 0.055f;

		_runAnim.currentVerticalPos = Math::HALF_PI;
		_runAnim.verticalSpeed = 2 * _runAnim.horizontalSpeed;
		_runAnim.verticalOffset = 0.055f;

		_runAnim.currentStrafingDir = _runAnim.oldStrafingDir = 0;
		
		_runAnim.offset = Vector3::ZERO;

		// Valores de configuracion de la animacion de aterrizar
		_landAnim.force = 0.0f;
		_landAnim.currentOffset = 0.0f;
		_landAnim.recoverySpeed = 0.01f;
		_landAnim.offset = Vector3::ZERO;

		// Valores de configuración de la animación de idle
		_idleAnim.currentVerticalPos = 0.0f;
		_idleAnim.verticalSpeed = 0.003f;
		_idleAnim.verticalOffset = 0.04f;

		_idleAnim.offset = Vector3::ZERO;

		// Valores de configuración de la animación de disparo
		_shootAnim.shootRecoveryCoef = 0.96f;
		_shootAnim.offset = Vector3::ZERO;

		// Valores de configuración de la animación de carga del arma
		_unstableLoadAnim.currentVerticalPos = 0.0f;
		_unstableLoadAnim.currentVerticalSpeed = _unstableLoadAnim.initVerticalSpeed = 0.01f;
		_unstableLoadAnim.maxVerticalSpeed = 0.035f;
		//_unstableLoadAnim.speedInc = 0.000035f;
		
		_unstableLoadAnim.verticalOffset = 0.05f;

		_unstableLoadAnim.currentNoise = _unstableLoadAnim.initNoiseSpeed = 0.0f;
		_unstableLoadAnim.maxNoiseSpeed = 0.1f;

		_unstableLoadAnim.offset = Vector3::ZERO;

		// Valores de configuracion de la animacion de disparo rapido
		_rapidShootAnim.shakeOffset = 0.08f;
		_rapidShootAnim.recoveryCoef = 0.96f;

		_rapidShootAnim.currentVerticalPos = 0.0f;
		_rapidShootAnim.verticalOffset = 0.05f;
		_rapidShootAnim.verticalSpeed = 0.025f;

		_rapidShootAnim.offset = Vector3::ZERO;

		_rapidShootAnim.firingRate = 20;
		_rapidShootAnim.acumFiringTime = 0;

		// Valores de configuración de la animación de salto
		_fallAnim.currentHorizontalPos = 0.0f;
		_fallAnim.horizontalSpeed = 0.0002f;
		_fallAnim.horizontalOffset = 0.2f;
		_fallAnim.movementDir = 0;
		
		_fallAnim.offset = Vector3::ZERO;
		_halfPi= Quaternion(Ogre::Radian(Math::HALF_PI),Vector3::UNIT_Y);
	}

	//________________________________________________________________________

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

	//________________________________________________________________________
	
	void CHudWeapons::onActivate() {
		//Cuando activamos el componente solo tendremos visible el arma 0( arma melee)
		_currentWeapon = WeaponType::eSOUL_REAPER;
		_graphicsEntities[_currentWeapon].graphicsEntity->setVisible(true);
		_overlayWeapon3D[_currentWeapon]->setVisible(true);
	} // activate

	//________________________________________________________________________

	void CHudWeapons::onDeactivate() {
		//Cuando desactivamos el componente, desactivaremos el arma actual
		_overlayWeapon3D[_currentWeapon]->setVisible(false);
	} // deactivate

	//________________________________________________________________________

	bool CHudWeapons::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();
			
		_graphicsEntities = new TGraphicsWeapon[WeaponType::eSIZE];
		
		// Por ahora leo a mano cada una de las armas que tiene el usuario
	
		for(int i = WeaponType::eSOUL_REAPER; i < WeaponType::eSIZE; ++i){
				
			WeaponType::Enum current = (WeaponType::Enum)i;
			std::string currentOnText = WeaponType::toString(current);
		
			std::stringstream aux;
			aux << "weapon" << currentOnText;
			std::string weapon = aux.str();
				
			//_graphicsEntities[i]._graphicsEntity = createGraphicsEntity(weapon, entityInfo->getStringAttribute(weapon+"Model"));
			_graphicsEntities[current].defaultYaw = _graphicsEntities[current].defaultPitch = _graphicsEntities[current].defaultRoll = 0;
			if(entityInfo->hasAttribute(weapon+"ModelYaw"))
				_graphicsEntities[current].defaultYaw = entityInfo->getFloatAttribute(weapon+"ModelYaw");
			if(entityInfo->hasAttribute(weapon+"ModelPitch"))
				_graphicsEntities[current].defaultPitch = entityInfo->getFloatAttribute(weapon+"ModelPitch");
			if(entityInfo->hasAttribute(weapon+"ModelRoll"))
				_graphicsEntities[current].defaultPitch = entityInfo->getFloatAttribute(weapon+"ModelRoll");
				
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

			_graphicsEntities[current].graphicsEntity->setOrientation(Math::setQuaternion(_graphicsEntities[current].defaultYaw, _graphicsEntities[current].defaultPitch, _graphicsEntities[current].defaultRoll));
	

			_overlayWeapon3D[current]->setVisible(false);
			_overlayWeapon3D[current]->setZBuffer(15);
			
			
			
		}
		_overlayWeapon3D[WeaponType::eSOUL_REAPER]->setVisible(true);
		if(!_graphicsEntities)
			return false;
		
		// Usamos un pequeño truco para calcular a la velocidad a la que tiene que incrementar
		// el ruido de carga
		// Primero obtenemos el tiempo máximo de carga del Iron Hell Goat
		Map::CEntity* info = CEntityFactory::getSingletonPtr()->getInfo("Screamer");
		assert( info->hasAttribute("weaponironHellGoatPrimaryFireLoadTime") );

		// Una vez conocido el tiempo de carga, como sabemos que vamos a utilizar fixed ticks
		// de 16 msecs, calculamos cuantos ticks van a pasar (aproximadamente) hasta que se
		// tiene el arma cargada.
		unsigned int nbTicks = (info->getIntAttribute("weaponironHellGoatPrimaryFireLoadTime") * 1000) / 16;

		// Calculamos el incremento de la velocidad distribuyendola uniformemente entre los
		// ticks de carga
		_unstableLoadAnim.speedInc = (_unstableLoadAnim.maxVerticalSpeed - _unstableLoadAnim.initVerticalSpeed) /  (float)nbTicks;
		_unstableLoadAnim.noiseInc = (_unstableLoadAnim.maxNoiseSpeed - _unstableLoadAnim.initNoiseSpeed) / (float)nbTicks;

		return true;

	} // spawn

	//________________________________________________________________________

	void CHudWeapons::onStart() {
		Matrix4 weaponTransform;
		for(int i = 0; i < WeaponType::eSIZE; ++i) {
			_graphicsEntities[i].defaultPos = _graphicsEntities[i].graphicsEntity->getPosition();
		}
	}
	
	//________________________________________________________________________

	bool CHudWeapons::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::SET_TRANSFORM				||
			   msgType == Message::CHANGE_WEAPON_GRAPHICS;
	} // accept
	
	//________________________________________________________________________

	void CHudWeapons::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CHANGE_WEAPON_GRAPHICS: {
				std::shared_ptr<CMessageChangeWeaponGraphics> chgWpnMsg = std::static_pointer_cast<CMessageChangeWeaponGraphics>(message);
				changeWeapon( chgWpnMsg->getWeapon() );
				break;
			}
		}
	} // process
	
	//________________________________________________________________________

	void CHudWeapons::changeWeapon(int newWeapon) {
		_overlayWeapon3D[_currentWeapon]->setVisible(false);
		_overlayWeapon3D[newWeapon]->setVisible(true);
		_currentWeapon = newWeapon;
	}

	//________________________________________________________________________

	void CHudWeapons::onFixedTick(unsigned int msecs) {
		_graphicsEntities[_currentWeapon].graphicsEntity->setVisible(true);
		if(_playerIsLanding)
			landAnim(msecs);
		else if(_playerIsWalking)
			walkAnim(msecs);
		else if(_playerIsFalling) {
			fallAnim(msecs);
		}
		else
			idleAnim(msecs);

		if(_loadingWeapon)
			loadWeaponAnim(msecs);
		else {
			_unstableLoadAnim.currentVerticalPos *= 0.95f;
			_unstableLoadAnim.currentNoise *= 0.95f;
			_unstableLoadAnim.offset *= 0.95f;
		}

		if(_continouslyShooting) {
			rapidShootAnim(msecs);
		}
		else {
			_rapidShootAnim.offset *= _rapidShootAnim.recoveryCoef;
			_rapidShootAnim.currentVerticalPos *= _rapidShootAnim.recoveryCoef;
		}
		_graphicsEntities[_currentWeapon].graphicsEntity->setPosition( _graphicsEntities[_currentWeapon].defaultPos + 
																	   _runAnim.offset + 
																	   _landAnim.offset +
																	   _idleAnim.offset +
																	   _shootAnim.offset +
																	   _unstableLoadAnim.offset +
																	   _rapidShootAnim.offset +
																	   _fallAnim.offset );
		_shootAnim.offset *= _shootAnim.shootRecoveryCoef;

		if(!_playerIsFalling)
			_fallAnim.offset *= 0.96f;
	}

	//________________________________________________________________________

	void CHudWeapons::fallAnim(unsigned int msecs) {
		Vector3 horizontal = (_graphicsEntities[_currentWeapon].graphicsEntity->getOrientation() * _halfPi) * Vector3::NEGATIVE_UNIT_Z;
		horizontal.normalise();
		Vector3 maxOffset = _fallAnim.horizontalOffset * _fallAnim.movementDir * horizontal;

		horizontal *= _fallAnim.movementDir * _fallAnim.horizontalSpeed * msecs * Vector3(1.0f, 0.0f, 1.0f);
		_fallAnim.offset += horizontal;
		if(_fallAnim.offset.length() > _fallAnim.horizontalOffset) {
			_fallAnim.offset = maxOffset;
		}
	}

	//________________________________________________________________________

	void CHudWeapons::loadingWeapon(bool state) {
		_loadingWeapon = state;
		if(!_loadingWeapon) {
			_unstableLoadAnim.currentVerticalSpeed = _unstableLoadAnim.initVerticalSpeed;
		}
	}

	//________________________________________________________________________

	void CHudWeapons::loadWeaponAnim(unsigned int msecs) {
		// Calculamos el ruido horizontal
		_unstableLoadAnim.offset = (_graphicsEntities[_currentWeapon].graphicsEntity->getOrientation() * _halfPi) * Vector3::NEGATIVE_UNIT_Z;
		_unstableLoadAnim.offset.normalise();

		_unstableLoadAnim.offset *= Math::unifRand(0.0f, _unstableLoadAnim.currentNoise) * Vector3(1.0f, 0.0f, 1.0f);

		if(_unstableLoadAnim.currentNoise != _unstableLoadAnim.maxNoiseSpeed) {
			_unstableLoadAnim.currentNoise += _unstableLoadAnim.noiseInc;
			
			if(_unstableLoadAnim.currentNoise > _unstableLoadAnim.maxNoiseSpeed) {
				_unstableLoadAnim.currentNoise = _unstableLoadAnim.maxNoiseSpeed;
			}
		}
		
		// Calculamos la velocidad de movimiento vertical
		_unstableLoadAnim.offset.y = sineStep(msecs, _unstableLoadAnim.currentVerticalPos, 
			_unstableLoadAnim.verticalOffset, _unstableLoadAnim.currentVerticalSpeed);
		
		if(_unstableLoadAnim.currentVerticalSpeed != _unstableLoadAnim.maxVerticalSpeed) {
			_unstableLoadAnim.currentVerticalSpeed += _unstableLoadAnim.speedInc;
			
			if(_unstableLoadAnim.currentVerticalSpeed > _unstableLoadAnim.maxVerticalSpeed) {
				_unstableLoadAnim.currentVerticalSpeed = _unstableLoadAnim.maxVerticalSpeed;
			}
		}
	}

	//________________________________________________________________________

	void CHudWeapons::shootAnim(float force) {
		Vector3 weaponDir =  _graphicsEntities[_currentWeapon].graphicsEntity->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
		weaponDir.normalise();
		_shootAnim.offset = weaponDir * force * Vector3(1.0f, 0.0f, 1.0f);
	}

	//________________________________________________________________________

	void CHudWeapons::continouosShooting(bool state) {
		_continouslyShooting = state;
		if(!_continouslyShooting)
			_rapidShootAnim.acumFiringTime = 0;
	}

	//________________________________________________________________________

	void CHudWeapons::rapidShootAnim(unsigned int msecs) {
		Vector3 weaponDir = _graphicsEntities[_currentWeapon].graphicsEntity->getOrientation() * Vector3::NEGATIVE_UNIT_Z;
		weaponDir.normalise();

		_rapidShootAnim.acumFiringTime += msecs;
		if(_rapidShootAnim.acumFiringTime > _rapidShootAnim.firingRate) {
			_rapidShootAnim.offset = weaponDir * _rapidShootAnim.shakeOffset * Vector3(1.0f, 0.0f, 1.0f);
			_rapidShootAnim.shakeOffset *= -1.0f;

			_rapidShootAnim.acumFiringTime = 0;
		}

		_rapidShootAnim.offset.y = sineStep(msecs, _rapidShootAnim.currentVerticalPos, 
			_rapidShootAnim.verticalOffset, _rapidShootAnim.verticalSpeed);
	}

	//________________________________________________________________________

	float CHudWeapons::sineStep(unsigned int msecs, float& currentSinePosition, float offset, float speed, float loBound, float hiBound) {
		currentSinePosition += speed * msecs;
		if(currentSinePosition > hiBound) currentSinePosition = loBound;

		return sin(currentSinePosition) * offset;
	}

	//________________________________________________________________________

	void CHudWeapons::idleAnim(unsigned int msecs) {
		_idleAnim.offset.y = sineStep(msecs, _idleAnim.currentVerticalPos, _idleAnim.verticalOffset, _idleAnim.verticalSpeed);
	}

	//________________________________________________________________________

	void CHudWeapons::landAnim(unsigned int msecs) {
		_landAnim.currentOffset += _landAnim.recoverySpeed * msecs;
		_landAnim.offset.y = sin(_landAnim.currentOffset) * _landAnim.force;
		if(_landAnim.offset.y >= 0.0f) {
			_landAnim.offset.y = _landAnim.currentOffset = _landAnim.force = 0;
			_playerIsLanding = false;
		}
	}

	//________________________________________________________________________

	void CHudWeapons::offsetRecovery(unsigned int msecs) {
		_runAnim.offset *= Vector3(0.85f, 0.85f, 0.85f);
		_landAnim.offset *= Vector3(0.85f, 0.85f, 0.85f);
		_runAnim.currentHorizontalPos *= 0.85f;
		_runAnim.currentVerticalPos *= 2 * 0.85f;
		
	}

	//________________________________________________________________________

	void CHudWeapons::playerIsWalking(bool walking, int direction) { 
		_playerIsWalking = walking;
		if(_playerIsWalking) {
			_runAnim.oldStrafingDir = _runAnim.currentStrafingDir;
			_runAnim.currentStrafingDir = direction;
		}
		else {
			_runAnim.currentStrafingDir = 0;
		}
	}

	//________________________________________________________________________

	void CHudWeapons::playerIsLanding(float hitForce, float estimatedLandingTime) {
		_playerIsLanding = true;
		_landAnim.force = hitForce * 0.2f;
		_landAnim.recoverySpeed = Math::PI / estimatedLandingTime;
	}

	//________________________________________________________________________

	void CHudWeapons::playerIsFalling(bool falling, int direction) {
		_fallAnim.movementDir = (_playerIsFalling = falling) ? direction : 0;
	}

	//________________________________________________________________________

	void CHudWeapons::walkAnim(unsigned int msecs) {
		_runAnim.offset = (_graphicsEntities[_currentWeapon].graphicsEntity->getOrientation() * _halfPi) * Vector3::NEGATIVE_UNIT_Z;
		_runAnim.offset.normalise();

		_runAnim.offset *= sineStep(msecs, _runAnim.currentHorizontalPos, _runAnim.horizontalOffset, 
									_runAnim.horizontalSpeed, Math::HALF_PI, (2 * Math::PI) + Math::HALF_PI)
							* Vector3(1.0f, 0.0f, 1.0f);
		
		_runAnim.offset.y = sineStep(msecs, _runAnim.currentVerticalPos, _runAnim.verticalOffset, 
			_runAnim.verticalSpeed, Math::HALF_PI, (2 * Math::PI) + Math::HALF_PI);
	}

} // namespace Logic

