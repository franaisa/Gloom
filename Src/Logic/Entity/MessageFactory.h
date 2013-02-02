/**
@file MessageFactory.h

Contiene la declaración de la clase CMessageFactory, Singleton que almacena 
los punteros a función de funciones de creación componentes (CMessage).

@see Logic::CMessageFactory
@see BaseSubsystems::CFactory
@see Logic::CComponent

@author Rubén Mulero
@date Febrero, 2013
*/
#ifndef __Logic_MessageFactory_H
#define __Logic_MessageFactory_H

#include <string>
#include <map>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CMessage;
}

// Declaración de la clase
namespace Logic
{
	/**
	Factoría de componentes. Extiende la plantilla BaseSubsystems::CFactory
	para CMessage haciendo de ella un singleton por lo que solo habrá una 
	instancia de esta clase.
	<p>
	Es usada para almacenar componentes 
	en Logic::CEntityFactory donde todo el registro de componentes se realiza 
	utilizando macros del preprocesador declarados en Logic::CComponent.

	@ingroup logicGroup
	@ingroup mapGroup

	@author Rubén Mulero
	@date Febrero, 2013
	*/
	
	class CMessageFactory
	{

	protected:
		
		/** 
		Puntero a función.  
		*/
		typedef CMessage* (*FunctionPointer)();
	public:

		/**
		Devuelve la única instancia de la clase CMessageFactory.
		
		@return Única instancia de la clase CMessageFactory.
		*/
		static CMessageFactory* getSingletonPtr();

		/**
		Destructor.
		*/
		~CMessageFactory();

		/**
		Añade una nueva función a la tabla de la factoría.

		@param funcion Función de creación del objeto.
		@param name Nombre con el que nos referiremos a la función para
		la futura creación del objeto.
		*/
		void add(FunctionPointer function, const std::string& name);

		/**
		Si una función ya está contenida en la tabla.

		@param name Nombre de la clase del objeto que se quiere saber si 
		se puede crear.
		@return true si se encuentra en la tabla.
		*/
		bool has(const std::string& name) const;

		/**
		Crea una nueva instancia de la clase requerida.

		@param name Nombre de la clase del objeto que se quiere crear.
		@return Nueva instancia de la clase. 0 si no está en la tabla.
		*/
		
		CMessage * create(const std::string& name) const;


		/**
		Crea una nueva instancia de la clase requerida.

		@param name Nombre de la clase del objeto que se quiere crear.
		@return Nueva instancia de la clase. 0 si no está en la tabla.
		*/
		
		CMessage * create(int name) const;

	protected:
		/**
		Constructor.
		*/
		CMessageFactory ();

		typedef std::pair <int,FunctionPointer> TStringFunctionPointerPair;
		typedef std::map<int,FunctionPointer> TFunctionPointerMap;

		TFunctionPointerMap _table;

	private:
		/**
		Única instancia de la clase.
		*/
		static CMessageFactory* _instance;

		

	}; // class CMessageFactory

} // namespace Logic

#endif // __Logic_MessageFactory_H
