//---------------------------------------------------------------------------
// GameOverState.cpp
//---------------------------------------------------------------------------

/**
@file GameOverState.cpp

Contiene la implementación del estado de game over.

@see Application::CApplicationState
@see Application::CGameOverState

@author David Llansó
@date Agosto, 2010
*/

#include "GameOverState.h"

#include "Input/Server.h"

namespace Application {

	CGameOverState::~CGameOverState() 
	{
	} // ~CGameOverState

	//--------------------------------------------------------

	bool CGameOverState::init() 
	{
		CApplicationState::init();
	
		return true;

	} // init

	//--------------------------------------------------------

	void CGameOverState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameOverState::activate() 
	{
		CApplicationState::activate();

	} // activate

	//--------------------------------------------------------

	void CGameOverState::deactivate() 
	{		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameOverState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameOverState::keyPressed(Input::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameOverState::keyReleased(Input::TKey key)
	{
		switch(key.keyId)
		{
		case Input::Key::ESCAPE:
			_app->exitRequest();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameOverState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameOverState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameOverState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
