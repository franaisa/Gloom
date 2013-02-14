//---------------------------------------------------------------------------
// MultiplayerTeamDeathmatchServerState.h
//---------------------------------------------------------------------------

/**
@file MultiplayerTeamDeathmatchServerState.h

Contiene la declaración del estado de juego.

@see Application::MultiplayerTeamDeathmatchServerState
@see Application::CGameState

@author Francisco Aisa García
@date Febrero, 2013
*/

#ifndef __Application_MultiplayerTeamDeathmatchServerState_H
#define __Application_MultiplayerTeamDeathmatchServerState_H

#include "GameState.h"
#include "Net/Manager.h"

#include <iostream>

namespace Application 
{
	/**
	@ingroup applicationGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	class CMultiplayerTeamDeathmatchServerState : public CGameState, public Net::CManager::IObserver {
	public:
		/** Constructor de la clase */
		CMultiplayerTeamDeathmatchServerState(CBaseApplication *app) : CGameState(app) {}

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

		/******************
			IOBSERVER
		******************/
		virtual void dataPacketReceived(Net::CPaquete* packet);
		virtual void connexionPacketReceived(Net::CPaquete* packet);
		virtual void disconnexionPacketReceived(Net::CPaquete* packet);

	}; // CGameState

} // namespace Application

#endif //  __Application_GameState_H
