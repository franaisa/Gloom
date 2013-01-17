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
			Logic::CMessageControl *m=new Logic::CMessageControl(Logic::Message::CONTROL);
			switch(key.keyId)
			{
			case GUI::Key::W:
				m->setString("walk");
				break;
			case GUI::Key::S:
				m->setString("walkBack");
				break;
			case GUI::Key::A:
				m->setString("strafeLeft");
				break;
			case GUI::Key::D:
				m->setString("strafeRight");
				break;
			case GUI::Key::SPACE:
				m->setString("jump");
				break;
			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
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
				m->setString("stopWalk");
				break;
			case GUI::Key::S:
				m->setString("stopWalkBack");
				break;
			case GUI::Key::A:
				m->setString("stopStrafeLeft");
				break;
			case GUI::Key::D:
				m->setString("stopStrafeRight");
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
			Logic::CMessageControl *m=new Logic::CMessageControl(Logic::Message::CONTROL);
			m->setString("mouse");
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
				m->setString("LeftClick");
				break;
			case GUI::Button::RIGHT:
				m->setString("RightClick");
				break;
			case GUI::Button::MIDDLE:
				m->setString("MiddleClick");
				break;
			case GUI::Button::BUTTON_3:
				m->setString("Button3Click");
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
