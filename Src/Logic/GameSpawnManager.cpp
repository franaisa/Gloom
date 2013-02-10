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

#include "Server.h"
#include "Logic/Maps/Map.h"

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
		//Cargamos los las entidades de tipo SpawnPoint para tenerlas
		CEntity *entidad;
		entidad=CServer::getSingletonPtr()->getMap()->getEntityByType("SpawnPoint");
		if(entidad!=NULL)
			_listSpawnPoints.push_back(entidad);
		while(entidad!=NULL){
			entidad=CServer::getSingletonPtr()->getMap()->getEntityByType("SpawnPoint",entidad);
			if(entidad!=NULL)
				_listSpawnPoints.push_back(entidad);
		}
	} // activate

	//--------------------------------------------------------

	void CGameSpawnManager::deactivate() 
	{	
		_listSpawnPoints.clear();
	} // deactivate

	//---------------------------------------------------------
	
	Vector3 CGameSpawnManager::getSpawnPosition(){
		//std::cout << "EL NUMERO DE PUNTOS SON: " << _listSpawnPoints.size() << std::endl;
		
		return Vector3(3,4,3);
	}

} // namespace Logic
