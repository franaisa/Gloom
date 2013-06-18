#include "Scoreboard.h"
#include "GUIManager.h"
#include <cassert>
#include <vector>
#include "Hikari.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "Graphics/Scene.h"

namespace Logic{
	CScoreboard* CScoreboard::_instance = 0;

	CScoreboard::CScoreboard(){
		assert(!_instance && "Segunda inicialización de Logic::CGUIKillersMessage no permitida!");

		_instance = this;
		_guiManager = CGUIManager::getSingletonPtr();
		_scoreboard = 0;
	}

	CScoreboard::~CScoreboard(){
		_instance = 0;
		
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::Init(){
		assert(!_instance && "Segunda inicialización de Logic::CScoreboard no permitida!");

		new CScoreboard();

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::Release()
	{
		if(_instance)
		{
			delete _instance;
		}
	} // 

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::addPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass){
		//por si acaso, vamos a comprobar que no lo habiamos añadido
		if(_players.find(name)!=_players.end())
			return;
		
		//primero lo insertamos en nuestra estructura de datos
		PlayerInfo player(name, playerEntity, playerClass);
		TPlayerInfo newPlayer(name, player);
		_players.insert(newPlayer);

		//ahora llamamos a la GUI para que cree nuestro nuevo player
		if(_scoreboard)
			_scoreboard->callFunction("addPlayer",Hikari::Args(name)(playerClass));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::addLocalPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass){
		//por si acaso, vamos a comprobar que no lo habiamos añadido
		if(_players.find(name)!=_players.end())
			return;

		//primero lo insertamos en nuestra estructura de datos
		PlayerInfo player(name, playerEntity, playerClass);
		TPlayerInfo newPlayer(name, player);
		_players.insert(newPlayer);

		//ahora llamamos a la GUI para que cree nuestro nuevo player
		if(_scoreboard)
			_scoreboard->callFunction("addLocalPlayer",Hikari::Args(name)(playerClass));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::changePlayerEntity(const std::string &name, CEntity * newPlayerEntity, const std::string &newClass){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.entityPlayer = newPlayerEntity;
		player->second.playerClass = newClass;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("changeClass",Hikari::Args(name)(newClass));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::deletePlayer(const std::string &name){
		auto player = _players.find(name);

		if(player!=_players.end())
			_players.erase(player);

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("deletePlayer",Hikari::Args(name));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::addKill(const std::string &name){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.kills++;

		if( ++(player->second.currentSpree) > player->second.bestSpree ) {
			player->second.bestSpree = player->second.currentSpree;
			_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)player->second.bestSpree));
		}

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::subKill(const std::string &name) {
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.kills--;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::addDeath(const std::string &name){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.deaths++;
		player->second.currentSpree = 0;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("addDeath",Hikari::Args(name)((int)player->second.deaths));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changeName(const std::string &oldName, const std::string &newName){
		auto player = _players.find(oldName);

		if(player==_players.end())
			return;
		PlayerInfo newPlayerInfo = player->second;
		newPlayerInfo.name = newName;
		_players.erase(player);
		TPlayerInfo playerUpdated(newName, newPlayerInfo);

		_players.insert(playerUpdated);

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("changeNick",Hikari::Args(oldName)(newName));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changePing(const std::string &name, unsigned int &ping){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.ping = ping;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("changeNick",Hikari::Args(name)((int) ping));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changeSpree(const std::string &name, unsigned int &newSpree){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.bestSpree = newSpree;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)newSpree));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	unsigned int CScoreboard::getSpree(const std::string &name){
		auto player = _players.find(name);

		if(player==_players.end())
			return 0;
		return player->second.bestSpree;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*bool CScoreboard::keyPressed(Input::TKey key){
		if(!_scoreboard){
			return true;
		}


		if (key.keyId == Input::Key::TAB){
			_scoreboard->show();
		}
		return true;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CScoreboard::keyReleased(Input::TKey key){
		if(!_scoreboard){
			return true;
		}
		if (key.keyId == Input::Key::TAB){
			_scoreboard->hide();
		}

		return true;
	}*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::clearPlayers(){
		_players.clear();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::activate(){
		//Input::CInputManager::getSingletonPtr()->addKeyListener(this);
		if(_scoreboard) _scoreboard->show();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::deactivate(){
		//Input::CInputManager::getSingletonPtr()->removeKeyListener(this);
		//_players.clear();
		if(_scoreboard) _scoreboard->hide();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	CEntity* CScoreboard::getPlayer(const std::string &name){
		auto player = _players.find(name);

		if(player != _players.end())
			return player->second.entityPlayer;
		return NULL;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::loadScoreboardDM(){
		_guiManager->addGUI("scoreboard", Hikari::Position(Hikari::Center), 0.85f);
		_guiManager->load("scoreboard", "ScoreDM.swf");
		_guiManager->showGUI("scoreboard");
		_guiManager->setTransparent("scoreboard",true);
		_scoreboard = _guiManager->getGUIControl("scoreboard");
		_scoreboard->hide();

		//ahora cargamos los jugadores que habia en la partida
		auto it = _players.begin();
		auto end = _players.end();

		for(;it!=end;++it){
			_scoreboard->callFunction("addPlayer",Hikari::Args(it->second.name)(it->second.playerClass));
			_scoreboard->callFunction("addKill",Hikari::Args(it->second.name)((int)it->second.kills));
			_scoreboard->callFunction("addDeath",Hikari::Args(it->second.name)((int)it->second.deaths));
			_scoreboard->callFunction("changePing",Hikari::Args(it->second.name)((int)it->second.ping));
			_scoreboard->callFunction("addSpree",Hikari::Args(it->second.name)((int)it->second.bestSpree));
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::loadScoreboardTDM(){
		_guiManager->addGUI("scoreboard", Hikari::Position(Hikari::Center), 0.85f);
		_guiManager->load("scoreboard", "ScoreTDM.swf");
		_guiManager->showGUI("scoreboard");
		_guiManager->setTransparent("scoreboard",true);
		_scoreboard = _guiManager->getGUIControl("scoreboard");
		_scoreboard->hide();

		//ahora cargamos los jugadores que habia en la partida
		auto it = _players.begin();
		auto end = _players.end();

		for(;it!=end;++it){
			_scoreboard->callFunction("addPlayer",Hikari::Args(it->second.name)(it->second.playerClass));
			_scoreboard->callFunction("addKill",Hikari::Args(it->second.name)((int)it->second.kills));
			_scoreboard->callFunction("addDeath",Hikari::Args(it->second.name)((int)it->second.deaths));
			_scoreboard->callFunction("changePing",Hikari::Args(it->second.name)((int)it->second.ping));
			_scoreboard->callFunction("addSpree",Hikari::Args(it->second.name)((int)it->second.bestSpree));
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::unLoadScoreboard(){
		_guiManager->deleteGUI("scoreboard");
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<CScoreboard::PlayerInfo> CScoreboard::getAllPlayers(){
		std::vector<CScoreboard::PlayerInfo> players;

		auto it = _players.begin();
		auto end = _players.end();

		for(;it!=end;++it){
			players.push_back(it->second);
		}
		return players;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	Net::CBuffer CScoreboard::serialize(){

		Net::CBuffer players;

		auto it = _players.begin();
		auto end = _players.end();

		players.serialize(_players.size());

		for(;it!=end;++it){
			players.serialize(it->second.name, false);
			players.serialize(it->second.playerClass, false);
			players.serialize(it->second.kills);
			players.serialize(it->second.deaths);
			players.serialize(it->second.bestSpree);
			players.serialize(it->second.team);
			players.serialize(it->second.ping);
		}
		return players;

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::deserialize(Net::CBuffer &players){
		unsigned int size;

		players.deserialize(size);

		for(int i=0;i<size;++i){
			std::string name, playerClass;
			players.deserialize(name);

			auto player = _players.find(name);

			players.deserialize(player->second.playerClass);
			players.deserialize(player->second.kills);
			players.deserialize(player->second.deaths);
			players.deserialize(player->second.bestSpree);
			players.deserialize(player->second.team);
			players.deserialize(player->second.ping);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////



}