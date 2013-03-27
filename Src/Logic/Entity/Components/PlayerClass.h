//---------------------------------------------------------------------------
// PlayerClass.h
//---------------------------------------------------------------------------

/**
@file PlayerClass.h

Contiene la declaración del componente que define
el comportamiento general de las habilidades de cada
clase.

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_PlayerClass_H
#define __Logic_PlayerClass_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"

#include <string>

namespace Logic {

	/**
	Componente encargado de la gestión general del uso
	de las habilidades de los personajes del juego.

	Cada clase tiene una serie de habilidades activas y
	pasivas. Las habilidades activas están compuestas 
	en realidad, por un ataque primario y otro secundario.

	Cada clase del juego estará representada por un componente.
	Dicho componente deberá heredar de esta clase e implementar
	como mínimo los métodos que se encargan de disparar
	las habilidades primaria y secundaria.

	En principio debería bastar con redefinir los métodos
	comentados anteriormente, ya que la clase base (ésta)
	se encarga de aceptar y procesar los mensajes de cambio
	de clase y de uso de las habilidades.

	IMPORTANTE: Este componente solo controla el uso de las
	habilidades activas. También es necesario controlar
	de alguna manera las habilidades pasivas al cambiar
	de clase. HAY QUE IMPLEMENTARLO.

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CPlayerClass : public IComponent {
		//DEC_FACTORY(CPlayerClass); -- Esta clase es abstracta y por lo tanto no instanciable
	public:

		// Tipo de la clase
		enum Type {
			eHOUND,
			eSCREAMER,
			eARCHANGEL,
			eSHADOW
		};

		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** 
		Constructor parametrizado. Todos los componentes necesitan un constructor
		por defecto para poder ser llamados desde la factoría, pero dado que este
		componente es abstracto, no tenemos ese problema.

		Pasamos el nombre de la clase hija como truco para leer los datos de cooldown.
		
		@param playerClassName Nombre de la clase de personaje. Las clases que hereden
		de ésta, deben rellenar este campo (Hound, Screamer, Shadow, Archangel).
		*/
		CPlayerClass(const std::string& playerClassName);

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CPlayerClass();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>primarySkillCooldown:</strong> Tiempo de recarga de la habilidad primaria. </li>
			<li><strong>secondarySkillCooldown:</strong> Tiempo de recarga de la habilidad secundaria. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>USE_PRIMARY_SKILL</li>
			<li>USE_SECONDARY_SKILL</li>
			<li>CHANGE_PLAYER_CLASS</li>
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(CMessage* message);

		//__________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage* message);

		//__________________________________________________________________

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Extiende el comportamiento de la clase padre ya que cuando se desactiva
		este componente puede ser porque el jugador haya cambiado de clase
		por lo que los timers de los cooldowns tienen que volver a 0.
		*/
		virtual void deactivate();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Método abstracto que debe implementar el disparo de la habilidad primaria
		del personaje.
		*/
		virtual void primarySkill() = 0;

		//__________________________________________________________________

		/**
		Método abstracto que debe implementar el disparo de la habilidad secundaria
		del personaje.
		*/
		virtual void secondarySkill() = 0;


	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		/**
		Dado un enumerado que representa el tipo de la clase al que queremos cambiar,
		activa dicho componente y desactiva el actual.
		*/
		void changePlayerClass(unsigned int classType);

		//__________________________________________________________________

		/** 
		Indica si la habilidad primaria puede usarse.

		@return True si ha finalizado el cooldown de la habilidad primaria.
		*/
		inline bool canUsePrimarySkill();

		//__________________________________________________________________

		/**
		Indica si la habilidad secundaria puede usarse.

		@return True si ha finalizado el cooldown de la habilidad secundaria.
		*/
		inline bool canUseSecondarySkill();


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		/** Nombre de la clase del personaje. */
		std::string _playerClassName;

		/** Tiempo de recarga de la habilidad primaria del personaje. */
		float _primarySkillCooldown;

		/** Tiempo de recarga de la habilidad secundaria del personaje. */
		float _secondarySkillCooldown;

		/** Cronometro para el cooldown de la primera skill. */
		float _primarySkillTimer;

		/** Cronometro para el cooldown de la segunda skill. */
		float _secondarySkillTimer;

	}; // class CPlayerClass

	//REG_FACTORY(CPlayerClass);

} // namespace Logic

#endif // __Logic_PlayerClass_H
