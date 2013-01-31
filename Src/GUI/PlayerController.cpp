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
#include "Logic/Entity/Components/AvatarController.h"
#include "Logic/Entity/Components/Shoot.h"
#include "Logic/Entity/Components/ArrayGraphics.h"

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
			if(key.keyId == GUI::Key::NUMBER1 || key.keyId == GUI::Key::NUMBER2 || key.keyId == GUI::Key::NUMBER3 || key.keyId == GUI::Key::NUMBER4 || 
				key.keyId == GUI::Key::NUMBER5 || key.keyId == GUI::Key::NUMBER6 ){
				
					Logic::CArrayGraphics* graphicsComponents= _controlledAvatar->getComponent<Logic::CArrayGraphics>("CArrayGraphics");
					Logic::CShoot* shootComponent = _controlledAvatar->getComponent<Logic::CShoot>("CShoot");

					switch(key.keyId)
					{
					case GUI::Key::NUMBER1:
						//graphicsComponents->changeWeapon(0);
						shootComponent->changeWeapon(0);
						break;
					case GUI::Key::NUMBER2:
						//graphicsComponents->changeWeapon(1);
						shootComponent->changeWeapon(1);
						break;
					case GUI::Key::NUMBER3:
						//graphicsComponents->changeWeapon(2);
						shootComponent->changeWeapon(2);
						break;
					case GUI::Key::NUMBER4:
						//graphicsComponents->changeWeapon(3);
						shootComponent->changeWeapon(3);
						break;
					case GUI::Key::NUMBER5:
						//graphicsComponents->changeWeapon(4);
						shootComponent->changeWeapon(4);
						break;
					case GUI::Key::NUMBER6:
						//graphicsComponents->changeWeapon(5);
						shootComponent->changeWeapon(5);
						break;
					}
			}else{
				Logic::CAvatarController* controller = _controlledAvatar->getComponent<Logic::CAvatarController>("CAvatarController");

				switch(key.keyId)
				{
				case GUI::Key::W:
					controller->walk();
				
					break;
				case GUI::Key::S:
					controller->walkBack();

					break;
				case GUI::Key::A:
					controller->strafeLeft();

					break;
				case GUI::Key::D:
					controller->strafeRight();
				
					break;
				case GUI::Key::SPACE:
					controller->jump();
				
					break;
			
				default:
					return false;
				}
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
			Logic::CAvatarController* controller = _controlledAvatar->getComponent<Logic::CAvatarController>("CAvatarController");
			
			switch(key.keyId)
			{
			case GUI::Key::W:
				controller->stopWalk();

				break;
			case GUI::Key::S:
				controller->stopWalkBack();

				break;
			case GUI::Key::A:
				controller->stopStrafeLeft();

				break;
			case GUI::Key::D:
				controller->stopStrafeRight();

				break;
			default:
				return false;
			}

			return true;
		}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			float mouse[]={-(float)mouseState.movX * TURN_FACTOR_X,-(float)mouseState.movY * TURN_FACTOR_Y};
			_controlledAvatar->getComponent<Logic::CAvatarController>("CAvatarController")->mouse(mouse);
			
			return true;
		}
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			Logic::CShoot* shootComponent = _controlledAvatar->getComponent<Logic::CShoot>("CShoot");
			
			switch(mouseState.button)
			{
			case GUI::Button::LEFT:
				shootComponent->shoot();

				break;
			case GUI::Button::RIGHT:
				//m->setType(Logic::Control::RIGHT_CLICK);
				
				break;
			case GUI::Button::MIDDLE:
				//m->setType(Logic::Control::MIDDLE_CLICK);
				
				break;
			case GUI::Button::BUTTON_3:
				//m->setType(Logic::Control::BUTTON3_CLICK);
				
				break;
			
			default:
				return false;
			}

			//_controlledAvatar->emitMessage(m);
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
