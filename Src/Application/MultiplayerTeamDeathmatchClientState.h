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

namespace Application {
	/**
	@ingroup applicationGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/
	// Heredar de Net::IObserver para realizar una desconexion explicita
	// de lo contrario el timeout dispara el envio del paquete de desconexion
	class CMultiplayerTeamDeathmatchClientState : public CGameState {
	public:
		/** Constructor de la clase */
		CMultiplayerTeamDeathmatchClientState(CBaseApplication *app) : CGameState(app) {}
	}; // CMultiplayerTeamDeathmatchClientState

} // namespace Application

#endif //  __Application_GameState_H
