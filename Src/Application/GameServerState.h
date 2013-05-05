//---------------------------------------------------------------------------
// GameServerState.h
//---------------------------------------------------------------------------

/**
@file GameServerState.h

Contiene la declaración del estado de juego.

@see Application::CGameServerState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Application_GameServerState_H
#define __Application_GameServerState_H

#include "GameState.h"
#include "Net/Manager.h"

namespace Logic {
	class CGameNetPlayersManager;
	class CMap;
}

namespace Application {
	
	/**
	@ingroup applicationGroup

	Representa el estado de juego online para el servidor. En el futuro
	tendremos diferentes estados por cada modo de juego, pero de momento
	se queda asi hasta que por lo menos tengamos un deathmatch.

	Hereda de CGameState por que es un estado de juego y de Net::CManager::IObserver
	para que se le avise de que ha recibido paquetes.

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CGameServerState : public CGameState, public Net::CManager::IObserver {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CGameServerState(CBaseApplication *app) : CGameState(app), _playersMgr(NULL) {}


		// =======================================================================
		//                           METODOS HEREDADOS
		// =======================================================================


		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		//______________________________________________________________________________

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de datos.

		@see Net::CManager::IObserver
		@param packet Paquete de datos recibido.
		*/
		virtual void dataPacketReceived(Net::CPaquete* packet);

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de conexion.

		@see Net::CManager::IObserver
		@param packet Paquete de conexion recibido.
		*/
		virtual void connectionPacketReceived(Net::CPaquete* packet);

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de desconexion.

		@see Net::CManager::IObserver
		@param packet Paquete de desconexion recibido.
		*/
		virtual void disconnectionPacketReceived(Net::CPaquete* packet);

		// Métodos de CKeyboardListener

		/**
		Método que será invocado siempre que se pulse una tecla. 
		Será la aplicación quién llame a este método cuando el 
		estado esté activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyPressed(Input::TKey key);

		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. Será la aplicación quién llame a este método 
		cuando el estado esté activo. Esta clase NO se registra en
		el InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyReleased(Input::TKey key);

		// Métodos de CMouseListener

		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const Input::CMouseState &mouseState);

		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const Input::CMouseState &mouseState);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón. La aplicación avisa de este evento al estado 
		actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. 
		*/
		virtual bool mouseReleased(const Input::CMouseState &mouseState);

	private:

		Logic::CGameNetPlayersManager* _playersMgr;

		Net::CManager* _netMgr;

		Logic::CMap* _map;
	}; // CMultiplayerTeamDeathmatchServerState

} // namespace Application

#endif //  __Application_GameState_H
