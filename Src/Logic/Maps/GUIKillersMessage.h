#include "GUIManager.h"

namespace Logic{
	class CGUIManager;
}

namespace Logic{
/**
Clase que maneja los mensajes que aparecen en la pantalla cuando alguien
mata a otro jugador.
<p>
Es una clase singleton que contiene un overlay flash que es el que se 
encarga de pintar en la pantallalos mensajes. Esta clase es la que se
comunica con el overlay para pasarlela información de lo que debe pintar 
en la pantalla.
</p><p>
Tiene dos modos de funcionamiento. Cuando un player mata a otro, que 
entonces muestra el mensaje de A mato a B, y el de suicidio, que es cuando
alguien se mata solo, que aparece un mensaje distinto indicando que se mato 
el solo
</p>

*/
	class GUIKillersMessage{
	public:
		GUIKillersMessage();

		~GUIKillersMessage(){_instance = 0;}

		/**
		Devuelve la única instancia de la clase GUIKillersMessage.
		
		@return Única instancia de la clase GUIKillersMessage.
		*/
		static GUIKillersMessage* getSingletonPtr(){return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static void Init();

		/**
		Libera la instancia de GUIKillersMessage. Debe llamarse en
		la destrucción de las estructuras de la lógica
		*/
		static void Release();

		/**
		Método que hace que se muestre el mensaje de que alguien mató a 
		otro jugador

		@param killer El jugador que mata
		@param killed El jugador que ha muerto
		*/
		void addKiller(const std::string &killer, const std::string &killed);

		/**
		Método que hace que se muestre un mensaje de que un jugador se ha
		suicidado

		@param name El nombre del jugador que se ha suicidado
		*/
		void suicide(const std::string &name);

	private:
		/**
		Única instancia de la clase.
		*/
		static GUIKillersMessage* _instance;

		/**
		Instancia del GUIManager
		*/
		CGUIManager* _guiManager;
	};

}