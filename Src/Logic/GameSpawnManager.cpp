//---------------------------------------------------------------------------
// GameSpawnManager.cpp
//---------------------------------------------------------------------------

/**
@file GameSpawnManager.cpp

Contiene la implementación del gestor del spawn/respawn de jugadores.

@see Logic::CGameSpawnManager

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#include "GameSpawnManager.h"

#include <cassert>


namespace Logic {

	CGameSpawnManager* CGameSpawnManager::_instance = 0;

	//--------------------------------------------------------

	CGameSpawnManager::CGameSpawnManager()
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CGameSpawnManager::~CGameSpawnManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CGameSpawnManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CGameNetMsgManager no permitida!");

		new CGameSpawnManager();

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameSpawnManager::Release()
	{
		assert(_instance && "Logic::CGameSpawnManager no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//--------------------------------------------------------

	void CGameSpawnManager::activate() 
	{
		
	} // activate

	//--------------------------------------------------------

	void CGameSpawnManager::deactivate() 
	{	
		
	} // deactivate

	//---------------------------------------------------------
		

} // namespace Logic
