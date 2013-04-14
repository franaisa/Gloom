//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchClientState.h
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchClientState.h

Contiene la declaración del estado de juego.

@see Application::CGameState
@see Application::CMultiplayerTeamDeathmatchClientState

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Application_MultiplayerTeamDeathmatchClientState_H
#define __Application_MultiplayerTeamDeathmatchClientState_H

#include "GameState.h"
#include "Net/Manager.h"
#include "FlashValue.h"
#include <iostream>

namespace Hikari{
	class FlashControl;
}

namespace Application {
	
	/**
	@ingroup applicationGroup

	Representa el estado de juego online para el cliente. En el futuro
	tendremos diferentes estados por cada modo de juego, pero de momento
	se queda asi hasta que por lo menos tengamos un deathmatch.

	Hereda de CGameState por que es un estado de juego y de Net::CManager::IObserver
	para que se le avise de que ha recibido paquetes (en este caso los
	paquetes de datos son los unicos que el cliente va a querer).

	@author Francisco Aisa García
	@date Febrero, 2013
	@deprecated Actualmente hereda de IObserver pero IObserver tiene listeners
	que no interesan al cliente (como las llamadas de conexion y desconexion).
	*/

	class CMultiplayerTeamDeathmatchClientState : public CGameState, public Net::CManager::IObserver {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CMultiplayerTeamDeathmatchClientState(CBaseApplication *app) : CGameState(app) {}


		// =======================================================================
		//                           METODOS HEREDADOS
		// =======================================================================

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/

		virtual bool init();

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
		Se dispara cuando se recibe un paquete de conexion. En el caso del cliente
		este metodo no deberia dispararse nunca.

		@see Net::CManager::IObserver
		@param packet Paquete de conexion recibido.
		*/
		virtual void connexionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }

		//______________________________________________________________________________

		/**
		Se dispara cuando se recibe un paquete de desconexion. En el caso del cliente
		este metodo no deberia dispararse nunca.

		@see Net::CManager::IObserver
		@param packet Paquete de desconexion recibido.
		*/
		virtual void disconnexionPacketReceived(Net::CPaquete* packet) { /* Los clientes no reciben este tipo de mensajes */ }

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

		Hikari::FlashValue CMultiplayerTeamDeathmatchClientState::classSelected(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	

	protected:

		unsigned int _npings;
		unsigned int _time;
		unsigned int _pingActual;

		//menu de seleccion de personaje
		Hikari::FlashControl* _seleccion;
	}; // CMultiplayerTeamDeathmatchClientState

} // namespace Application

#endif //  __Application_GameState_H
