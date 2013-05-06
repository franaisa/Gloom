/**
@file CameraFeedbackNotifier.cpp

Contiene la implementación del componente que 
controla los daños recibidos por el escudo
del Screamer.
 
@see Logic::CCameraFeedbackNotifier
@see Logic::IComponent

@author Jaime Chapinal Cervantes
@date Abril, 2013
*/

#include "Graphics/Scene.h"
#include "CameraFeedbackNotifier.h"
#include "Camera.h"
#include "AvatarController.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

// Mensajes
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageSetReducedDamage.h"
#include "Logic/Messages/MessageCameraOffset.h"
#include "Logic/Messages/MessageControl.h"





namespace Logic {
	
	IMP_FACTORY(CCameraFeedbackNotifier);

	//________________________________________________________________________

	CCameraFeedbackNotifier::CCameraFeedbackNotifier() : _owner(NULL),
														 _playerIsWalking(false),
														 _playerIsLanding(false),
														 _landForce(0),
														 _landRecoverySpeed(0.007f),
														 _currentLandOffset(0),
														 _walkingRollSpeed(0.008f),
														 _walkingRollOffset(0.003f),
														 _currentWalkingRoll(0) {

		// Nada que hacer
	}

	//________________________________________________________________________

	CCameraFeedbackNotifier::~CCameraFeedbackNotifier() {
		// Nada que borrar
	}

	//________________________________________________________________________

