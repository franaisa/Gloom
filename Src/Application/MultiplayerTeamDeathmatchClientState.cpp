//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchClientState.cpp
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchClientState.cpp

Contiene la implementación del estado de juego.

@see Application::MultiplayerTeamDeathmatchClientState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#include "MultiplayerTeamDeathmatchClientState.h"
#include "OgreClock.h"
#include "Net/Manager.h"
#include "Net/Servidor.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/buffer.h"

#include "Logic/GameNetPlayersManager.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityID.h"
#include "Logic\Entity\Components\Camera.h"

#include "Hikari.h"
#include "FlashValue.h"
#include "GUI\Server.h"
#include "Input\PlayerController.h"
#include "Input/Server.h"

namespace Application {

	
	bool CMultiplayerTeamDeathmatchClientState::init(){

		//iniciamos el menu de seleccion de personaje
		_seleccion = GUI::CServer::getSingletonPtr()->addLayout("seleccion", Hikari::Position(Hikari::Center));
		_seleccion->load("SeleccionPersonaje.swf");
		_seleccion->bind("selected",Hikari::FlashDelegate(this, &CMultiplayerTeamDeathmatchClientState::classSelected));
		_seleccion->hide();
		_seleccion->setTransparent(true, true);
		
		return true;
	}
	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::activate() {
		CGameState::activate();
		
		_seleccion->show();

		// Registramos a este estado como observador de red para que sea notificado
		Net::CManager::getSingletonPtr()->addObserver(this);
		
		//comenzamos el proceso de sincronizacion, para ello enviamos un mensaje de ping
		//y tomamos el tiempo para cronometrar cuanto tarda el servidor en respondernos
		Net::NetMessageType ackMsg = Net::PING;
		Net::NetID id = Net::CManager::getSingletonPtr()->getID();
		Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(id));
		ackBuffer.write(&ackMsg, sizeof(ackMsg));
		ackBuffer.write(&id, sizeof(id));
		_reloj = clock();
		_npings = 0;
		_pingActual = 0;
		Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());
	} // activate

	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::deactivate() {
		// Indicamos que ya no queremos ser notificados por la red
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Nos desconectamos
		Net::CManager::getSingletonPtr()->deactivateNetwork();

		CGameState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	void CMultiplayerTeamDeathmatchClientState::dataPacketReceived(Net::CPaquete* packet) {
		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset();

		Net::NetMessageType msg;
		buffer.read(&msg, sizeof(msg));

		switch (msg) {
			case Net::LOAD_PLAYER: {
				// cargamos la informacion del player que nos han enviado
				Net::NetID id;
				Logic::TEntityID entityID;
				buffer.read(&id, sizeof(id));
				buffer.read(&entityID, sizeof(entityID));
				std::string type, name;
				buffer.deserialize(type);
				buffer.deserialize(name);

				//llamo al metodo de creacion del jugador
				Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, type, entityID);

				//Enviamos el mensaje de que se ha creado el jugador
				Net::NetMessageType ackMsg = Net::PLAYER_LOADED;
				Net::CBuffer ackBuffer(sizeof(ackMsg) + sizeof(id));
				ackBuffer.write(&ackMsg, sizeof(ackMsg));
				ackBuffer.write(&id, sizeof(id));
				Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());
				player->activate();
				break;
			}
			case Net::PING: {
				// me llega la respuesta de un ping, por lo tanto tomo el tiempo y calculo mi ping
				unsigned int time = clock();
				unsigned int ping = time - _reloj;
				_npings++;
				_pingActual += ping/2;

				if(_npings>5) { //si ya he tomado suficientes pings, calculo la media y la seteo al server
					_pingActual = _pingActual/_npings;
					clock_t serverTime;
					buffer.read(&serverTime,sizeof(serverTime));
					serverTime+=_pingActual;
					Logic::CServer::getSingletonPtr()->setDiffTime(serverTime-time);
				}
				else { //si no he tomado suficientes pings, me guardo el ping y envío otro
					Net::NetMessageType msgping = Net::PING;
					Net::NetID id = Net::CManager::getSingletonPtr()->getID();
					Net::CBuffer ackBuffer(sizeof(msgping) + sizeof(id));
					ackBuffer.write(&msgping, sizeof(msgping));
					ackBuffer.write(&id, sizeof(id));
					_reloj = clock();
					Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());
				}

				break;
			}
			case Net::LOAD_LOCAL_PLAYER: {
				// cargamos la informacion del player que nos han enviado
				Logic::TEntityID entityID;
				buffer.read(&entityID, sizeof(entityID));
				std::string type, name;
				buffer.deserialize(type);
				buffer.deserialize(name);

				//llamo al metodo de creacion del jugador
				Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createLocalPlayer(name, type, entityID);

				player->activate();
				Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Camera")->getComponent<Logic::CCamera>("CCamera")->setTarget(player);


				break;
			}
		}
	}

	bool CMultiplayerTeamDeathmatchClientState::keyPressed(Input::TKey key)
	{
		CGameState::keyPressed(key);
		
		switch(key.keyId)
		{
		case Input::Key::C://cambio de clase
			//primero, quitamos al player de escuchar las teclas, para ello lo desactivamos del playerController
			Input::CServer::getSingletonPtr()->getPlayerController()->deactivate();
			//mostramos la gui
			_seleccion->show();
			break;
		default:
			return true;
		}
		
		return true;

	} // keyPressed

	//--------------------------------------------------------

	bool CMultiplayerTeamDeathmatchClientState::keyReleased(Input::TKey key)
	{
		CGameState::keyReleased(key);
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMultiplayerTeamDeathmatchClientState::mouseMoved(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMultiplayerTeamDeathmatchClientState::mousePressed(const Input::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMultiplayerTeamDeathmatchClientState::mouseReleased(const Input::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

	//--------------------------------------------------------

	Hikari::FlashValue CMultiplayerTeamDeathmatchClientState::classSelected(Hikari::FlashControl* caller, const Hikari::Arguments& args)
	{
		
		int selectedClass = args.at(0).getNumber();
		Net::NetMessageType msgType = Net::CLASS_SELECTED;
		Net::CBuffer msg (sizeof(msgType) + sizeof(selectedClass));
		switch(selectedClass)
		{
			case 0:
				if(Input::CServer::getSingletonPtr()->getPlayerController()->getControllerAvatar()){
					Input::CServer::getSingletonPtr()->getPlayerController()->activate();
				}else{

				}

				break;
			case 1:
			case 2:
			case 3:
			case 4:
				_seleccion->hide();
				//enviamos la clase elegida
				msg.serialize(msgType);
				msg.serialize(selectedClass);
				Net::CManager::getSingletonPtr()->send(msg.getbuffer(), msg.getSize());
				break;
			case 5:
				{
				_seleccion->hide();
				//random de la clase y lo enviamos por red
				int randomclass = ((rand()*clock())%4)+1;
				msg.serialize(msgType);
				msg.serialize(randomclass);
				Net::CManager::getSingletonPtr()->send(msg.getbuffer(), msg.getSize());
				break;
				}
		}//switch

		//enviamos el mensaje
		

		return FLASH_VOID;

	} // backReleased

};