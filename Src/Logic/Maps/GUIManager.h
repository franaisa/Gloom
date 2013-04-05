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
	del juego (menus, minimap, etc). Esta clase permite que todas las GUIs estén centralizadas
	y que cualquier evento o llamada a función de las GUIs tenga que pasar por aquí, de manera
	que queda el manejo de GUIs abstraído del resto de la lógica
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

		/**
		Método que borra una gui del manager y libera la memoria asociada a ella
		@param gui el nombre de la gui que se quiere borrar
		*/
		void deleteGUI(const std::string &gui);

		/**
		Método que le dice a una gui concreta que se muestre en la pantalla

		@param name el nombre de la gui que se desea mostrar
		*/
		void showGUI(const std::string &name);

		/**
		Método que le dice a una gui concreta que se borre de la pantalla y
		deje de ser visible

		@param name El nombre de la gui que se desea ocultar
		*/
		void hideGUI(const std::string &name);
		
		/**
		Método genérico para añadir a la gui que se desea un callback asociado a una función
		de la clase llamante. Asocia la función que se le pasa como parámetro al flash control
		correspondiente para que cuando éste llame a la función desde flash, le llegue al 
		método de la clase correspondiente

		@param gui el nombre de la gui a la que quiero poner el callback
		@param funcName el nombre de la función que debe llamar flash para que se ejecute el
		callback
		@param instance la instancia de la clase a la que llama el callback
		@param function la función que actúa como callback
		*/
		template<class T, typename ReturnType>
		void addCallback(const std::string &gui, const std::string &funcName, T instance, ReturnType (T::*function)());

		/**
		Método que llama a una función de flash de una gui concreta sin parámetros.

		@param gui nombre de la gui a la que estoy llamando
		@param function nombre de la función en flash a la que estoy llamando
		*/
		void callFunction(const std::string &gui, const std::string &function);

		/**
		Método que llama a una función de flash de una gui concreta. Se le pueden pasar
		un número variable de parámetros, pero tienen que estar metidos en listas
		concretas para poder llamar a flash. una lista para strings, una para float y una
		para integers.

		@param gui nombre de la gui a la que estoy llamando
		@param function nombre de la función en flash a la que estoy llamando
		@param stringPars lista de parámetros de tipo string que se le pasan a la funcion
		@param floatPars lista de parametros de tipo float que se le pasan a la funcion
		@param intPars lista de parametros de tipo integer que se le pasan a la funcion
		*/
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