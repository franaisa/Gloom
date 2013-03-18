//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal de audio, llevará el control de los sonidos en el juego.

@see Audio::CServer

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "Server.h"

#include <cassert>
#include <iostream>

namespace Audio
{
	CServer *CServer::_instance = 0;

	CServer::CServer()
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

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
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		FMOD_RESULT result;
		//Creamos
		result = System_Create(&_system);
		if(result!= FMOD_OK) return false;
			
		//Iniciamos
		result = _system->init(100, FMOD_INIT_NORMAL, 0);
		if(result!= FMOD_OK) return false;


		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		_system->release();

	} // close

	//--------------------------------------------------------

	void CServer::tick(unsigned int secs) 
	{
		/*if(_activeScene != _dummyScene)
			_activeScene->tick(secs);
		if(_root)
		{
			// Actualizamos todas las ventanas de reenderizado.
			Ogre::WindowEventUtilities::messagePump();
			// Reenderizamos un frame
			_root->renderOneFrame(secs);
		}*/




	} // tick

	//--------------------------------------------------------

} // namespace Audio