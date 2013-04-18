//---------------------------------------------------------------------------
// GameSpawnManager.h
//---------------------------------------------------------------------------

/**
@file GameSpawnManager.h

Contiene la declaración del gestor de los mensajes de red durante la partida.

@see Logic::CGameSpawnManager

@author Jose Antonio García Yáñez
@date Febrero, 2013
*/

#ifndef __GameSpawnManager_H
#define __GameSpawnManager_H

#include "BaseSubsystems\Math.h"
#include "Entity\Entity.h"

namespace Logic 
{
	/**
	Este módulo es un singleton que se usa como manager de los puntos de spawn.

	@author Jose Antonio García Yáñez
	@date Febrero, 2013
	*/

	class CGameSpawnManager 
	{
	public:

		/**
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CGameSpawnManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CGameSpawnManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada para activar la escucha.
		*/
		void activate();

		/**
		Función llamada desactivar la escucha.
		*/
		void deactivate();


		/**
		Función llamada desactivar la escucha.
		*/
		CEntity* getSpawnPosition();

	protected:
		
		/** 
		Constructor de la clase 
		*/
		CGameSpawnManager();

		/** 
		Destructor 
		*/
		~CGameSpawnManager();

		/**
		Lista de puntos
		*/
		std::vector<CEntity*> _listSpawnPoints;

	private:
		/**
		Única instancia de la clase.
		*/
		static CGameSpawnManager* _instance;

	}; // CGameSpawngManager

} // namespace Logic

#endif //  __GameSpawnManager_H
