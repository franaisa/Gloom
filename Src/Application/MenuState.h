//---------------------------------------------------------------------------
// MenuState.h
//---------------------------------------------------------------------------

/**
@file MenuState.h

Contiene la declaración del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Application_MenuState_H
#define __Application_MenuState_H

#include "ApplicationState.h"
#include "Hikari.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
	class CBaseApplication;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del menú
	principal del juego. Es muy sencilla y lo único que hace es cargar
	un layout de Hikari al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haciéndo visible/invisible también
	el puntero del ratón). También asocia los eventos de los botones 
	del menú a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es Hikari dependiente, lo cual no es deseable, la aplicación
	debería ser independiente de las tecnologías usadas.

	@ingroup applicationGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CMenuState : public CApplicationState 
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CMenuState(CBaseApplication *app) : CApplicationState(app)
				{}

		/** 
		Destructor 
		*/
		virtual ~CMenuState();

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Función llamada cuando se elimina ("desengancha") el
		estado de la aplicación.
		*/
		virtual void release();

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/**
		Función llamada por la aplicación para que se ejecute
		la funcionalidad del estado.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

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

		void netError();

	private:

		/**
		Ventana de hikari que muestra el menu
		*/
		Hikari::FlashControl* _menu;
		
		/**
		Función que se quiere realizar cuando se pulse el botón start.
		Simplemente cambia al estado de juego.
		*/
		Hikari::FlashValue startReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		/**
		Función que se quiere realizar cuando se pulse el botón exit.
		Simplemente termina la aplicación.
		*/
		Hikari::FlashValue onExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		Hikari::FlashValue multiplayerReleased(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
