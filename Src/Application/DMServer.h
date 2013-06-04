//---------------------------------------------------------------------------
// DMServer.h
//---------------------------------------------------------------------------

/**
@file DMServer.h

@see Application::CDMServer
@see Application::CGameServerState

@author Francisco Aisa García
@date Junio, 2013
*/

#ifndef __Application_DMServer_H
#define __Application_DMServer_H

#include "GameServerState.h"

namespace Logic {
	class CEntity;
}

namespace Application {
	
	/**
	@ingroup applicationGroup

	@author Francisco Aisa García
	@date Junio, 2013
	*/

	class CDMServer : public CGameServerState {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CDMServer(CBaseApplication* app);

		void setGameConfig(const std::pair<unsigned int, unsigned int>& timeLimit, unsigned int fragLimit, bool voteKick = false);

		virtual void gameEventOcurred(Logic::CEntity* emitter, const std::shared_ptr<Logic::CMessage>& msg);


		// =======================================================================
		//                           METODOS HEREDADOS
		// =======================================================================

		virtual void tick(unsigned int msecs);

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

		//______________________________________________________________________________

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

		//______________________________________________________________________________

		// Métodos de CMouseListener

		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const Input::CMouseState &mouseState);

		//______________________________________________________________________________

		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const Input::CMouseState &mouseState);

		//______________________________________________________________________________

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

		void endGame();

		bool isPlayer(Logic::CEntity* entity);

		int _time;
		unsigned int _fragLimit;
		bool _voteKick;
		bool _voteMap;
		bool _inEndGame;
		bool _autoChangeMap;

	}; // CDMServer

} // namespace Application

#endif // __Application_DMServer_H
