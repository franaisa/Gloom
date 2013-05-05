//---------------------------------------------------------------------------
// LobbyServerState.h
//---------------------------------------------------------------------------

/**
@file LobbyServerState.h

Contiene la declaración del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Application_LobbyServerState_H
#define __Application_LobbyServerState_H

#include "ApplicationState.h"
#include "FlashValue.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application {
	class CBaseApplication;
}

namespace Hikari {
	class FlashControl;
}

namespace Application {
	
	/**
	Como su nombre indica, esta clase es la clase del menú
	principal del juego. Es muy sencilla y lo único que hace es cargar
	un layout de Hikari al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haciéndo visible/invisible también
	el puntero del ratón). También asocia los eventos de los botones 
	del menú a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es CEGUI dependiente, lo cual no es deseable, la aplicación
	debería ser independiente de las tecnologías usadas.

	@ingroup applicationGroup

	@author David Llansó
	@date Agosto, 2010
	*/

	class CLobbyServerState : public CApplicationState {
	public:

		/** Constructor de la clase */
		CLobbyServerState(CBaseApplication *app);

		//__________________________________________________________________

		/** 
		Destructor 
		*/
		virtual ~CLobbyServerState();

		//__________________________________________________________________

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		//__________________________________________________________________

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		//__________________________________________________________________

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		//__________________________________________________________________

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

		//__________________________________________________________________
		
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

		//__________________________________________________________________

		// Métodos de CMouseListener
		
		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const Input::CMouseState &mouseState);
		
		//__________________________________________________________________

		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const Input::CMouseState &mouseState);

		//__________________________________________________________________

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

		
		/**
		Función que se quiere realizar cuando se pulse el botón start.
		Simplemente cambia al estado de juego.
		*/
		Hikari::FlashValue startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		//__________________________________________________________________

		/**
		Función que se quiere realizar cuando se pulse el botón back.
		Simplemente cambia al estado de menu.
		*/
		Hikari::FlashValue backReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		//__________________________________________________________________

		/**
		Función que se quiere realizar cuando se pulse el botón create game.
		Simplemente cambia al estado de menu.
		*/
		Hikari::FlashValue createReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		//__________________________________________________________________

		/**
		Método que lista los ficheros que hay en el directorio /media/maps y los muestra en el gui sin la extension
		*/
		void listFiles();



		/**
		layout de hikari que muestra el menu
		*/
		Hikari::FlashControl* _menu;

		/**
		map en el que se va a crear la partida
		*/
		std::string _map;

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
