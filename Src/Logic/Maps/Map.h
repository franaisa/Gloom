/**
@file Map.h

Contiene la declaración de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Map_H
#define __Logic_Map_H

#include <map>
#include <set>
#include <list>
#include <unordered_map>
#include "EntityID.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CEntity;
}

namespace Map
{
	class CEntity;
}

namespace Graphics 
{
	class CScene;
}

// Declaración de la clase
namespace Logic
{
	/**
	Clase que representa un mapa lógico.
	<p>
	Esta clase se encarga de almacenar todas las entidades del mapa, tanto 
	aquellas que tienen representación gráfica y se ven como entidades
	puramente lógicas. Gestiona la activación y desactivación de éstas y
	tiene también métodos para buscar entidades, tanto por su nombre como 
	por su tipo y por su identificador.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CMap {
	public:

		/**
		Método factoría que carga un mapa de fichero. Tras el parseo de
		todas las entidades del mapa mediante CMapParser, genera todas las
		entidades con CEntityFactory.

		@param filename Nombre del archivo a cargar.
		@return Mapa generado.
		*/
		static CMap *createMapFromFile(const std::string &filename);

		/**
		Constructor.

		@param name Nombre que se le da a este mapa.
		*/
		CMap (const std::string &name);

		/**
		Destructor.
		*/
		~CMap();
		
		/**
		Activa el mapa. Invocará al método activate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true al invocador si todas las entidades se
		activaron sin problemas.
		*/
		bool activate();

		/**
		Desactiva el mapa. Invocará al método deactivate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Función llamada tras la carga del mapa antes de que se ejecute
		el primer tick.
		*/
		void start();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Llamará a los métodos tick() de todas las entidades.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Añade una nueva entidad al mapa. Si la entidad ya estaba incluida
		no se hace nada.

		@param entity Entidad a añadir.
		*/
		void addEntity(CEntity *entity);

		/**
		Elimina una entidad del mapa. Si la entidad no estaba incluida
		no se hace nada. La función desactiva previamente la entidad si
		ésta se encontraba activa.

		@note El mapa no se hace responsable de eliminar (con delete) la
		entidad.

		@param entity Entidad a eliminar.
		*/
		void removeEntity(CEntity *entity);

		/**
		Elimina y destruye todas las entidades del mapa dejando la lista 
		de entidades vacía.
		*/
		void destroyAllEntities();

		/**
		Recupera una entidad del mapa a partir de su ID.

		@param entityID ID de la entidad a recuperar.
		@return Entidad con el ID pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByID(TEntityID entityID);

		/**
		Recupera una entidad del mapa a partir de su nombre.

		@param name nombre de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. Útil si se tienen varias entidades con el mismo
		nombre y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByName(const std::string &name);

		/**
		Recupera una entidad del mapa a partir de su tipo.

		@param type nombre del tipo de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. Útil si se tienen varias entidades del mismo tipo
		y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		std::list<CEntity*> getEntitiesByType(const std::string &type);

		/**
		Devuelve la escena gráfica correspondiente a este mapa.

		@return Escena con las entidades gráficas.
		*/
		Graphics::CScene *getScene() {
			return _scene;}
		
		std::string getMapName(){return _name;}

		/**
		Crea un nuevo jugador y le porporciona un nombre determinado.

		@param name Nombre del jugador.
		*/

		CEntity* createPlayer(const std::string &name, const std::string &type);

		/**
		Crea un nuevo jugador y le porporciona un nombre determinado.

		@param name Nombre del jugador.
		@param id La id de la entidad que devuelve el método.
		*/
		CEntity* createLocalPlayer(const std::string &name, const std::string &type, TEntityID id);

		CEntity* createPlayer(const std::string &name, const std::string &type, TEntityID id);

		/**
			Método que elimina una entidad en el siguiente tick del mapa
			@param entity Entidad que será borrada
		*/
		void deleteDeferredEntity(CEntity* entity);

		void entityTimeToLive(CEntity* entity, unsigned int msecs);

		void setFixedTimeStep(unsigned int stepSize);

		void wantsTick(CEntity* entity);

		void wantsFixedTick(CEntity* entity);

	private:

		void checkTimeouts(unsigned int msecs);

		void processComponentMessages();

		void doTick(unsigned int msecs);

		void doFixedTick(unsigned int msecs);

		/**
		Lista de entidades que han sido marcadas para ser borradas en
		un tiempo dado.
		*/
		std::list< std::pair<CEntity*, unsigned int> > _entitiesWithTimeout;

		struct EntityInfo {
			CEntity* _entityPtr;
			std::list<CEntity*>::const_iterator _processIterator;
			std::list<CEntity*>::const_iterator _tickIterator;
			std::list<CEntity*>::const_iterator _fixedTickIterator;
		};

		//std::unordered_map<TEntityID, EntityInfo> _entityInfoTable;

		// Hash de punteros a informacion de entidades
		std::unordered_map<TEntityID, EntityInfo*> _entityIdLookupTable;
		std::unordered_map<std::string, EntityInfo*> _entityNameLookupTable;
		std::unordered_multimap<std::string, EntityInfo*> _entityTypeLookupTable;
		
		// Listas de entidades a recorrer
		std::list<CEntity*> _entityList;
		std::list<CEntity*> _entitiesWithTick;
		std::list<CEntity*> _entitiesWithFixedTick;

		/**
		Tipo tabla de entidades de mapa.
		*/
		//typedef std::map<TEntityID, CEntity*> TEntityMap;

		/**
		tabla con las entidades del mapa localizadas por su ID.
		*/
		//TEntityMap _entityMap;

		/**
		Lista de entidades que hay que borrar
		*/
		std::list<CEntity*> _deleteEntities;

		/**
		Nombre del mapa.
		*/
		std::string _name;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades.
		*/
		Graphics::CScene* _scene;

		/**
		@deprectaed -> no se usa en ninguna parte.
		Número de jugadores creados hasta el momento.
		*/
		unsigned int _numOfPlayers;

		/**
		Variable de clase que indica el número de milisegundos que se procesan en cada
		iteración del tick fijo.
		*/
		unsigned int _fixedTimeStep;

		/** Tiempo acumulado a tener en cuenta para el tick fijo. */
		unsigned int _acumTime;

	}; // class CMap

} // namespace Logic

#endif // __Logic_Map_H
