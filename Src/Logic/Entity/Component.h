/**
@file Component.h

Contiene la declaración de la clase base de los componentes.

@see Logic::IComponent
@see Logic::CCommunicationPort

@author Francisco Aisa García
@date Abril, 2013
*/

#ifndef __Logic_Component_H
#define __Logic_Component_H

#include "CommunicationPort.h"
#include "Entity.h"
#include "Logic/Maps/ComponentFactory.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map {
	class CEntity;
}

namespace Logic {
	class CMap;
}

// Declaración de la clase
namespace Logic {

	/**
	Clase base de los componentes que forman las entidades.
	<p>
	Un componente puede recibir mensajes y reaccionar ante ellos
	enviando otros mensajes al resto de componentes hermanos o
	realizando alguna acción sobre el entorno.
	<p>
	Dependiendo del estado en el que se encuentre el componente
	y de si ha redefinido o no los métodos de onTick y onFixedTick
	la entidad invoca al método que corresponda en cada frame.

	onTick y onFixedTick se llaman en cada frame con la diferencia
	de que onFixedTick siempre se llama con una cantidad de msecs
	constantes (tantas veces como haga falta para procesar los msecs
	que realmente han transcurrido en ese frame).

	El ciclo de vida de un componente típico viene determinado por
	el ciclo de vida de la entidad a la que pertenece:
	<ol>
		<li>
			Cuando se crea la entidad se crea el componente.
		</li>
		<li>
			Tras la llamada al constructor se llama al spawn,
			que es el método de donde podemos leer los parámetros
			del mapa/arquetipos.
		</li>
		<li>
			Al cargarse el mapa se activan todas las entidades y con
			ello todos los componentes de la entidad (onActivate se invoca).
		</li>
		<li>
			Antes de comenzar con los ticks se invoca al start de la entidad
			y de los componentes (onStart en este caso).
		</li>
		<li>
			Durante la partida se llama periodicamente al método onTick o
			onFixedTick de los componentes.
		</li>
		<li>
			Al salir de la partida o desactivar la entidad se desactivan
			todos sus componentes (onDeactivate se invoca).
		</li>
		<li>
			Al borrar el mapa se eliminan todas las entidades y se ejecuta
			el destructor de todos los componentes.
		</li>
	</ol>

	A groso modo el componente puede estar activado o desactivado.
	Si el componente está desactivado ni procesa mensajes ni tiene tick.
	Sin embargo, si el componente está activado puede encontrarse
	en uno de los siguientes estados:
	<ul>
		<li>
			<strong>AWAKE/AVAILABLE: </strong> El componente está 
			despierto/disponible. Recibe mensajes y tiene tick.
		</li>
		<li>
			<strong>SLEEPING: </strong> El componente está dormido.
			No tiene tick pero recibe mensajes. Al recibir mensajes
			vuelve a tener tick y pasa al estado AWAKE/AVAILABLE.
		</li>
		<li>
			<strong>BUSY: </strong> El componente está ocupado.
			Tiene tick pero no recibe mensajes.
		</li>
	</ul>
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó García
	@date Julio, 2010
	*/

	class IComponent : public CCommunicationPort {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		IComponent();

		//__________________________________________________________________

		/**
		Destructor (virtual); en la clase base no hace nada.
		*/
		virtual ~IComponent();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================
		

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). El método es extendido
		en las clases hijas, que hacen cosas más inteligentes, dependiendo
		del tipo de componente concreto, pero siempre deberán invocar a esta
		antes de hacer nada.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________
		
		/**
		Método llamado una única vez tras la activación del componente. Se
		encarga de invocar a onStart.
		*/
		inline void start() { onStart(); }

		//__________________________________________________________________

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		Los milisegundos dados son variables.

		Se encarga de llamar a onTick que es el método que realmente ejecuta 
		el comportamiento de los componentes derivados.

		@param msecs Milisegundos transcurridos desde el último tick (variable).
		*/
		inline void tick(unsigned int msecs) { onTick(msecs); }

		//__________________________________________________________________
		
		/**
		Método llamado en cada frame que actualiza el estado del componente.
		Se encarga llamar al método que realmente tienen que redefinir los 
		componentes (onFixedTick).

		IMPORTANTE: Se diferencia del tick, en que msecs siempre es constante.

		@param msecs Milisegundos transcurridos desde el último tick. Siempre
		son constantes.
		*/
		inline void fixedTick(unsigned int msecs) { onFixedTick(msecs); }
		
		//__________________________________________________________________

		/**
		Si el componente estaba desactivado lo activa. Si se lleva a cabo
		la activación invoca al método onActivate para que los componentes
		hijos hagan lo que necesiten en el evento de activación.
		*/
		void activate();

		//__________________________________________________________________

		/**
		Si el componente estaba activado lo desactiva. Si se lleva a cabo
		la desactivación invoca al método onDeactivate para que los componentes
		hijos hagan lo que necesiten en el evento de desactivación.
		*/
		void deactivate();

		//__________________________________________________________________

		/**
		Pone el componente a dormir (deja de tener tick). Si el componente 
		recibe un mensaje en este estado se despierta. También podemos 
		despertarlo explicitamente llamando a wakeUp.

		Invoca al método onSleep para que el componente haga lo que necesite
		al ponerse a dormir.
		
		@param deepSleep true si no queremos que el componente se despierte
		al recibir mensajes.
		*/
		void putToSleep(bool deepSleep = false);

