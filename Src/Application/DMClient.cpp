//---------------------------------------------------------------------------
// DMClient.cpp
//---------------------------------------------------------------------------

/**
@file DMClient.cpp

@see Application::CDMClient
@see Application::CGameClientState

@author Francisco Aisa García
@date Junio, 2013
*/

#include "DMClient.h"

namespace Application {

	void CDMClient::activate() {
		CGameClientState::activate();
	} // activate

	//______________________________________________________________________________

	void CDMClient::deactivate() {
		CGameClientState::deactivate();
	} // deactivate

	//______________________________________________________________________________

	bool CDMClient::keyPressed(Input::TKey key) {
		return CGameClientState::keyPressed(key);
	} // keyPressed

	//______________________________________________________________________________

	bool CDMClient::keyReleased(Input::TKey key) {
		return CGameClientState::keyReleased(key);
	} // keyReleased

	//______________________________________________________________________________

	bool CDMClient::mouseMoved(const Input::CMouseState &mouseState) {
		return false;
	} // mouseMoved

	//______________________________________________________________________________

	bool CDMClient::mousePressed(const Input::CMouseState &mouseState) {
		return false;
	} // mousePressed

	//______________________________________________________________________________

	bool CDMClient::mouseReleased(const Input::CMouseState &mouseState) {
		return false;
	} // mouseReleased
	
} // namespace Application