	bool CCameraFeedbackNotifier::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) )
			return false;

		assert( entityInfo->hasAttribute("maxVelocity") && "Error: No se ha definido el atributo maxVelocity en el mapa" );
		_maxVelocity = entityInfo->getFloatAttribute("maxVelocity");
		

		return true;
	} // spawn
	
	//________________________________________________________________________

	bool CCameraFeedbackNotifier::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::DAMAGED				|| 
			   msgType == Message::SET_REDUCED_DAMAGE;
	} // accept
	
	//________________________________________________________________________

	void CCameraFeedbackNotifier::process(const std::shared_ptr<CMessage>& message) {
		//Ambas hacen lo mismo de momento, pero lo dejo separado por si luego queremos poner 
		//comportamientos distintos en función del daño
		switch( message->getMessageType() ) {
			case Message::DAMAGED: {
				damaged();
				break;
			}
			/*case Message::SET_REDUCED_DAMAGE: {
				damaged();
				break;
			}*/
		}
	} // process

	//________________________________________________________________________

	void CCameraFeedbackNotifier::onStart() {
		Logic::CEntity* cameraEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
		assert(cameraEntity != NULL && "Error: No existe una entidad camara");
		_cameraComponent = cameraEntity->getComponent<CCamera>("CCamera");
		assert(_cameraComponent != NULL && "Error: La entidad camara no tiene un componente de camara");
		
		_avatarc = _entity->getComponent<CAvatarController>("CAvatarController");
		assert(_avatarc != NULL && "Error: no tenemos avatar controller lol");

		_scene = _entity->getMap()->getScene();
		_effect = "damageCompositor";
		_motionblur = "Motion Blur";
		_strengthEffect = "strength";
		_effectIsActivated = false;
		_scene->createCompositor(_effect);
		_scene->setCompositorVisible(_effect, false);
		// Por ahora esta a hierro, lo suyo es ponerlo por el mapa
		_scene->updateCompositorVariable(_effect, _strengthEffect, 1);

		_scene->createCompositor(_motionblur);
		_scene->setCompositorVisible(_motionblur, true);
		// Por ahora esta a hierro, lo suyo es ponerlo por el mapa
		_scene->updateCompositorVariable(_motionblur, "blur", 0.75);
		
		
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::onFixedTick(unsigned int msecs) {
		if(_playerIsWalking) {
			walkEffect(msecs);
		}
		else if(_currentWalkingRoll != 0) {
			walkEffect(msecs);
		}
		
		if(_effectIsActivated){
			_scene->updateCompositorVariable(_effect, _strengthEffect, 1+_timestamp*0.01);
			_timestamp += msecs;
			if(_timestamp > 500){
				_effectIsActivated = false;
				_scene->setCompositorVisible(_effect, false);
			}
		}

		if(_playerIsLanding) {
			landEffect(msecs);
		}	

		//Ahora actualizamos el motion blur
		float blur = (_avatarc->getVelocity().length()/_maxVelocity)/2;

		if(blur>0.5)
			blur=0.5f;

		_scene->updateCompositorVariable(_motionblur, "blur", blur);
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::walkEffect(unsigned int msecs) {
		_currentWalkingRoll += _walkingRollSpeed * msecs;
		if(_currentWalkingRoll > 2 * Math::PI) _currentWalkingRoll = 0;

		_cameraComponent->rollCamera(sin(_currentWalkingRoll) * _walkingRollOffset);
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::playerIsTouchingGround(float hitForce) {
		if(hitForce < -0.3f) {
			_playerIsLanding = true;
			_landForce = abs(hitForce);
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::landEffect(unsigned int msecs) {
		_currentLandOffset += _landRecoverySpeed * msecs;
		float vOffset = sin(_currentLandOffset + Math::PI) * _landForce;
		if(vOffset >= 0.0f) {
			_currentLandOffset =  _landForce = 0;
			_cameraComponent->setVerticalOffset(0);
			_playerIsLanding = false;
		}
		else {
			_cameraComponent->setVerticalOffset( sin(_currentLandOffset + Math::PI) * _landForce);
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::damaged() {
		std::shared_ptr<Logic::CMessageCameraOffset> m3 = std::make_shared<Logic::CMessageCameraOffset>();
		m3->setOffsetTimer(100.0f);//Timer								 
		Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
		camera->emitMessage(m3);

		_scene->setCompositorVisible(_effect, true);
		_effectIsActivated = true;
		_timestamp = 0;

		calculateEnemyPosition();
	}// damaged

	//________________________________________________________________________

	void CCameraFeedbackNotifier::setOwner(Logic::CEntity* owner) { 
		this->_owner = owner; 
	}

	//________________________________________________________________________

	/** Cuadrantes:  
	      +z 90º				
	  	 2  |  1			Los ángulos se miden en sentido contrario al sentido del reloj.
  180º -z___|___ +x 0º
			|
		 3  |  4
		  -z 270º
	*/
	//SIN ACABAR
	void CCameraFeedbackNotifier::calculateEnemyPosition() { 
		//Obtengo la posición del enemigo
		Ogre::Vector3 vEnemyPos;
		//Obtengo mi posición (entidad a la que han dañado)
		Ogre::Vector3 vMyPos;

		//Obtengo desde qué cuadrante me ha disparado
		short sCuadrante; 
		if ((vEnemyPos.x > vMyPos.x) && (vEnemyPos.z > vMyPos.z))
			sCuadrante = 1;
		else if ((vEnemyPos.x > vMyPos.x) && (vEnemyPos.z < vMyPos.z))
			sCuadrante = 4;
		else if ((vEnemyPos.x < vMyPos.x) && (vEnemyPos.z > vMyPos.z))
			sCuadrante = 2;
		else if ((vEnemyPos.x < vMyPos.x) && (vEnemyPos.z < vMyPos.z))
			sCuadrante = 3;

		//En función del cuadrante, hallo el ángulo de disparo
		float fAngulo; //Angulo en radianes
		float fCateto, fHipotenusa;

		fHipotenusa = (vEnemyPos - vMyPos).length();
		fCateto = vEnemyPos.z - vMyPos.z;
		fHipotenusa = (vEnemyPos - vMyPos).length(); //En radianes
		fAngulo = asin(fCateto / fHipotenusa);

		//switch de cuadrante para poner el signo del cateto y para sumar ángulos en función del cuadrante
		switch (sCuadrante)
		{
			/*
			case 1:
				//Ya hecho antes del switch
			break;
			*/
			case 2:
				fAngulo += Math::HALF_PI;
			break;
			case 3:
				fAngulo = asin(-fCateto / fHipotenusa);
				fAngulo += Math::PI;				
			break;
			case 4:
				fAngulo = asin(-fCateto / fHipotenusa);
				fAngulo += (Math::PI + Math::HALF_PI);
			break;
		}

		//Trabajar con el fAngulo para poner la flecha en el hud, o darle efecto al compositor

	}

	//________________________________________________________________________

} // namespace Logic