		//__________________________________________________________________

		/**
		Despierta al componente e invoca al método onWake SOLO si el componente
		estaba dormido.
		*/
		void wakeUp();

		//__________________________________________________________________

		/**
		Pone el componente en estado ocupado (deja de recibir mensajes pero
		tiene tick). Invoca al método onBusy.

		La única forma de sacar al componente de este estado es llamando a
		stayAvailable.
		*/
		void stayBusy();

		//__________________________________________________________________

		/**
		Pone el componente en estado disponible (realmente lo que hace es
		despertarlo ya que disponible y despierto son el mismo estado) e
		invoca al método onAvailable SOLO si el componente estaba en el estado
		BUSY.
		*/
		void stayAvailable();

		//__________________________________________________________________

		/** 
		Devuelve true si el componente está activado. 

		@return true si el componente está activado.
		*/
		inline bool isActivated() const { return _isActivated; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente está ocupado. 
		
		@return true si el componente está ocupado.
		*/
		inline bool isBusy() const { return _state == ComponentState::eBUSY; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente está despierto/disponible. 
		
		@return true si el componente está despierto.
		*/
		inline bool isAwake() const { return _state == ComponentState::eAWAKE; }

		//__________________________________________________________________

		/** 
		Devuelve true si el componente está durmiendo. 

		@return true si el componente está durmiendo.
		*/
		inline bool isSleeping() const { return _state == ComponentState::eSLEEPING; }

		//__________________________________________________________________

		/**
		Método que devuelve la entidad a la que pertenece el componente.

		@return La entidad a la que pertenece el componente.
		*/
		inline CEntity* getEntity() const { return _entity; }

	
	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		/**
		Se ejecuta la primera vez que la entidad se activa. Garantiza que todas las 
		entidades (incluidos sus componentes) han ejecutado el spawn y están listas
		para hacer el tick.
		*/
		virtual void onStart();

		//__________________________________________________________________

		/**
		Llamado en cada frame por fixedTick. Los clientes que hereden de esta
		clase deben redefinir su comportamiento.

		@param msecs Milisegundos transcurridos desde el último tick. Siempre
		son constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deberán sobreescribir este método con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Invocado cuando se activa el componente. Los componentes derivados
		deberán redefinir este método para hacer sus labores de activación.
		*/
		virtual void onActivate();

		//__________________________________________________________________

		/**
		Invocado cuando se desactiva el componente. Los componentes derivados
		deberán redefinir este método para hacer sus labores de desactivación.
		*/
		virtual void onDeactivate();

		//__________________________________________________________________

		/**
		Invocado al poner a dormir un componente. 
		*/
		virtual void onSleep();

		//__________________________________________________________________

		/**
		Invocado al despertar un componente.
		*/
		virtual void onWake();

		//__________________________________________________________________

		/**
		Invocado al poner un componente como ocupado.
		*/
		virtual void onBusy();

		//__________________________________________________________________

		/**
		Invocado al poner un componente como disponible.
		*/
		virtual void onAvailable();

		//__________________________________________________________________

		/**
		Método que establece la entidad a la que pertenece el componente.

		@param entity Entidad a la que pertenece el componente.
		*/
		void setEntity(CEntity *entity) { _entity = entity; }


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		/**
		Entidad que contiene al componente. Para reenviar el mensaje a 
		los otros componentes
		*/
		CEntity *_entity;


	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** clase amiga que puede establecerse como poseedor del componente. */
		friend class CEntity;

		/** true si el componente esta activado. */
		bool _isActivated;

		/** 
		Enumerado que indica el estado en el que se encuentra el componente:
		<ul>
			<li>eAWAKE: El componete está despierto/disponible.</li>
			<li>eBUSY: El componente está ocupado.</li>
			<li>eSLEEPING: El componente está durmiendo.</li>
		</ul>
		*/
		ComponentState::Enum _state;

		/** 
		Enumerado que indica el tipo de tick que tiene el componente: 
		<ul>
			<li>eNONE: No tiene tick de ningún tipo.</li>
			<li>eTICK: Tiene tick.</li>
			<li>eFIXED_TICK: Tiene fixed tick.</li>
			<li>eBOTH: Tiene tick y fixed tick.</li>
		</ul>
		*/
		TickMode::Enum _tickMode;

		/** 
		true si el componente no se va a despertar al recibir 
		mensajes cuando este dormido 
		*/
		bool _deepSleep;
		
	}; // class IComponent


/////////////////////////////////////////////////////////////
// Macros para la adición de los componentes a la factoría // 
// de componentes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaración de los métodos necesarios para que 
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factoría.
*/
#define DEC_FACTORY(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static IComponent* create(); \
	/** \
	Registra el componente de la clase en la factoría. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementación de los métodos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factoría.
*/
#define IMP_FACTORY(Class) \
IComponent* Class::create() \
{ \
	IComponent* res = new Class(); \
	return res; \
} \
bool Class::regist() \
{ \
	if (!CComponentFactory::getSingletonPtr()->has(#Class)) \
	{ \
		CComponentFactory::getSingletonPtr()->add(Class::create, #Class); \
	} \
	return true; \
}

/** 
Macro que invoca al método que registra la clase en la factoría.
*/
#define REG_FACTORY(Class) \
static bool RegisteredFactory_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Component_H
