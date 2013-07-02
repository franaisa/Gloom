/**
@file Entity.h

Contiene la declaración de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author Francisco Aisa García
@author Jose Antonio García Yáñez
@date Junio, 2013
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"
#include "Logic/Maps/EntityID.h"
#include "Logic/Messages/Message.h"
#include "Logic/Maps/ComponentID.h"

#include <list>
#include <string>
#include <map>
#include <set>
#include <memory>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map {
	class CEntity;
}

namespace Logic {
	class CMap;
	class IComponent;
	class CEntityFactory;
}

// Declaración de la clase
namespace Logic {


	/**
	Contiene un enumerado que representa los estados
	de un componente.
	*/
	struct ComponentState {
		enum Enum {
			eSLEEPING		= 1 << 0, // (Durmiendo) Recibe mensajes pero no tiene tick, despierta al recibir un mensaje
			eAWAKE			= 1 << 1, // (Despierto/Disponible) Recibe mensajes y tiene tick
			eBUSY			= 1 << 2  // (Ocupado) No recibe mensajes pero tiene tick
		};
	};


	/**
	Contiene un enumerado que indica el tipo de tick que
	tiene el componente.
	*/
	struct TickMode {
		enum Enum {
			eNONE			= 0,	  // No tiene tick
			eTICK			= 1 << 0, // Tiene tick
			eFIXED_TICK		= 1 << 1, // Tiene tick fijo
			eBOTH			= eTICK | eFIXED_TICK
		};
	};


	struct Orientation {
		enum Enum {
			eYAW		= 0,
			ePITCH		= 1,
			eROLL		= 2  
		};
	};

	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cuáles tenga. Además, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creación de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocará a su método spawn() en el
	que se inicializarán los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en algún momento, cuando se active
	el mapa se llamará al método activate() de la entidad. Tras el activate
	se llamará al start, para garantizar labores de inicialización en las que
	es necesario que el resto de componentes hayan ejecutado el spawn. En 
	este momento los componentes harán visibles los componentes gráficos y 
	demás ya que apartir de ahí es cuando la entidad empieza su función 
	siendo actualizada por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author Francisco Aisa García
	@author Jose Antonio García Yáñez
	@date Abril, 2013
	*/

	class CEntity {
	public:


		// =======================================================================
		//                       MÉTODOS DE CENTRALES
		// =======================================================================

		
		/**
		Activa la entidad. Cuando el mapa se activa se activan todas las 
		entidades. Sin embargo, las entidades también se pueden activar
		desde la lógica del juego.

		El método llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();

		//__________________________________________________________________

		/**
		Desactiva la entidad. Cuando el mapa se desactiva se desactivan todas
		las entidades. Sin embargo, las entidades también se pueden desactivar
		desde la lógica del juego.
		
		El método llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		//__________________________________________________________________________

		/**
		Desactiva todos los componentes excepto los indicados en el set pasado
		por parámetro.

		@param id string que identifica al componente. Es el mismo nombre que el
		asignado en el blueprints.
		*/
		void deactivateAllComponentsExcept(const std::set<std::string>& exceptionList);

		//__________________________________________________________________________

		/**
		Desactiva los componentes que se indiquen.

		@param componentList Array que contiene la lista de componentes que hay que
		activar.
		*/
		void deactivateComponents(const std::vector<std::string>& componentList);

		//__________________________________________________________________

		/**
		Función llamada tras la activación de la entidad antes de que se ejecute
		el primer tick. Se garantiza que todas las entidades han sido creadas
		con sus componentes inicializados.

		Esta función SOLO se ejecuta una vez al principio.
		*/
		void start();

		//__________________________________________________________________

		void processComponentMessages();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.

		Se encarga de que se procesen los mensajes y se llamen a los
		ticks de los componentes que posee. Dependiendo del estado
		de cada componente se harán unas cosas u otras.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		bool tick(unsigned int msecs);

		bool fixedTick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si éste se especifica en el segundo campo.

		Por supuesto la emisión del mensaje dependerá del estado de cada 
		componente.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviará el mensaje.
		@return true si al menos un componente aceptó el mensaje
		*/
		bool emitMessage(const std::shared_ptr<CMessage>& message, IComponent* emitter = 0);

		//__________________________________________________________________

		/**
		Método que añade un nuevo componente a la lista de la entidad.

		@param component Componente a añadir.
		*/
		void addComponent(IComponent* component, const std::string& id);

		//__________________________________________________________________

		/**
		Método que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borró el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);


		// =======================================================================
		//          MÉTODOS DE ROTACIÓN Y POSICIONAMIENTO DE LA ENTIDAD
		// =======================================================================


		/**
		Devuelve la posición de la entidad.
		<p>
		La posición es inicialmente leída del mapa (si no aparece,
		se colocará a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posición de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _position; }

		//__________________________________________________________________

		/**
		Devuelve el quaternion de rotación de la entidad.
		<p>
		La orientación es inicialmente leída del mapa como un simple 
		viraje (si no aparece, se asignará el quaternion identidad), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientación en el entorno.
		*/
		Ogre::Quaternion getOrientation() const;
		//__________________________________________________________________

		/**
		Establece la posición de la entidad.

		@param position Nueva posición.
		*/
		void setPosition(const Vector3 &position);

		//__________________________________________________________________

		
		/**
		Establece la orientación de la final de la entidad.
		Las orientaciones parciales tambien serán actualizadas.

		@param pos Nueva orientación.
		*/
		void setOrientation(const Quaternion& orientation);

		//__________________________________________________________________

		
		/**
		Devuelve el Yaw de la entidad.

		@return _yawOrientation Quaternion del yaw.
		*/
		Quaternion getYaw();

		//__________________________________________________________________

		/**
		Devuelve el Pitch de la entidad.

		@return _pitchOrientation Quaternion del pitch.
		*/
		Quaternion getPitch();

		//__________________________________________________________________

		/**
		Devuelve el Roll de la entidad.

		@return _rollOrientation Quaternion del roll.
		*/
		Quaternion getRoll();

		//__________________________________________________________________
		/**
		Establece el Yaw de la entidad.

		@param yaw Orientacion nueva para el yaw.
		*/
		void setYaw(const Quaternion &yaw);

		//__________________________________________________________________

		/**
		Establece el Pitch de la entidad.

		@param pitch Orientacion nueva para el pitch.
		*/
		void setPitch(const Quaternion &pitch);

		//__________________________________________________________________

		/**
		Establece el Roll de la entidad.

		@param roll Orientacion nueva para el roll.
		*/
		void setRoll(const Quaternion &roll);

		//__________________________________________________________________


		/**
		Metodo que permite elegir que eje rotar y cuantos grados hacerlo (en radianes).

		@param orientation Eje en el cual queremos rotar.
		@param rotation Grados en radianes que rotaremos.
		*/
		void rotate(int orientation, Ogre::Radian rotation);

		// =======================================================================
		//                         MÉTODOS DE CONSULTA
		// =======================================================================


		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad está activa.
		*/
		inline bool isActivated() { return _activated; }

		//__________________________________________________________________

		/**
		Método que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		diseño de hacerlo, pero esta es la más rápida: la entidad 
		con la descripción de la entidad tiene esta descripción que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() const { return _isPlayer; }


		// =======================================================================
		//                               GETTERS
		// =======================================================================


		/**
		Obtiene un puntero al componente que buscamos por id.

		@param id String que identifica al componente. Es el mismo nombre
		que el asignado en el blueprints.
		@return Puntero al componente que buscamos.
		*/
		template<typename T>
		T* getComponent(const std::string& id) {
			std::map<std::string, ComponentInfo>::iterator it = _components.find(id);
			return it != _components.end() ? static_cast<T*>(it->second.componentPtr) : NULL;
		}

		//__________________________________________________________________

		/**
		Devuelve el identificador único de la entidad.

		@return Identificador.
		*/
		TEntityID getEntityID() const { return _entityID; }

		//__________________________________________________________________

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap* getMap() { return _map; }

		//__________________________________________________________________

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap* getMap() const { return _map; }

		//__________________________________________________________________

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		//__________________________________________________________________

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }


		// =======================================================================
		//                               SETTERS
		// =======================================================================


		/**
		Setea el id de una entidad.
		*/
		void setEntityID(TEntityID id) { _entityID = id; }

		//__________________________________________________________________

		/**
		Setea a una entidad como jugador (o no).

		@param isPlayer true si queremos que la entidad sea un player.
		*/
		void setIsPlayer(bool isPlayer) { _isPlayer = isPlayer; }

		//__________________________________________________________________

		/**
		Setea un nombre de entidad.

		@param name Nombre que queremos asignar a la entidad.
		*/
		void setName(const std::string& name) { this->_name = name; }

		void wakeUp(IComponent* component);

	protected:


		// =======================================================================
		//                           CLASES AMIGAS
		// =======================================================================


		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factoría CEntityFactory). El constructor
		no hace más que poner el identificador único de la entidad
		, pues la inicialización efectiva se hace en el método spawn().
		
		@param entityID Identificador único de la entidad.
		*/
		CEntity(TEntityID entityID);

		//__________________________________________________________________

		/**
		Destructor de la clase. Es un método protegido pues para
		eliminar CEntity debe utilizarse la factoría
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, éste _ya ha sido desenganchado
		del mapa al que perteneció_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();


		// =======================================================================
		//                         MÉTODOS PROTEGIDOS
		// =======================================================================
		

		/**
		Método que destruye todos los componentes de una entidad (usando delete).
		*/
		void destroyAllComponents();

		//__________________________________________________________________

		/**
		Inicialización del objeto Logic, utilizando la información extraída de
		la entidad leída del mapa (Map::CEntity). Avisará a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrará la entidad.
		@param entity Información de construcción de la entidad leída del
		fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity);

		//__________________________________________________________________

		/**
		Llama al spawn de la entidad asignandole un nombre generado en tiempo
		de ejecución (nombre de entidad + id).

		@param map Mapa Logic en el que se registrará la entidad.
		@param entity Información de construcción de la entidad leída del
		fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool dynamicSpawn(CMap* map, Map::CEntity* entityInfo);
		

		// =======================================================================
		//                         MIEMBROS PROTEGIDOS
		// =======================================================================


		struct ComponentInfo {
			IComponent* componentPtr;
			std::list<IComponent*>::const_reverse_iterator tickIterator;
			std::list<IComponent*>::const_reverse_iterator fixedTickIterator;
		};

		std::list<IComponent*> _componentsWithTick;
		std::list<IComponent*> _componentsWithFixedTick;

		/** Identificador único de la entidad. */
		Logic::TEntityID _entityID;
		
		// typedef
		typedef std::map<std::string, ComponentInfo> TComponentMap;

		/**
		*/
		TComponentMap _components;


		/** Indica si la entidad está activada. */
		bool _activated;

		/** Tipo de la entidad declarado en el archivo blueprints. */
		std::string _type;

		/** Nombre de la entidad. */
		std::string _name;

		/** Mapa lógico donde está la entidad. */
		Logic::CMap *_map;

		/**
		Variables de posición y orientación.
		*/
		Vector3 _position;
		Ogre::Quaternion _orientation;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
