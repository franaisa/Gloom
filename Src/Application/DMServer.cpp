//---------------------------------------------------------------------------
// DMServer.cpp
//---------------------------------------------------------------------------

/**
@file DMServer.cpp

@see Application::CDMServer
@see Application::CGameServerState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "DMServer.h"

namespace Application {

	void CDMServer::activate() {
		CGameServerState::activate();
	} // activate

	//______________________________________________________________________________

	void CDMServer::deactivate() {
		CGameServerState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	bool CDMServer::keyPressed(Input::TKey key) {
		return CGameServerState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CDMServer::keyReleased(Input::TKey key) {
		return CGameServerState::keyReleased(key);
	} // keyReleased

	//______________________________________________________________________________

	bool CDMServer::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CDMServer::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CDMServer::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
