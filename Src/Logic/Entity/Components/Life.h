/**
@file Life.h

Contiene la declaración del componente que 
controla la vida de un personaje.

@see Logic::CLife
@see Logic::IComponent

@author Francisco Aisa García
@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

namespace Logic {
	
	/**
	Componente que controla la vida y la armadura de una entidad.
	Procesa mensajes de daño y de "tweaking".
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@author Jose Antonio García Yáñez
	@date Marzo, 2013
	*/
	
	class CLife : public IComponent {
		DEC_FACTORY(CLife);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CLife();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CLife();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Método llamado en cada frame. En este componente se encarga
		de reducir la vida del individuo paulatinamente en base a los
		parámetros fijados desde fichero.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>defaultLife:</strong> Vida por defecto del personaje (depende de la clase de éste). </li>
			<li><strong>damageOverTime:</strong> Cantidad de vida que se pierde con el tiempo. </li>
			<li><strong>damageTimeStep:</strong> Cada cuanto se resta vida de forma automática. </li>
			<li><strong>shieldDamageAbsorption:</strong> Cantidad de daño que absorbe el escudo (de 0 a 100). </li>
			<li><strong>maxLife:</strong> Máximo de vida que puede tener el personaje. </li>
			<li><strong>maxShield:</strong> Máximo de armadura que puede tener el personaje. </li>
			<li><strong>audioPain:</strong> Nombre del fichero de audio que se dispara al herir al personaje. </li>
			<li><strong>audioDeath:</strong> Nombre del fichero de audio que se dispara al morir el personaje. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de vida y escudo a los fijados por defecto.
		*/
		virtual void activate();

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>DAMAGED</li>
			<li>ADD_LIFE</li>
			<li>ADD_SHIELD</li>
			<li>SET_REDUCED_DAMAGE</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Resta los daños indicados a la entidad (tanto de escudo
		como de vida), dispara los sonidos de daño correspondientes
		y envia el mensaje de muerte si es necesario.

		@param damage Daño que queremos restar al personaje.
		@param enemy Entidad que nos está haciendo daño.
		*/
		void damaged(int damage, CEntity* enemy);

		//__________________________________________________________________
		
		/**
		Suma la vida pasada por parámetro.

		@param life Cantidad de vida que queremos sumar.
		*/
		void addLife(int life);

		//__________________________________________________________________
		
		/**
		Suma los puntos de armadura pasados por parámetro.

		@param shield Puntos de armadura que queremos sumar.
		*/
		void addShield(int shield);

		//__________________________________________________________________

		/**
		Dado un porcentaje de reducción de daños (comprendido entre
		0 y 1) reduce todo el daño recibido en base a ese porcentaje.

		Por ejemplo, un porcentaje de 1 haría que el personaje siempre
		recibiera un daño de 0 puntos (o en otras palabras, que
		fuera inmune).

		@param percentage Porcentaje de reducción de daños. Tiene que 
		estar comprendido entre 0 y 1.
		*/
		inline void reducedDamageAbsorption(float percentage);

		//__________________________________________________________________

		/**
		El personaje muere al ejecutarse este método independientemente de
		la situación o los powerups que tenga.
		*/
		void suicide();

	private:


		// =======================================================================
		//                          MÉTODOS PRIVADOS
		// =======================================================================
		
		
		/**
		Resta los puntos de daño indicados.

		@param damage Daño que queremos restar a la entidad.
		@return true si el personaje ha muerto.
		*/
		bool updateLife(int damage);

		//__________________________________________________________________

		/**
		Setea la posición de la cámara y envía el mensaje de muerte.

		@param enemy Enemigo al que queremos que mire la cámara.
		*/
		void triggerDeathState(CEntity* enemy);

		//__________________________________________________________________

		/**	Dispara el sonido de muerte. */
		void triggerDeathSound();

		//__________________________________________________________________

		/** Dispara el sonido de herido. */
		void triggerHurtSound();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** 
		Porcentaje de daños recibidos. Su valor por defecto es 0 (desactivado), para hacer
		que el personaje sea inmune hay que fijarlo a 1. 
		*/
		float _reducedDamageAbsorption;

		/** Vida por defecto del personaje, depende de la clase. */
		int _defaultLife;

		/** Vida actual del personaje. */
		int _currentLife;
		
		/** Vida máxima del personaje. */
		int _maxLife;

		/** Puntos de armadura que el personaje tiene. */
		int _currentShield;
		
		/**	Puntos máximos de armadura que el personaje puede tener. */
		int _maxShield;
		
		/** Porcentaje de daño que absorbe la armadura. De 0 a 1. */
		float _shieldDamageAbsorption;

		/** Cantidad de vida que se le resta al personaje con el paso del tiempo. */
		int _damageOverTime;
		
		/** Cada cuanto tiempo baja la vida del personaje. */
		unsigned int _damageTimeStep;
		
		/** Timer que controla el tiempo que ha pasado desde el anterior step de daño. */
		unsigned int _damageTimer;

		/** Ruta del sonido de daño. */
		std::string _audioPain;

		/** Ruta del sonido de muerte. */
		std::string _audioDeath;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
