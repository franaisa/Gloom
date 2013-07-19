//---------------------------------------------------------------------------
// LightManager.cpp
//---------------------------------------------------------------------------

/**
@file LightManager.cpp

@see Logic::CLightManager

@author Francisco Aisa García
@date Julio, 2013
*/

#include "LightManager.h"
#include "Logic/Entity/Components/Light.h"

#include <assert.h>
#include <new>

using namespace std;

namespace Logic {
	
	CLightManager* CLightManager::_instance = NULL;

	//______________________________________________________________________________

	CLightManager::CLightManager(){
		_instance = this;
		_instance->MAX_LIGHTS = 3;
	} // CServer

	//______________________________________________________________________________

	CLightManager::~CLightManager() {
		_instance = NULL;
	} // ~CServer
	
	//______________________________________________________________________________

	bool CLightManager::Init() {
		assert(!_instance && "Segunda inicialización de Graphics::CGameNetMsgManager no permitida!");

		return new(nothrow) CLightManager();
	} // Init

	//______________________________________________________________________________

	void CLightManager::Release() {
		assert(_instance && "Graphics::CLightManager no está inicializado!");

		delete _instance;
		_instance = NULL;
	} // Release

	//______________________________________________________________________________

	void CLightManager::activate() {
		// De momento no cargamos nada
	} // activate

	//______________________________________________________________________________

	void CLightManager::deactivate() {
		// De momento no hacemos nada
		for(int i = 0; i < _instance->_activeLights.size(); ++i) {
			delete _instance->_activeLights[i];
		}

		_instance->_activeLights.clear();
	} // deactivate

	//______________________________________________________________________________

	Graphics::CLight* CLightManager::createLight(Graphics::LightType::Enum lightType, const std::string& lightName, 
												 const Vector3& position, const Vector3& direction) {

		// Como se trata de un número reducido de luces, podemos hacer la busqueda
		// secuencial
		for(int i = 0; i < _activeLights.size(); ++i) {
			// Si la luz que queremos crear ya ha sido creada, no hacemos nada
			if(_activeLights[i]->getName() == lightName) {
				return _activeLights[i];
			}
		}

		// Si la luz que queremos crear no existe, la creamos y la añadimos a la lista
		// por el principio. En caso de a ver sobrepasado el tope de luces, borramos
		// la mas antigua
		Graphics::CLight* newLight = new(nothrow) Graphics::CLight(lightType, lightName, position, direction);
		if(newLight) {
			_activeLights.push_front(newLight);

			if(_activeLights.size() > MAX_LIGHTS) {
				auto it = _activeLights.rbegin();
				delete *it;
				_activeLights.pop_back();
			}
		}

		return newLight;
	}
}