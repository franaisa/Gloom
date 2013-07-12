//---------------------------------------------------------------------------
// PreloadResourceManager.h
//---------------------------------------------------------------------------

/**
@file PreloadResourceManager.h

Contiene la declaración del gestor de precarga de recursos de cada nivel.

@see Logic::CPreloadResourceManager

@author Jose Antonio García Yáñez
@date Julio, 2013
*/

#ifndef __PreloadResourceManager_H
#define __PreloadResourceManager_H

#include <string>

namespace Logic 
{
	/**
	Este módulo es un singleton que se usa como manager de la precarga de recursos.

	@author Jose Antonio García Yáñez
	@date Julio, 2013
	*/

	class CPreloadResourceManager
	{
	public:

		/**
		Devuelve la única instancia de la clase CPreloadResourceManage.
		
		@return Única instancia de la clase CPreloadResourceManage.
		*/
		static CPreloadResourceManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CPreloadResourceManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Función llamada para declarar e inicializar los recursos comunes.
		*/
		void preloadCommon();

		/**
		Función llamada para la precarga de recursos del nivel pasado como parámetro.

		@param filename Nombre del mapa a cargar sus recursos.
		*/
		void preloadResources(const std::string &mapName);

		/**
		Función llamada para la el borrado de recursos del nivel pasado como parámetro.

		@param filename Nombre del mapa a borrar sus recursos.
		*/
		void unloadResources(const std::string &mapName);

	protected:
		
		/** 
		Constructor de la clase 
		*/
		CPreloadResourceManager();

		/** 
		Destructor 
		*/
		~CPreloadResourceManager();

	private:
		/**
		Única instancia de la clase.
		*/
		static CPreloadResourceManager* _instance;


	}; // CPreloadResourceManager

} // namespace Logic

#endif //  __PreloadResourceManager_H
