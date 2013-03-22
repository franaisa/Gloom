/**
@file ErrorManager.h

Contiene la declaración del gestor de errores para PhysX.

@see Physics::CErrorManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_ErrorManager_H
#define __Physics_ErrorManager_H

#include <foundation/PxErrorCallback.h>

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Gestor de errores para PhysX. Cuando PhysX detecta que se intenta hacer algo incorrecto 
	(por ejemplo, llamadas a funciones con parámetros no válidos), invoca a este gestor 
	que a su vez imprime el mensaje en la consola.

	@ingroup physicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CErrorManager : public physx::PxErrorCallback {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CErrorManager();

		//________________________________________________________________________

		/** Destructor. */
		~CErrorManager();


		// =======================================================================
		//                   METODOS HEREDADOS DE PxErrorCallback
		// =======================================================================


		/** 
		Método invocado por PhysX cada vez que se produce un error. El mensaje de error se 
		imprime por pantalla.

		@param code Código del error.
		@param message Mensaje a imprimir.
		@param file Fichero en el que ocurrió el error.
		@param line Línea donde ocurrió el error.
		*/
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);

	}; // classCErrorManager

}; // namespace Physics

#endif // __Physics_ErrorManager_H