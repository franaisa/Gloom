#include "Scoreboard.h"
#include "GUIManager.h"
#include <cassert>
#include <vector>
#include "Hikari.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"

#include "Audio/Server.h"

#include "Graphics/Scene.h"

namespace Logic{
	CScoreboard* CScoreboard::_instance = 0;

	CScoreboard::CScoreboard(){
		assert(!_instance && "Segunda inicialización de Logic::CGUIKillersMessage no permitida!");

		_instance = this;
		_guiManager = CGUIManager::getSingletonPtr();
		_scoreboard = 0;

		initSpreeMessages();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	void CScoreboard::addPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass, int team){
		//por si acaso, vamos a comprobar que no lo habiamos añadido
		if(_players.find(name)!=_players.end())
			return;
		
		//primero lo insertamos en nuestra estructura de datos
		PlayerInfo player(name, playerEntity, playerClass, team);
		TPlayerInfo newPlayer(name, player);
		_players.insert(newPlayer);
		std::cout << "Añadiendo " << name << " al equipo " << team << std::endl;
		//ahora llamamos a la GUI para que cree nuestro nuevo player
		if(_scoreboard && team == 0)
			_scoreboard->callFunction("addPlayer",Hikari::Args(name)(playerClass));
		else if (_scoreboard && team!= 0)
			_scoreboard->callFunction("addPlayer",Hikari::Args(name)(playerClass)(team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::addLocalPlayer(const std::string &name, CEntity * playerEntity, const std::string &playerClass, int team){
		//por si acaso, vamos a comprobar que no lo habiamos añadido
		if(_players.find(name)!=_players.end())
			return;

		//primero lo insertamos en nuestra estructura de datos
		PlayerInfo player(name, playerEntity, playerClass, team);
		TPlayerInfo newPlayer(name, player);
		_players.insert(newPlayer);
		std::cout << "Añadiendo " << name << " al equipo " << team << std::endl;
		//ahora llamamos a la GUI para que cree nuestro nuevo player
		if(_scoreboard && team == 0)
			_scoreboard->callFunction("addLocalPlayer",Hikari::Args(name)(playerClass));
		else if(_scoreboard && team!=0)
			_scoreboard->callFunction("addLocalPlayer",Hikari::Args(name)(playerClass)(team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::changePlayerEntity(const std::string &name, CEntity * newPlayerEntity, const std::string &newClass, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.entityPlayer = newPlayerEntity;
		player->second.playerClass = newClass;

		//ahora avisamos a la GUI de que ha habido un cambio
		if(player->second.team==0)
			_scoreboard->callFunction("changeClass",Hikari::Args(name)(newClass));
		else
			_scoreboard->callFunction("changeClass",Hikari::Args(name)(newClass)((int)player->second.team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::deletePlayer(const std::string &name, int team){
		auto player = _players.find(name);

		if( player != _players.end() ) {
			//ahora avisamos a la GUI de que ha habido un cambio
			if(player->second.team == 0)
				_scoreboard->callFunction("deletePlayer",Hikari::Args(name));
			else
				_scoreboard->callFunction("deletePlayer",Hikari::Args(name)((int)player->second.team));

			_players.erase(player);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::addKill(const std::string &name, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.kills++;

		if( ++(player->second.currentSpree) > player->second.bestSpree ) {
			player->second.bestSpree = player->second.currentSpree;
			if(player->second.team==0)
				_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)player->second.bestSpree));
			else
				_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)player->second.bestSpree)((int)player->second.team));
		}

		showSpreeMessage(player->second.entityPlayer, player->second.name, player->second.currentSpree);

		//ahora avisamos a la GUI de que ha habido un cambio
		if(player->second.team==0)
			_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills));
		else
			_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills)((int)player->second.team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::showSpreeMessage(Logic::CEntity* entity, const std::string &name, unsigned int nbKills) {
		if(nbKills % 3 == 0 && nbKills < 28) {
			unsigned int index = nbKills / 3;
			
			// Sonido chachi de que llevas un spree
			if( entity != NULL && entity->isPlayer() ) {
				Audio::CServer::getSingletonPtr()->playSound("feedback/bell.mp3", false, false);
				std::cout << _spreePlayerMsgList[index - 1] << std::endl;
				//_spreeMenu->callFunction( "spreePlayer", Hikari::Args(name)(_spreePlayerMsgList[index - 1]) );
			}
			else {
				std::cout << _spreeMsgList[index - 1] << std::endl;
				_spreeMenu->callFunction( "spree", Hikari::Args(name)(_spreeMsgList[index - 1]) );
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::subKill(const std::string &name, int team) {
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.kills--;

		//ahora avisamos a la GUI de que ha habido un cambio
		if(player->second.team==0)
			_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills));
		else
			_scoreboard->callFunction("addKill",Hikari::Args(name)((int)player->second.kills)((int)player->second.team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::addDeath(const std::string &name, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.deaths++;
		player->second.currentSpree = 0;

		//ahora avisamos a la GUI de que ha habido un cambio
		if(player->second.team==0)
			_scoreboard->callFunction("addDeath",Hikari::Args(name)((int)player->second.deaths));
		else
			_scoreboard->callFunction("addDeath",Hikari::Args(name)((int)player->second.deaths)((int)player->second.team));
		
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changeName(const std::string &oldName, const std::string &newName, int team){
		auto player = _players.find(oldName);

		if(player==_players.end())
			return;
		PlayerInfo newPlayerInfo = player->second;
		newPlayerInfo.name = newName;
		_players.erase(player);
		TPlayerInfo playerUpdated(newName, newPlayerInfo);

		_players.insert(playerUpdated);

		//ahora avisamos a la GUI de que ha habido un cambio
		if(team==0)
			_scoreboard->callFunction("changeNick",Hikari::Args(oldName)(newName));
		else
			_scoreboard->callFunction("changeNick",Hikari::Args(oldName)(newName)((int)player->second.team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changePing(const std::string &name, unsigned int &ping, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.ping = ping;

		//ahora avisamos a la GUI de que ha habido un cambio
		_scoreboard->callFunction("changeNick",Hikari::Args(name)((int) ping));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CScoreboard::changeSpree(const std::string &name, unsigned int &newSpree, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return;
		player->second.bestSpree = newSpree;

		//ahora avisamos a la GUI de que ha habido un cambio
		if(team==0)
			_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)newSpree));
		else
			_scoreboard->callFunction("addSpree",Hikari::Args(name)((int)newSpree)((int)player->second.team));
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	unsigned int CScoreboard::getSpree(const std::string &name, int team){
		auto player = _players.find(name);

		if(player==_players.end())
			return 0;
		return player->second.bestSpree;
	}

	void CScoreboard::changeTeam(const std::string &name, int newTeam){
		auto player = _players.find(name);

		if(player==_players.end())
			return;

		player->second.team = newTeam;
		_scoreboard->callFunction("changeTeam", Hikari::Args(name)(newTeam));
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
		loadSpreeMenu();
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
			_scoreboard->callFunction("addPlayer",Hikari::Args(it->second.name)(it->second.playerClass)((int)it->second.team));
			_scoreboard->callFunction("addKill",Hikari::Args(it->second.name)((int)it->second.kills)((int)it->second.team));
			_scoreboard->callFunction("addDeath",Hikari::Args(it->second.name)((int)it->second.deaths)((int)it->second.team));
			_scoreboard->callFunction("changePing",Hikari::Args(it->second.name)((int)it->second.ping)((int)it->second.team));
			_scoreboard->callFunction("addSpree",Hikari::Args(it->second.name)((int)it->second.bestSpree)((int)it->second.team));
		}
		loadSpreeMenu();
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

	void CScoreboard::loadSpreeMenu()  {
		_guiManager->addGUI("spreeMenu", Hikari::Position(Hikari::Center), 0.85f);
		_guiManager->load("spreeMenu", "spreeMsg.swf");
		_guiManager->showGUI("spreeMenu");
		_guiManager->setTransparent("spreeMenu",true);
		_spreeMenu = _guiManager->getGUIControl("spreeMenu");
		_spreeMenu->show();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CScoreboard::initSpreeMessages() {
		_spreeMsgList.reserve(9);
		_spreeMsgList.push_back("is on a Killing Spree");
		_spreeMsgList.push_back("is in a Blood Bath");
		_spreeMsgList.push_back("is on a Ludicrous Massacre");
		_spreeMsgList.push_back("is on a Gruesome Carnage");
		_spreeMsgList.push_back("is on a Daunting Feast");
		_spreeMsgList.push_back("is in a Wicked Carnival");
		_spreeMsgList.push_back("is in the Devil's Playground");
		_spreeMsgList.push_back("is the Soul Master");
		_spreeMsgList.push_back("owns the match");

		_spreePlayerMsgList.reserve(9);
		_spreePlayerMsgList.push_back("Killing Spree");
		_spreePlayerMsgList.push_back("Blood Bath");
		_spreePlayerMsgList.push_back("Ludicrous Massacre");
		_spreePlayerMsgList.push_back("Gruesome Carnage");
		_spreePlayerMsgList.push_back("Daunting Feast");
		_spreePlayerMsgList.push_back("Wicked Carnival");
		_spreePlayerMsgList.push_back("Devil's Playground");
		_spreePlayerMsgList.push_back("Soul Master");
		_spreePlayerMsgList.push_back("Ownage");
	}
}