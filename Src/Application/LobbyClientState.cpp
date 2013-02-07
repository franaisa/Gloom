//---------------------------------------------------------------------------
// LobbyClientState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyClientState.cpp

Contiene la implementación del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llansó
@date Agosto, 2010
*/

#include "LobbyClientState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "Logic/Entity/Entity.h"
#include "Net/Manager.h"
#include "Net/Cliente.h"
#include "Net/factoriared.h"
#include "Net/paquete.h"
#include "Net/conexion.h"
#include "Net/buffer.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CLobbyClientState::~CLobbyClientState() 
	{
	} // ~CLobbyClientState

	//--------------------------------------------------------

	bool CLobbyClientState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyClient.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::startReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyClientState::backReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CLobbyClientState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLobbyClientState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

		Net::CManager::getSingletonPtr()->addObserver(this);
		// Activar la red
		Net::CManager::getSingletonPtr()->activateAsClient();

	} // activate

	//--------------------------------------------------------

	void CLobbyClientState::deactivate() 
	{	
		Net::CManager::getSingletonPtr()->removeObserver(this);

		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);

		CApplicationState::deactivate();


	} // deactivate

	//--------------------------------------------------------

	void CLobbyClientState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	void CLobbyClientState::dataPacketReceived(Net::CPaquete* packet)
	{

		Net::CBuffer buffer(packet->getDataLength());
		buffer.write(packet->getData(), packet->getDataLength());
		buffer.reset();

		Net::NetMessageType msg;
		//memcpy(&msg, packet->getData(), sizeof(msg));
		buffer.read(&msg, sizeof(msg));

		switch (msg)
		{
		case Net::LOAD_MAP:

			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_client.txt"))
			{
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();
			}

			// Cargamos el nivel a partir del nombre del mapa. 
			if (!Logic::CServer::getSingletonPtr()->loadLevel("map_client.txt"))
			{
				Net::CManager::getSingletonPtr()->deactivateNetwork();
				_app->exitRequest();
			}
			else
			{
				//Avisamos de que hemos terminado la carga.
				Net::NetMessageType ackMsg = Net::MAP_LOADED;
				Net::CBuffer ackBuffer(sizeof(ackMsg));

				ackBuffer.write(&ackMsg, sizeof(ackMsg));
				Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());

			}
			break;
		case Net::LOAD_PLAYER:
			{

			// Creamos el player. Deberíamos poder propocionar caracteríasticas
			// diferentes según el cliente (nombre, modelo, etc.). Esto es una
			// aproximación, solo cambiamos el nombre y decimos si es el jugador
			// local. Los datos deberían llegar en el paquete de red.
			Net::NetID id;
			Logic::TEntityID entityID;
			//memcpy(&id, packet->getData() + sizeof(msg), sizeof(id));
			buffer.read(&id, sizeof(id));
			buffer.read(&entityID, sizeof(entityID));
			std::string name("Player");
			std::stringstream number;
			number << id;
			name.append(number.str());

			// @todo Llamar al método de creación del jugador. Deberemos decidir
			// si el jugador es el jugador local (si el ID del paquete coincide 
			// con nuestro ID de red).

			bool localPlayer = false;
			if(id == Net::CManager::getSingletonPtr()->getID()) {
				localPlayer = true;
			}
			Logic::CEntity * player = Logic::CServer::getSingletonPtr()->getMap()->createPlayer(name, localPlayer);
			player->setEntityID(entityID);

			//Enviamos el mensaje de que se ha creado el jugador
			Net::NetMessageType ackMsg = Net::PLAYER_LOADED;
			Net::CBuffer ackBuffer(sizeof(ackMsg));
			ackBuffer.write(&ackMsg, sizeof(ackMsg));
			Net::CManager::getSingletonPtr()->send(ackBuffer.getbuffer(), ackBuffer.getSize());

			//Net::NetMessageType msg = Net::PLAYER_LOADED;
			//Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));
			}
			break;
		case Net::START_GAME:

			_app->setState("game");
			break;
		}
		
	} // dataPacketReceived

	//--------------------------------------------------------
	
	bool CLobbyClientState::keyPressed(GUI::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyClientState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			doStart();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLobbyClientState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyClientState::mousePressed(const GUI::CMouseState &mouseState)
	{
		//CEGUI::System::getSingleton().injectMouseButtonDown(mouseState.);
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyClientState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CLobbyClientState::startReleased(const CEGUI::EventArgs& e)
	{
		doStart();
		return true;
	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyClientState::backReleased(const CEGUI::EventArgs& e)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork();
		_app->setState("netmenu");
		return true;

	} // backReleased
			
	//--------------------------------------------------------

	void CLobbyClientState::doStart()
	{
		// Deshabilitamos el botón de Start
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Start")->setEnabled(false);
		
		// Actualizamos el status
		CEGUI::Window * status = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Status");
		status->setText("Status: Connecting...");

		// Obtenemos la ip desde el Editbox
		CEGUI::String ip = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyClient/Editbox")->getText();

		// Conectamos
		Net::CManager::getSingletonPtr()->connectTo((char*)ip.c_str(),1234,1);

		// Actualizamos el status
		status->setText("Status: Connected to server. Waiting to start game...");

		//tickNet is in charge of receiving the message and change the state

	} // doStart

} // namespace Application
