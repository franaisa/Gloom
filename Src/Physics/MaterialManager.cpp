/**
@file MaterialManager.cpp

Contiene la implementación del gestor de materiales.

@see Physics::CMaterialManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "MaterialManager.h"
#include "Physics/Server.h"

#include <cassert>

#include <PxMaterial.h>
#include <PxPhysics.h>

using namespace Physics;
using namespace physx;
using namespace std;

namespace Physics {

	CMaterialManager* CMaterialManager::_instance = 0;
	
	//________________________________________________________________________

	// Hacer que el tamaño del vector sea el del numero de materiales
	CMaterialManager::CMaterialManager() {
		_instance = this;

		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");

		// Creamos los materiales
		_materialTable.resize(1); // Cuanta memoria vamos a necesitar?
		_materialTable[MaterialType::eDEFAULT] = _physxSDK->createMaterial(0.5f, 0.5f, 0.1f);
	} // CMaterialManager
	
	//________________________________________________________________________

	CMaterialManager::~CMaterialManager() {
		// Liberamos los materiales que han sido creados
		for(int i = 0; i < _materialTable.size(); ++i) {
			_materialTable[i]->release();
		}

		_instance = NULL;
		_physxSDK = NULL;
	} // ~CMaterialManager
	
	//________________________________________________________________________

	bool CMaterialManager::Init() {
		assert(!_instance && "Segunda inicialización de Physics::CMaterialManager no permitida!");

		return new CMaterialManager();
	} // Init
	
	//________________________________________________________________________

	void CMaterialManager::Release() {
		assert(_instance && "Physics::CMaterialManager no está inicializado!");

		if(_instance) {
			delete _instance;
		}
	} // Release

	//________________________________________________________________________

	Material* CMaterialManager::getMaterial(MaterialType material) {
		return _materialTable[material];
	}

}