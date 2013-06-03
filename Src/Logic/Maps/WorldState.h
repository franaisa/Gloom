//---------------------------------------------------------------------------
// WorldState.h
//---------------------------------------------------------------------------

/**
@file WorldState.h

Contiene la implementación del estado del mundo.

@author Rubén Mulero Guerrero
@date May, 2013
*/

#ifndef __Logic_WorldState_H
#define __Logic_WorldState_H

#include "Logic/Maps/EntityID.h"
#include "Logic/Messages/Message.h"
#include "Net/Buffer.h"

#include <map>
#include <vector>

namespace Logic{
	//forward declarations
	class CEntity;

/**
Clase que guarda todas las entidades que han sido creadas en tiempo de 
ejecución y que para cada una de ellas almacena en qué estado se encuentra.
También almacena la información de aquellas entidades que han sido 
modificadas y cuya información es relevante para el otro lado de la red.
<p>
Es una clase singleton que contiene una estructura de datos que almacena
las entidades creadas/modificadas y un pool con el último mensaje de cada
tipo relevante a ser enviado por la red.
</p><p>
Cuando una entidad se crea dinámicamente, la factoría ha de llamar a esta
clase para introducir los cambios, así como cuando se destruye una entidad.
Cuando se produzca un cambio relevante, el componente correspondiente debe
comunicarse con esta clase para introducir el cambio que se ha producido.
</p>

*/
	class CWorldState{
	public:

		/**
		Para registrar observadores que quieran subscribirse a un determinado
		tipo de eventos. Generalmente será el caso de los modos de juego.

		Previamente los modos de juego (u otras entidades interesadas) deberán
		registrarse especificando la máscara de mensajes que les interesa.
		*/
		class IObserver {
		public:
			virtual void gameEventOcurred(std::shared_ptr<Logic::CMessage> msg) = 0;
		};

		/**
		Estructura donde guardamos la información de una entidad relevante
		para el otro lado de la red
		*/
		struct EntityInfo{
			Logic::TEntityID id;
			CEntity* entity;
			std::map<unsigned int, std::shared_ptr<CMessage>> messages;
		};

		// =======================================================================
		//                          METODOS ESTÁTICOS
		// =======================================================================

		CWorldState();

		~CWorldState(){_instance = 0;}

		/**
		Devuelve la única instancia de la clase CWorldState.
		
		@return Única instancia de la clase CWorldState.
		*/
		static CWorldState* getSingletonPtr(){return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CWorldState. Debe llamarse en
		la destrucción de las estructuras de la lógica
		*/
		static void Release();

		// =======================================================================
		//                          METODOS PÚBLICOS
		// =======================================================================

		void addEntity(CEntity* entity);

		void deleteEntity(CEntity* entity);

		void addChange(CEntity* entity, std::shared_ptr<CMessage> message);

		void deleteChange(CEntity* entity, unsigned int messageType);

		void clearEntities();

		void addObserver(IObserver* listener, const std::vector<TMessageType>& eventsMask);

		void removeObserver(IObserver* listener);

		/**
		Método que serializa la información contenida en el estado del mundo y la deja
		preparada en un buffer para enviarla por la red.
		Para poder deserializar la información, este buffer debe pasarse al método
		deserialize desde el otro lado de la red, ya que esta clase es la que sabe
		cómo la información es almacenada en el buffer.

		@return El buffer con toda la información del estado del mundo lista para 
		enviar por la red.
		*/
		Net::CBuffer serialize();

		/**
		Método que dado un buffer deserializa toda la información y la carga en el
		mundo para que esté actualizado con el servidor.
		Para que este método funcione correctamente, el buffer debe haber sido 
		serializado por el método deserialize de esta clase desde el otro lado de la 
		red.

		@param worldState Buffer que contiene toda la información del estado del
		mundo.
		*/
		void deserialize(Net::CBuffer &worldState);

		/**
		Método que coge toda la información del worldstate y la propaga por las 
		entidades para que se actualicen.
		OJO! este método solamente debe ser llamado en la activación del mapa!!!!!!
		*/
		void updateEntities();

		// =======================================================================
		//                          METODOS PROTEGIDOS
		// =======================================================================

	protected:


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

		// =======================================================================
		//                          MIEMBROS DE CLASE
		// =======================================================================
		
		/**
		Única instancia de la clase.
		*/
		static CWorldState* _instance;

		/**
		Tabla hash donde almacenamos que players han sido creados/modificados
		*/
		std::map<TEntityID,EntityInfo> _entities;

		/**
		@deprecated

		En el futuro sera un vector de observadores-máscara. Por el momento como
		las máscaras de mensajes no están implementadas, se queda así.
		*/
		std::vector< std::pair<IObserver*, std::vector<TMessageType> > > _observers;

		/**
		Comfort typedefs
		*/
		typedef std::pair<TEntityID, EntityInfo> TEntityInfo;
		typedef std::pair<unsigned int, std::shared_ptr<CMessage>> TInfo;
	};

} // namespace Logic

#endif // __Logic_WorldState_H