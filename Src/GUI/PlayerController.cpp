/**
@file PlayerController.cpp

Contiene la implementación de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/

#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Messages/Message.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/WeaponsManager.h"
#include "Logic/Entity/Components/ArrayGraphics.h"

#include "Logic/Messages/MessageChangeWeapon.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageMouse.h"

#include <cassert>

//PRUEBAS,QUITAR LUEGO LOS DOS INCLUDE SIGUIENTES
#include "Logic/Messages/MessageAddLife.h"
#include "Logic/Messages/MessageAddShield.h"


#define TURN_FACTOR_X 0.001f
#define TURN_FACTOR_Y 0.001f

namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0)
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
			if(key.keyId == GUI::Key::NUMBER1 || key.keyId == GUI::Key::NUMBER2 || key.keyId == GUI::Key::NUMBER3 || key.keyId == GUI::Key::NUMBER4 || 
				key.keyId == GUI::Key::NUMBER5 || key.keyId == GUI::Key::NUMBER6
				|| key.keyId == GUI::Key::NUMBER7 || key.keyId == GUI::Key::NUMBER8){
				
					Logic::CMessageChangeWeapon *message=new Logic::CMessageChangeWeapon();

					//A quitar en un futuro, usado ahora para debuguear
					Logic::CMessageAddLife *message2=new Logic::CMessageAddLife();
					Logic::CMessageAddShield *message3=new Logic::CMessageAddShield();
					switch(key.keyId)
					{
					case GUI::Key::NUMBER1:
						message->setWeapon(0);
						break;
					case GUI::Key::NUMBER2:
						message->setWeapon(1);
						break;
					case GUI::Key::NUMBER3:
						message->setWeapon(2);
						break;
					case GUI::Key::NUMBER4:
						message->setWeapon(3);
						break;
					case GUI::Key::NUMBER5:
						message->setWeapon(4);
						break;
					case GUI::Key::NUMBER6:
						message->setWeapon(5);
						break;
					case GUI::Key::NUMBER7:
						message2->setAddLife(20);
						_controlledAvatar->emitMessage(message2);
						break;
					case GUI::Key::NUMBER8:
						message3->setAddShield(20);
						_controlledAvatar->emitMessage(message3);
						break;
					}

					_controlledAvatar->emitMessage(message);
					
					
			}else{
				Logic::CMessageControl *m=new Logic::CMessageControl();
				switch(key.keyId)
				{
				case GUI::Key::W:
					m->setType(Logic::Control::WALK);
					break;
				case GUI::Key::S:
					m->setType(Logic::Control::WALKBACK);
					break;
				case GUI::Key::A:
					m->setType(Logic::Control::STRAFE_LEFT);
					break;
				case GUI::Key::D:
					m->setType(Logic::Control::STRAFE_RIGHT);
					break;
				case GUI::Key::SPACE:
					m->setType(Logic::Control::JUMP);
					break;
				default:
					return false;
				}
				_controlledAvatar->emitMessage(m);
				return true;
			}
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::CMessageControl *m=new Logic::CMessageControl();
			switch(key.keyId)
			{
			case GUI::Key::W:
				m->setType(Logic::Control::STOP_WALK);
				break;
			case GUI::Key::S:
				m->setType(Logic::Control::STOP_WALKBACK);
				break;
			case GUI::Key::A:
				m->setType(Logic::Control::STOP_STRAFE_LEFT);
				break;
			case GUI::Key::D:
				m->setType(Logic::Control::STOP_STRAFE_RIGHT);
				break;
			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			Logic::CMessageMouse *m=new Logic::CMessageMouse();
			m->setType(Logic::Control::MOUSE);
			float mouse[]={-(float)mouseState.movX * TURN_FACTOR_X,-(float)mouseState.movY * TURN_FACTOR_Y};
			m->setMouse(mouse);
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			Logic::CMessageControl *m=new Logic::CMessageControl();
			switch(mouseState.button)
			{
			case GUI::Button::LEFT:
				m->setType(Logic::Control::LEFT_CLICK);
				break;
			case GUI::Button::RIGHT:
				m->setType(Logic::Control::RIGHT_CLICK);
				break;
			case GUI::Button::MIDDLE:
				m->setType(Logic::Control::MIDDLE_CLICK);
				break;
			case GUI::Button::BUTTON_3:
				m->setType(Logic::Control::BUTTON3_CLICK);
				break;
			
			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mousePressed

	//--------------------------------------------------------
	
	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
