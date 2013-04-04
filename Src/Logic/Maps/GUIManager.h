


#ifndef __Logic_GUIManager_H
#define __Logic_GUIManager_H


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

		**/
		bool addGUI();

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
		static CGUIManager* _instance;

	};//class CGUIManager
}//nampespace Logic

#endif // __Logic_GUIManager_H