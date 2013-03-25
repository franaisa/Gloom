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

//PRUEBAS,QUITAR LUEGO el INCLUDE SIGUIENTE
#include "Audio/Server.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

//MENSAJE PARA DEBBUG
#include "Logic/Messages/MessageHudDebbug.h"

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


		/*_strafingRight=false;
		_strafingLeft=false;
		_unpressLeft=false;
		_unpressRight=false;
		_jumping=false;
		//Inicializacion variables de control ( salto, salto lateral, concatenacion salto lateral y rebote )
		_jumping = false;
		_jumpingControl = false;
		_speedJump=-0.5f;
		_falling=false;
		_caida=false;
		_jumpLeft=0;
		_jumpRight=0;
		_timeSideJump=0;
		_sideJump=false;
		_velocitySideJump=false;
		_unpressRight=false;
		_unpressLeft=false;
		_readySideJumpLeft=false;
		_readySideJumpRight=false;
		_dontCountUntilUnpress=false;
		_nConcatSideJump=0;
		_timeConcatSideJump=0;
		_activeConcat=false;
		_sideFly=false;
		_sideContact=false;*/



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
				key.keyId == GUI::Key::NUMBER5 || key.keyId == GUI::Key::NUMBER6 || key.keyId == GUI::Key::NUMBER7 || key.keyId == GUI::Key::NUMBER8){
				
					Logic::CMessageChangeWeapon *message=new Logic::CMessageChangeWeapon();
					Vector3 position;										
					
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
						position=Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Jumper1")->getPosition();
						Audio::CServer::getSingletonPtr()->playSound3D("media/audio/plasma.wav", "cosa", position);
						break;
					case GUI::Key::NUMBER8:
						break;
					}

					_controlledAvatar->emitMessage(message);
					
					
			}else{
				Logic::CMessageControl *m=new Logic::CMessageControl();
				Logic::CMessageHudDebbug *m2=new Logic::CMessageHudDebbug();
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
				case GUI::Key::O:
					_controlledAvatar->emitMessage(m2);
					break;
				default:
					return false;
				}
				if (key.keyId != GUI::Key::O){
					//Tomamos el tiempo
					unsigned int time = clock();
					//Antes de emitir el tipico mensaje comprobaremos si en verdad es un salto lateral ( y después si es concatenado )
					/*
					//Control del tiempo para el salto lateral(al contar aqui cuando se activa no se cuenta el primer tick)
					if(_jumpLeft!=0 || _jumpRight!=0){
						_timeSideJump+=msecs;
					}
					else{
						_timeSideJump=0;
					}

					//Controlamos cuando soltamos la tecla para hacer que la siguiente vez se active el salto
					if(_unpressLeft && _jumpLeft==1 && !_dontCountUntilUnpress){
						_readySideJumpLeft=true;
						_readySideJumpRight=false;
						_jumpRight=0;
						_unpressLeft=false;
					}
					else if(_unpressRight && _jumpRight==1 && !_dontCountUntilUnpress){
						_readySideJumpRight=true;
						_readySideJumpLeft=false;
						_jumpLeft=0;
						_unpressRight=false;
					}
					//Cuando soltemos la segunda presión entonces empezamos el conteo de presiones otra vez
					else if((_unpressRight || _unpressLeft) && _dontCountUntilUnpress){
						_dontCountUntilUnpress=false;
						_unpressRight=false;
						_unpressLeft=false;
						_jumpRight=0;
						_jumpLeft=0;
						_readySideJumpLeft=false;
						_readySideJumpRight=false;
						_timeSideJump=0;
					}
					else if(_unpressRight || _unpressLeft){
						_unpressRight=false;
						_unpressLeft=false;
					}

					//Izquierda/Derecha
					if(_strafingLeft || _strafingRight)
					{
						//Si se presionaron ambas teclas
						if(_strafingRight && _strafingLeft){
							_timeSideJump=0;
							_jumpRight=0;
							_jumpLeft=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}
						//Si se presionó la izq , el contador esta a 0 y puedo contar || o el salto izq esta listo y le di a la izq
						else if((_strafingLeft && _jumpLeft==0 && !_dontCountUntilUnpress) || (_readySideJumpLeft && _strafingLeft)){
							if(_jumpRight!=0)
								_timeSideJump=0;
							_jumpLeft++;
							_jumpRight=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}
						//contrario al de arriba
						else if((_strafingRight  && _jumpRight==0 && !_dontCountUntilUnpress) || (_readySideJumpRight && _strafingRight)){
							if(_jumpLeft!=0)
								_timeSideJump=0;
							_jumpRight++;
							_jumpLeft=0;
							_readySideJumpLeft=false;
							_readySideJumpRight=false;
						}

						//Si se activo el salto lateral hacia algun lado y está dentro del tiempo
						if((_jumpRight==2 || _jumpLeft==2) && _timeSideJump<_maxTimeSideJump){ 
							_jumping=true; //Activo el salto
							_sideJump=true;
							_dontCountUntilUnpress=true;
							_nConcatSideJump++;
							_timeSideJump=0;
							_jumpRight=0;
							_jumpLeft=0;
							_readySideJumpRight=false;
							_readySideJumpLeft=false;
							//Control del salto lateral concatenado
							//Si llevamos mas de uno hecho,no estoy cayendo y el tiempo es inferior a _maxTimeConcatSideJump activamos la concatenacion (dará velocidad)
							if(_nConcatSideJump>1 && _timeConcatSideJump<_maxTimeConcatSideJump && !_falling){
								_activeConcat=true;
								_timeConcatSideJump=0;
								_sideContact=false;
							}
							//Si llevo al menos 1 salto, no estoy cayendo, pero el tiempo es mayor a 500msecs reseteo las variables de concatenacion de salto
							else if(_nConcatSideJump>1 && !_falling){
								_nConcatSideJump=1; // A uno por que seria un nuevo conteo de saltos laterales
								_timeConcatSideJump=0;
								_sideContact=false;
								_activeConcat=false;
							}
						}
						//Si se pasó el tiempo reseteo
						else if(_timeSideJump>_maxTimeSideJump){
							_timeSideJump=0;
							_jumpLeft=0;
							_jumpRight=0;
							_readySideJumpRight=false;
							_readySideJumpLeft=false;
						}*/






					_controlledAvatar->emitMessage(m);
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
		
		if(_controlledAvatar)
		{
			Logic::CMessageControl *m=new Logic::CMessageControl();
			switch(mouseState.button)
			{
			case GUI::Button::LEFT:
				m->setType(Logic::Control::UNLEFT_CLICK);
				break;
			case GUI::Button::RIGHT:
				m->setType(Logic::Control::UNRIGHT_CLICK);
				break;
			case GUI::Button::MIDDLE:
				m->setType(Logic::Control::UNMIDDLE_CLICK);
				break;
			case GUI::Button::BUTTON_3:
				m->setType(Logic::Control::UNBUTTON3_CLICK);
				break;
			
			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mouseReleased

} // namespace GUI
