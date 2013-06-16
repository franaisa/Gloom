//---------------------------------------------------------------------------
// TDMServer.h
//---------------------------------------------------------------------------

/**
@file TDMServer.h

@see Application::CTDMServer
@see Application::CGameServerState

@author Francisco Aisa García
@date Junio, 2013
*/

#ifndef __Application_TDMServer_H
#define __Application_TDMServer_H

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

	class CTDMServer : public CGameServerState {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor por parametro.

		@param app Aplicacion que se encarga de manejar los estados.
		*/
		CTDMServer(CBaseApplication* app);

		void gameSettings(const std::vector<std::string>& mapList, bool loopMaps, const std::pair<unsigned int, unsigned int>& timeLimit, 
						  unsigned int goalScore, bool forceRespawn, bool warmUp, bool autoBalanceTeams);

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

		// Game Settings
		bool _forceRespawn;
		bool _warmUp;
		bool _autoBalanceTeams;

		// Map Control
		std::vector<std::string> _mapList; // Se respeta el orden que se le haya dado
		bool _loopMaps;
		unsigned int _currentMap;

		// Variables
		bool _inEndGame;
		unsigned int _blueTeamScore;
		unsigned int _redTeamScore;

	}; // CTDMServer

} // namespace Application

#endif // __Application_DMServer_H
