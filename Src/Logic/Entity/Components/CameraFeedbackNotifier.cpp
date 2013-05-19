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
#include "Logic/Messages/MessageImpact.h"
#include "Logic/Messages/MessageHudDebugData.h"




namespace Logic {
	
	IMP_FACTORY(CCameraFeedbackNotifier);

	//________________________________________________________________________

	CCameraFeedbackNotifier::CCameraFeedbackNotifier() : _owner(NULL),
														 _playerIsWalking(false),
														 _playerIsLanding(false),
														 _landForce(0),
														 _strafingDir(0),
														 _landRecoverySpeed(0.007f),
														 _currentLandOffset(0) {


		_walkAnim.currentHorizontalPos = Math::HALF_PI;
		_walkAnim.horizontalSpeed = 0.0075f;
		_walkAnim.horizontalOffset = 0.06f;

		_walkAnim.currentVerticalPos = Math::HALF_PI;
		_walkAnim.verticalSpeed = _walkAnim.horizontalSpeed * 2;
		_walkAnim.verticalOffset = 0.06f;

		_walkAnim.currentRoll = 0.0f;
		_walkAnim.rollSpeed = 0.00005f;
		_walkAnim.rollOffset = 0.012f;
		_walkAnim.rollCoef = 0.95f;
		_walkAnim.recoveringRoll = false;
		_walkAnim.currentStrafingDir = 0;
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
				std::shared_ptr<CMessageDamaged> damageMess = std::static_pointer_cast<CMessageDamaged>(message);
				damaged(damageMess->getEnemy()->getPosition());
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
		Logic::CEntity* cameraEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
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
		if(_playerIsLanding) 
			landEffect(msecs);
		else if(_playerIsWalking && !_playerIsSideColliding)
			walkEffect(msecs);
		else
			offsetRecovery(msecs);

		if(_effectIsActivated){
			_scene->updateCompositorVariable(_effect, _strengthEffect, 1+_timestamp*0.01);
			_timestamp += msecs;
			if(_timestamp > 500){
				_effectIsActivated = false;
				_scene->setCompositorVisible(_effect, false);
			}
		}

		//Ahora actualizamos el motion blur
		float blur = (_avatarc->getVelocity().length()/_maxVelocity)/2;

		if(blur>0.5)
			blur=0.5f;

		_scene->updateCompositorVariable(_motionblur, "blur", blur);
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::playerIsWalking(bool walking, int direction) { 
		_playerIsWalking = walking;
		if(_playerIsWalking) {
			_walkAnim.currentStrafingDir = _strafingDir;
			_strafingDir = direction;
		}
		else {
			_strafingDir = 0;
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::offsetRecovery(unsigned int msecs) {
		// Queda mas suave sin correccion
		_cameraComponent->setOffset( _cameraComponent->getOffset() * Vector3(0.85f, 0.95f, 0.85f) );
		if(_walkAnim.currentRoll != 0) {
			_walkAnim.currentRoll *= 0.85f; // Coeficiente un poco más fuerte
			_cameraComponent->rollCamera(_walkAnim.currentRoll);
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::walkEffect(unsigned int msecs) {
		Vector3 offset = _cameraComponent->getOffset();

		if(_strafingDir == 0) {
			Matrix4 transform = _entity->getTransform();
			Math::yaw(Math::HALF_PI, transform);
			Vector3 horizontal = Math::getDirection(transform);

			_walkAnim.currentHorizontalPos += _walkAnim.horizontalSpeed * msecs;
			if(_walkAnim.currentHorizontalPos > ((2 * Math::PI) + Math::HALF_PI)) _walkAnim.currentHorizontalPos = Math::HALF_PI;

			// Multiplicamos el vector horizontal normalizado por el desplazamiento y lo sumamos al offset
			horizontal *= sin(_walkAnim.currentHorizontalPos) * _walkAnim.horizontalOffset;
			offset += horizontal;
		}
		else {
			// Reducimos el offset horizontal
			// Queda mas suave sin correccion
			offset = offset * Vector3(0.95f, 1.0f, 0.95f);

			// Si no estamos recuperando el offset por haber llegado al máximo
			// incrementamos el roll
			if(!_walkAnim.recoveringRoll) {
				_walkAnim.currentRoll += _strafingDir * _walkAnim.rollSpeed * msecs;

				if(abs(_walkAnim.currentRoll) >= _walkAnim.rollOffset) {
					_walkAnim.currentRoll = _walkAnim.rollOffset * _walkAnim.rollCoef * _strafingDir;
					_walkAnim.recoveringRoll = true;
				}
			}
			// Si cambiamos de direccion dejamos de recuperar el offset
			else if(_walkAnim.currentStrafingDir != _strafingDir) {
				_walkAnim.recoveringRoll = false;
			}
		}

		// Recuperamos la posicion inicial multiplicando por el coeficiente
		// de recuperacion
		if(_walkAnim.recoveringRoll)
			_walkAnim.currentRoll *= _walkAnim.rollCoef;
		
		_cameraComponent->rollCamera(_walkAnim.currentRoll);

		_walkAnim.currentVerticalPos += _walkAnim.verticalSpeed * msecs;
		if(_walkAnim.currentVerticalPos > ((2 * Math::PI) + Math::HALF_PI)) _walkAnim.currentVerticalPos = Math::HALF_PI; 

		offset.y += sin(_walkAnim.currentVerticalPos) * _walkAnim.verticalOffset;
		_cameraComponent->setOffset(offset);
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::playerIsTouchingGround(float hitForce) {
		if(hitForce < -0.3f) {
			_playerIsLanding = true;
			_landForce = hitForce * 0.6;
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::playerIsSideColliding(bool colliding, float force) {
		_playerIsSideColliding = colliding;
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::landEffect(unsigned int msecs) {
		_currentLandOffset += _landRecoverySpeed * msecs;
		float vOffset = sin(_currentLandOffset) * _landForce;
		if(vOffset >= 0.0f) {
			_currentLandOffset =  _landForce = 0;
			_cameraComponent->setVerticalOffset(0);
			_playerIsLanding = false;
		}
		else {
			_cameraComponent->setVerticalOffset(vOffset);
		}
	}

	//________________________________________________________________________

	void CCameraFeedbackNotifier::damaged(Vector3 vEnemyPosition) {
		std::shared_ptr<Logic::CMessageCameraOffset> m3 = std::make_shared<Logic::CMessageCameraOffset>();
		m3->setOffsetTimer(100.0f);//Timer								 
		Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
		assert(camera != NULL && "Error: Esto no se puede hacer asi que sois unos lamers, ahora el servidor que hace?");
		camera->emitMessage(m3);

		_scene->setCompositorVisible(_effect, true);
		_effectIsActivated = true;
		_timestamp = 0;

		calculateEnemyPosition(vEnemyPosition);
	}// damaged

	//________________________________________________________________________

	void CCameraFeedbackNotifier::setOwner(Logic::CEntity* owner) { 
		this->_owner = owner; 
	}

	//________________________________________________________________________


	//SIN ACABAR
	void CCameraFeedbackNotifier::calculateEnemyPosition(Vector3 vPosEnemy) { 
		//Obtengo la posición del enemigo
		Ogre::Vector3 vEnemyPos = vPosEnemy;
		//Obtengo mi posición (entidad a la que han dañado)
		Ogre::Vector3 vMyPos = this->_entity->getPosition();

		/*
		short sCuadrante; 	
		
		//Angulo en radianes
		float fAngulo; 
		if (vEnemyPos != vMyPos)
		{
			//Obtengo desde qué cuadrante me ha disparado
			if ((vEnemyPos.x >= vMyPos.x) && (vEnemyPos.z >= vMyPos.z))
				sCuadrante = 1;
			else if ((vEnemyPos.x > vMyPos.x) && (vEnemyPos.z < vMyPos.z))
				sCuadrante = 4;
			else if ((vEnemyPos.x < vMyPos.x) && (vEnemyPos.z > vMyPos.z))
				sCuadrante = 2;
			else if ((vEnemyPos.x < vMyPos.x) && (vEnemyPos.z < vMyPos.z))
				sCuadrante = 3;

			//En función del cuadrante, hallo el ángulo de disparo
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
				break;//*
				
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
		}
		else
			fAngulo = 0.0f;

		//Trabajar con el fAngulo para poner la flecha en el hud, o darle efecto al compositor
		//std::cout << "Angulo (en radianes) con el que ha impactado: " << fAngulo << std::endl;

		//std::shared_ptr<Logic::CMessageImpact> impact = std::make_shared<Logic::CMessageImpact>();
		//impact->setDirectionImpact(fAngulo);//Timer								 
		//_entity->emitMessage(impact);
		*/

		Vector3 vMyDirVision = Math::getDirection(_entity->getTransform());
		vMyDirVision = Vector3(vMyDirVision.x,0,vMyDirVision.z);
		Vector3 vEnemyDirVision = vPosEnemy - vMyPos;
		vEnemyDirVision = Vector3(vEnemyDirVision.x, 0, vEnemyDirVision.z);
		Ogre::Radian rad = vMyDirVision.angleBetween(vEnemyDirVision);
		float fRadianes = (float)rad.valueRadians();

		Matrix4 mat = _entity->getTransform().inverse();
		Vector3 vec = mat * vPosEnemy; //este verctor es la posicion del enemigo respecto a mi
		if (vec.x > 0) 
		{
			//El enemigo está a la derecha, así que tengo que multiplicar
			//por -1 para que se oriente bien la flecha de daño
			fRadianes *= -1.0f;
		}

		std::shared_ptr<Logic::CMessageImpact> impact = std::make_shared<Logic::CMessageImpact>();
		impact->setDirectionImpact(fRadianes);							 
		_entity->emitMessage(impact);

	}

	//________________________________________________________________________

} // namespace Logic


