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
#include "Logic/Entity/Message.h"

#include <cassert>

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
			//esto no me gusta pero por ahora estara asi U.U
			if(key.keyId == GUI::Key::NUMBER1 || key.keyId == GUI::Key::NUMBER2){
				Logic::CMessageChangeWeapon *m = new Logic::CMessageChangeWeapon(Logic::Message::CHANGE_WEAPON);
				
				switch(key.keyId)
				{
					case GUI::Key::NUMBER1:
						m->setWeapon(0);
					break;
					case GUI::Key::NUMBER2:
						m->setWeapon(1);
					break;
				}
				_controlledAvatar->emitMessage(m);
				return true;
			}else{

				Logic::CMessageControl *m=new Logic::CMessageControl(Logic::Message::CONTROL);
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
			}
			return true;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::CMessageControl *m=new Logic::CMessageControl(Logic::Message::CONTROL);
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
			Logic::CMessageMouse *m=new Logic::CMessageMouse(Logic::Message::CONTROL);
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
			Logic::CMessageControl *m=new Logic::CMessageControl(Logic::Message::CONTROL);
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
