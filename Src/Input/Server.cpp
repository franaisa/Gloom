/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la entrada de periféricos.

@see Input::CServer

@author Rubén Mulero
@date March, 2013
*/

#include "Server.h"
#include "PlayerController.h"
#include "BaseSubsystems/Server.h"

#include "Hikari.h"

#include <cassert>
namespace Input {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _playerController(0) 
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;
	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		_playerController = new CPlayerController();

		_GUISystem = BaseSubsystems::CServer::getSingletonPtr()->getHikari();

		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addKeyListener(this);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

		delete _playerController;

	} // close

	//--------------------------------------------------------

	bool CServer::keyPressed(TKey key)
	{
		//_GUISystem->injectKeyDown(key.keyId);    
		//_GUISystem->injectChar(key.text);    

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CServer::keyReleased(TKey key)
	{
		//_GUISystem->injectKeyUp(key.keyId);

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CServer::mouseMoved(const CMouseState &mouseState)
	{
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		_GUISystem->injectMousePosition((float)mouseState.posAbsX,(float)mouseState.posAbsY);
#else 
		//_GUISystem->injectMouseMove((float)mouseState.movX,(float)mouseState.movY);
		_GUISystem->injectMouseMove(mouseState.posAbsX, mouseState.posAbsY);
#endif	
		// Queremos que si hay más oyentes también reciban el evento
		return true;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CServer::mousePressed(const CMouseState &mouseState)
	{
		_GUISystem->injectMouseDown(mouseState.button);

		// Queremos que si hay más oyentes también reciban el evento
		return true;

	} // mousePressed

	//--------------------------------------------------------

	bool CServer::mouseReleased(const CMouseState &mouseState)
	{
		_GUISystem->injectMouseUp(mouseState.button);

		// Queremos que si hay más oyentes también reciban el evento
		return true;

	} // mouseReleased

} // namespace Input
