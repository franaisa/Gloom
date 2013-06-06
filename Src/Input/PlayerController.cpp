/**
@file PlayerController.cpp

Contiene la implementación de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author Jaime Chapinal Cervantes
@date Abril, 2013
*/

#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageHudDebugData.h"

#include "Logic/Messages/MessageChangeWeapon.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

//#include "Logic/Messages/MessageCameraOffset.h"
//#include "Logic/Server.h"
//#include "Logic/Maps/Map.h"

//#include "Logic/Messages/MessageCameraRoll.h"

#include <cassert>
#include <memory>

//PRUEBAS,QUITAR LUEGO LOS DOS INCLUDE SIGUIENTES
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"

//MENSAJE PARA DEBBUG
#include "Logic/Messages/MessageHudDebug.h"

#define TURN_FACTOR_X 0.001f
#define TURN_FACTOR_Y 0.001f

#define ROLL_DEGREES 0.8f

namespace Input {

	CPlayerController::CPlayerController() : _controlledAvatar(0), m_iLastTime(0), m_eLastMove(NONE)
	{

	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
	} // deactivate

	//--------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key)
	{						
		if(_controlledAvatar)
		{
			int iType = typeOfKey(key);
			switch (iType)
			{
				case 0: //Change weapons
					ChangeWeaponMessage(key);
				break;
				case 1: //Movement
					MovementMessage(key);
				case 2: //Habilities
					HabilityMessage(key);
				break;
				case 3: //Other (debug)
					OtherMessages(key);
					return true;
				break;				
			}//switch iType
		}//if _controlledAvatar
		return true;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageHudDebugData> hud3 = std::make_shared<Logic::CMessageHudDebugData>();
			hud3->setKey("ultimatecla");
			hud3->setValue("keyreleased");
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(key.keyId)
			{
			case Input::Key::W:
				m->setType(Logic::Control::STOP_WALK);
 				break;
			case Input::Key::S:
				m->setType(Logic::Control::STOP_WALKBACK);
				break;
			case Input::Key::A:
			{
				m->setType(Logic::Control::STOP_STRAFE_LEFT);
				/*std::cout << "StopStrafeLeft" << std::endl;
				//emitir mensaje de roll
				std::shared_ptr<Logic::CMessageCameraRoll> messageRoll = std::make_shared<Logic::CMessageCameraRoll>();
				messageRoll->setRollDegrees(ROLL_DEGREES);//Roll para que vuelva a posición original
				Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(messageRoll);*/
				break;
			}
			case Input::Key::D:
			{
				m->setType(Logic::Control::STOP_STRAFE_RIGHT);
				/*
				std::cout << "StopStrafeRight" << std::endl;
				std::shared_ptr<Logic::CMessageCameraRoll> messageRoll = std::make_shared<Logic::CMessageCameraRoll>();
				messageRoll->setRollDegrees(-ROLL_DEGREES); //Roll para que vuelva a posición original
				Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(messageRoll);	*/			
				break;
			}
			case Input::Key::SPACE:
				m->setType(Logic::Control::STOP_JUMP);
				break;
			case Input::Key::Z:
				m->setType(Logic::Control::STOP_CROUCH);
				break;
			case Input::Key::Q:
				m->setType(Logic::Control::STOP_PRIMARY_SKILL);
				break;
			case Input::Key::E:
				m->setType(Logic::Control::STOP_SECONDARY_SKILL);
				break;
			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);

			return true;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------

	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageMouse> m = std::make_shared<Logic::CMessageMouse>();
			m->setType(Logic::Control::MOUSE);
			float mouse[]={-(float)mouseState.movX * TURN_FACTOR_X,-(float)mouseState.movY * TURN_FACTOR_Y};
			m->setMouse(mouse);
			_controlledAvatar->emitMessage(m);

			if (mouseState.posAbsZ != 0) //Ha movido el scroll del mouse
				ScrollWheelChangeWeapon(mouseState);
			
			return true;
		}
		return true;

	} // mouseMoved

	//--------------------------------------------------------

	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(mouseState.button)
			{
			case Input::Button::LEFT:
				m->setType(Logic::Control::LEFT_CLICK);
				break;
			case Input::Button::RIGHT:
				//m->setType(Logic::Control::RIGHT_CLICK);
				m->setType(Logic::Control::RIGHT_CLICK);
				break;
			case Input::Button::MIDDLE:
				//m->setType(Logic::Control::MIDDLE_CLICK);
				m->setType(Logic::Control::USE_SECONDARY_SKILL);
				break;
			case Input::Button::BUTTON_3:
				m->setType(Logic::Control::BUTTON3_CLICK);
				break;
			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return true;

	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			int a = mouseState.scrool;
			std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
			switch(mouseState.button)
			{
			case Input::Button::LEFT:
				m->setType(Logic::Control::UNLEFT_CLICK);
				break;
			case Input::Button::RIGHT:
				//m->setType(Logic::Control::UNRIGHT_CLICK);
				m->setType(Logic::Control::UNRIGHT_CLICK);
				break;
			case Input::Button::MIDDLE:
				//m->setType(Logic::Control::UNMIDDLE_CLICK);
				m->setType(Logic::Control::STOP_SECONDARY_SKILL);
				break;
			case Input::Button::BUTTON_3:
				m->setType(Logic::Control::UNBUTTON3_CLICK);
				break;

			default:
				return true;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return true;

	} // mouseReleased

	//--------------------------------------------------------

	int CPlayerController::typeOfKey(TKey key)
	{
		if ( key.keyId == Input::Key::NUMBER1 || key.keyId == Input::Key::NUMBER2 || key.keyId == Input::Key::NUMBER3 || key.keyId == Input::Key::NUMBER4 || 
				key.keyId == Input::Key::NUMBER5)
		{		
			return 0;//Change weapon
		}
		else if (key.keyId == Input::Key::W || key.keyId == Input::Key::A || key.keyId == Input::Key::S || key.keyId == Input::Key::D 
			|| key.keyId == Input::Key::SPACE || key.keyId == Input::Key::Z )
		{
			return 1; //Movement
		}
		else if (key.keyId == Input::Key::Q || key.keyId == Input::Key::E)
		{
			return 2; //Habilities
		}
		else if (key.keyId == Input::Key::O || key.keyId == Input::Key::NUMBER7 
			|| key.keyId == Input::Key::NUMBER8 || key.keyId == Input::Key::ESCAPE || key.keyId == Input::Key::T)
		{
			return 3; //Other
		}
		return -1;
	} // typeOfKey

	//--------------------------------------------------------

	void CPlayerController::ScrollWheelChangeWeapon(const CMouseState &mouseState)
	{
		int iScrollValue = -1;
		//Asigno 100 he movido la rueda para adelante y -100 si la he movido hacia atrás
		iScrollValue = (mouseState.posAbsZ > 0) ? 100 : -100; 
		ChangeWeaponMessage(iScrollValue);
		
	}

	//--------------------------------------------------------

	void CPlayerController::ChangeWeaponMessage(TKey key)
	{
		std::shared_ptr<Logic::CMessageChangeWeapon> message = std::make_shared<Logic::CMessageChangeWeapon>();

		switch(key.keyId)
		{
		case Input::Key::NUMBER1:
			message->setWeapon(0);
			break;
		case Input::Key::NUMBER2:
			message->setWeapon(1);
			break;
		case Input::Key::NUMBER3:
			message->setWeapon(2);
			break;
		case Input::Key::NUMBER4:
			message->setWeapon(3);
			break;
		case Input::Key::NUMBER5:
			message->setWeapon(4);
			break;
		}

		_controlledAvatar->emitMessage(message);
	}//EmitMessageChangeWeapon

	//--------------------------------------------------------
	//Método sobrecargado para cambiar el arma con un entero (y no con una tecla TKey)
	void CPlayerController::ChangeWeaponMessage(int iWeapon)
	{
		std::shared_ptr<Logic::CMessageChangeWeapon> message = std::make_shared<Logic::CMessageChangeWeapon>();
		message->setWeapon(iWeapon);
		_controlledAvatar->emitMessage(message);

	}//EmitMessageChangeWeapon

	//--------------------------------------------------------

	void CPlayerController::MovementMessage(TKey key)
	{
		std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
		unsigned int iDiffTime = clock() - m_iLastTime;
		switch(key.keyId)
		{
			case Input::Key::W:
				if ((iDiffTime < MAX_TIME_DOBULE_PUSH)	&& (m_eLastMove == WALK))	
				{
					m->setType(Logic::Control::DODGE_FORWARD);
				}
				else
				{
					m->setType(Logic::Control::WALK);
				}
				m_iLastTime=clock();
				m_eLastMove = WALK;
				break;
			case Input::Key::S:
				if ((iDiffTime < MAX_TIME_DOBULE_PUSH)	&& (m_eLastMove == WALKBACK))	
				{
					m->setType(Logic::Control::DODGE_BACKWARDS);
				}
				else
				{
					m->setType(Logic::Control::WALKBACK);				
				}
				m_iLastTime=clock();
				m_eLastMove = WALKBACK;
				break;
			case Input::Key::A:
				if ((iDiffTime < MAX_TIME_DOBULE_PUSH)	&& (m_eLastMove == LEFT))	
				{
					m->setType(Logic::Control::SIDEJUMP_LEFT);
				}
				else
				{
					m->setType(Logic::Control::STRAFE_LEFT);
					//emitir mensaje de roll
					/*std::shared_ptr<Logic::CMessageCameraRoll> messageRoll = std::make_shared<Logic::CMessageCameraRoll>();
					messageRoll->setRollDegrees(-ROLL_DEGREES);
					Logic::CEntity *camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
					camera->emitMessage(messageRoll);*/
				}
				m_eLastMove = LEFT;
				m_iLastTime=clock();
				break;
			case Input::Key::D:
				if ((iDiffTime < MAX_TIME_DOBULE_PUSH) && (m_eLastMove == RIGHT))
				{
					m->setType(Logic::Control::SIDEJUMP_RIGHT);
				}
				else
				{
					m->setType(Logic::Control::STRAFE_RIGHT);
				}
				m_iLastTime=clock();
				m_eLastMove = RIGHT;
				break;
			case Input::Key::SPACE:
				m->setType(Logic::Control::JUMP);
				break;
			case Input::Key::Z:
				m->setType(Logic::Control::CROUCH);
				break;
		}
		std::shared_ptr<Logic::CMessageHudDebugData> hud2 = std::make_shared<Logic::CMessageHudDebugData>();
		hud2->setKey("lastkey-pcontroller");
		hud2->setValue(m->getType());
		_controlledAvatar->emitMessage(hud2);
		_controlledAvatar->emitMessage(m);
	}//EmitMessageMovement

	//--------------------------------------------------------

	void CPlayerController::HabilityMessage(TKey key)
	{
		std::shared_ptr<Logic::CMessageControl> m = std::make_shared<Logic::CMessageControl>();
		switch (key.keyId)
		{
			case Input::Key::Q:
				m->setType(Logic::Control::USE_PRIMARY_SKILL);
			break;
			case Input	::Key::E:
				m->setType(Logic::Control::USE_SECONDARY_SKILL);
			break;
		}
		_controlledAvatar->emitMessage(m);
	}//HabilityMessage

	//--------------------------------------------------------

	void CPlayerController::OtherMessages(TKey key)
	{
		switch (key.keyId)
		{					
			case Input::Key::NUMBER7:	
			{
				std::shared_ptr<Logic::CMessageAddLife> message2 = std::make_shared<Logic::CMessageAddLife>();
				message2->setAddLife(20);
				_controlledAvatar->emitMessage(message2);
				std::cout << "Mi posicion actual es : " << _controlledAvatar->getPosition() << std::endl;
			}
			break;
			case Input::Key::NUMBER8:
			{
				std::shared_ptr<Logic::CMessageAddShield> message3 = std::make_shared<Logic::CMessageAddShield>();
				message3->setAddShield(20);
				_controlledAvatar->emitMessage(message3);
			}
			break;
			case Input::Key::O:
			{
				std::shared_ptr<Logic::CMessageHudDebug> m2 = std::make_shared<Logic::CMessageHudDebug>();
				_controlledAvatar->emitMessage(m2);
			}
			break;
			case Input::Key::T:
			{
				/*std::shared_ptr<Logic::CMessageCameraOffset> m3 = std::make_shared<Logic::CMessageCameraOffset>();
				m3->setOffsetTimer(100.0f);//Timer								 
				Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(m3);*/

				/*std::shared_ptr<Logic::CMessageCameraRoll> m3 = std::make_shared<Logic::CMessageCameraRoll>();
				m3->setRollDegrees(ROLL_DEGREES);//Grados								 
				Logic::CEntity * camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera");
				camera->emitMessage(m3);*/
			}
			break;
			case Input::Key::ESCAPE:// esto debe desaparecer en el futuro
				std::cout << "escape pulsado" << std::endl;
				//return true;
			break;
		}

	}//EmitOtherMessages
} // namespace Input
