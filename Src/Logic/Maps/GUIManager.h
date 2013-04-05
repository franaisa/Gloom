#include <string>
#include <map>
#include <vector>


#ifndef __Logic_GUIManager_H
#define __Logic_GUIManager_H


//predeclaracion de clases

namespace Hikari{
	class FlashControl;
}


namespace Logic
{
	/**
	Clase que representa la GUI en la lógica.
	<p>
	Esta clase se encarga de manejar todos los eventos de GUI que tienen que ver con la lógica 
	del juego (menus, minimap, etc).
	</p>

	@ingroup logicGroup
	@ingroup mapGroup

	@author Rubén Mulero
	@date April, 2013
	*/
	class CGUIManager
	{
	public:

		/**
		Devuelve la única instancia de la clase CGUIManager.
		
		@return Única instancia de la clase CGUIManager.
		*/
		static CGUIManager* getSingletonPtr() {return _instance;}

		/**
		Método que añade un GUI control al manager de GUIs, para que podamos invocar 
		sus métodos

		@param overlay El overlay en flash que estamos añadiendo al manager
		@param name el nombre que le hemos asignado al overlay (debe ser único)

		@return true si todo fue bien, false en caso contrario
		**/
		bool addGUI(Hikari::FlashControl* overlay,const std::string &name);

		/**
		Método que devuelve el control que maneja la GUI

		@param name el nombre de la GUI que estamos pidiendo

		@return Un puntero al control de la GUI
		*/
		Hikari::FlashControl* getGUIControl(const std::string &name);

		void deleteGUI(const std::string &gui);

		void showGUI(const std::string &name);

		void hideGUI(const std::string &name);

		template<class T, typename ReturnType>
		void addCallback(const std::string &gui, const std::string &funcName, T instance, ReturnType (T::*function)());

		void callFunction(const std::string &gui, const std::string &function);

		void callFunction(const std::string &gui, const std::string &function, const std::vector<std::string> &stringPars, 
						const std::vector<float> &floatPars, 
						const std::vector<int> &intPars);

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		void activate();

	protected:

		/**
		Constructor.
		*/
		CGUIManager ();

		/**
		Destructor.
		*/
		~CGUIManager();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

	private:
		//unica instancia de singleton
		static CGUIManager* _instance;

		//tabla hash con todos los guis cargados para ser utilizados en la logica
		typedef std::pair<std::string ,Hikari::FlashControl*> GUIPair;
		typedef std::map<std::string ,Hikari::FlashControl*> TGUI;
		TGUI _loadedGUIs;

	};//class CGUIManager
}//nampespace Logic

#endif // __Logic_GUIManager_H