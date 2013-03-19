/**
@file MaterialManager.h

@see Physics::CMaterialManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_MaterialManager_H
#define __Physics_MaterialManager_H

#include <vector>

namespace physx {
	class PxMaterial;
	class PxPhysics;
}

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	enum MaterialType {
		eDEFAULT
	};

	/**
	Gestor sencillisimo de materiales, para tenerlos organizados en algun
	sitio. Como en principio vamos a tener pocos materiales, el enumerado
	nos sirve (nada de tablas con strings).

	@ingroup physicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CMaterialManager {
	public:

		/**
		Inicializa la base de datos de la factoría.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la base de datos. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Devuelve un puntero al único objeto de la clase.

		@return Factoría de entidades.
		*/
		static CMaterialManager* getSingletonPtr() { return _instance; }

		physx::PxMaterial* getMaterial(MaterialType material);

	private:

		/**
		Única instancia de la clase.
		*/
		static CMaterialManager *_instance;

		/** 
		Constructor de la clase, protegido, pues es un singleton.
		Registra al objeto como observer del cargador de mapas.
		*/
		CMaterialManager();

		/**
		Destructor protegido, por ser singleton.
		*/
		~CMaterialManager();

		// SDK de Physx
		physx::PxPhysics* _physxSDK;

		std::vector<physx::PxMaterial*> _materialTable;

	}; // CMaterialManager

}; // namespace Physics

#endif // __Physics_MaterialManager_H