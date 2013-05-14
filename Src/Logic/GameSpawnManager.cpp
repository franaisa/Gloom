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
#include "Logic\Entity\Components\PhysicStaticEntity.h"

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

	// @deprecated la busqueda debe hacerse sobre un unordered_multimap
	// por lo que no deberiamos dar la posicion de la entidad sobre la
	// que empezar
	void CGameSpawnManager::activate() {
		//Cargamos los las entidades de tipo SpawnPoint para tenerlas
		std::list<CEntity*> spawnPointsFound = CServer::getSingletonPtr()->getMap()->getEntitiesByType("SpawnPoint");
		std::list<CEntity*>::const_iterator it = spawnPointsFound.begin();
		std::list<CEntity*>::const_iterator end = spawnPointsFound.end();

		for(; it != end; ++it) {
			_listSpawnPoints.push_back(*it);
		}
		
		_maxTrys=15;
	} // activate

	//--------------------------------------------------------

	void CGameSpawnManager::deactivate() 
	{	
		_listSpawnPoints.clear();
	} // deactivate

	//---------------------------------------------------------
	
	CEntity* CGameSpawnManager::getSpawnPosition(){
		/*std::cout << "EL NUMERO DE PUNTOS DE SPAWN ES: " << _listSpawnPoints.size() << std::endl;
		int disponibles=0;
		for(int i =0;i<_listSpawnPoints.size();i++)
			if(!_listSpawnPoints[i]->getComponent<CPhysicStaticEntity>("CPhysicStaticEntity")->getInTrigger())
				disponibles++;
		std::cout << "EL NUMERO DE PUNTOS DE SPAWN DISPONIBLES ES: " << _listSpawnPoints.size() << std::endl;*/
		int random=(rand()*clock())%_listSpawnPoints.size();
		//Mientras que nos devuelva que el trigger esta activado buscamos otro punto
		int intentos=0;
		while(_listSpawnPoints[random]->getComponent<CPhysicStaticEntity>("CPhysicStaticEntity")->getInTrigger()){
			random=(rand()*clock())%_listSpawnPoints.size();
			intentos++;
			if(intentos>_maxTrys)
				return NULL;
		}
		//Ademas por si acaso se pide mas de un punto en el mismo tick hay que marcarlo instantaneamente
		_listSpawnPoints[random]->getComponent<CPhysicStaticEntity>("CPhysicStaticEntity")->setInTrigger(true);
		return _listSpawnPoints[random];
	}

} // namespace Logic
