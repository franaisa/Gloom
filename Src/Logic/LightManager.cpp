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
		_instance->_activeLights.clear();
	} // deactivate

	//______________________________________________________________________________

	void CLightManager::createLight(Graphics::CLight* & light, Logic::CLight* lightComp, Graphics::LightType::Enum lightType, bool controlledByManager, 
									const Vector3& position, const Vector3& direction) {

		if(light != NULL) 
			return;

		if(!controlledByManager) {
			light = new(nothrow) Graphics::CLight(lightType, position, direction);
		}
		else {
			// En caso de sobrepasar el tope de luces, borramos la mas antigua
			light = new(nothrow) Graphics::CLight(lightType, position, direction);
			if(light) {
				_activeLights.push_front( std::pair<Graphics::CLight*, Logic::CLight*>(light, lightComp) );

				if(_activeLights.size() > MAX_LIGHTS) {
					auto it = _activeLights.rbegin();
					it->second->turnOff();
				}
			}
		}
	}

	//______________________________________________________________________________

	void CLightManager::destroyLight(Graphics::CLight* light, bool controlledByManager) {
		if(light != NULL) {
			if(controlledByManager) {
				// Se entiende que el numero de luces controladas por el manager es muy reducido
				// y que por lo tanto esta busqueda es permisiva
				for(auto it = _activeLights.begin(); it != _activeLights.end(); ++it) {
					if(it->first == light) {
						delete it->first;
						_activeLights.erase(it);

						break;
					}
				}
			}
			else {
				delete light;
			}
		}
	}
}