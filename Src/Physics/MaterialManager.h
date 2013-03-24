/**
@file MaterialManager.h

Contiene la declaración del manager de materiales físicos.

@see Physics::CMaterialManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_MaterialManager_H
#define __Physics_MaterialManager_H

#include <vector>

// Predeclaración de clases
namespace physx {
	class PxMaterial;
	class PxPhysics;
}

namespace Physics {

	// Alias para evitar acoplamiento en el proyecto de lógica
	typedef physx::PxMaterial Material;

	// Enumerado que contiene la lista de materiales que podemos usar
	enum MaterialType {
		eDEFAULT
	};


	/**
	Gestor que mantiene una tabla de materiales físicos que iremos asignando
	a los actores físicos que creemos. La implementación es prácticamente
	dummy, ya que probablemente no necesitaremos muchos materiales (por
	lo que un vector indexado mediante enumerados es más que suficiente).

	@ingroup physicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CMaterialManager {
	public:


		// =======================================================================
		//                 METODOS DE INICIALIZACION Y LIBERACION
		// =======================================================================


		/**
		Inicializa el manager.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/** Libera los recursos reservados por el manager. */
		static void Release();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Devuelve un puntero al único objeto de la clase.

		@return Manager de materiales.
		*/
		static CMaterialManager* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al material físico que buscamos.

		@param material Tipo de material que queremos.
		*/
		Material* getMaterial(MaterialType material);

	private:

		
		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase, privado, pues es un singleton. */
		CMaterialManager();

		//________________________________________________________________________

		/** Destructor privado, por ser singleton. */
		~CMaterialManager();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Única instancia de la clase. */
		static CMaterialManager *_instance;

		/** Puntero a la SDK de PhysX, necesario para registrar materiales. */
		physx::PxPhysics* _physxSDK;

		/** Vector que contendrá los materiales registrados. */
		std::vector<physx::PxMaterial*> _materialTable;

	}; // CMaterialManager

}; // namespace Physics

#endif // __Physics_MaterialManager_H