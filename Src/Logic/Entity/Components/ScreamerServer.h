//---------------------------------------------------------------------------
// ScreamerServer.h
//---------------------------------------------------------------------------

/**
@file ScreamerServer.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Screamer" (parte del servidor).

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_ScreamerServer_H
#define __Logic_ScreamerServer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaración de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer" (parte del servidor).

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CScreamerServer : public CPlayerClass {
		DEC_FACTORY(CScreamerServer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamerServer();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamerServer();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>screamerShieldThreshold:</strong> Daño máximo que aguanta el escudo del Screamer. </li>
			<li><strong>screamerShieldDamageTimeStep:</strong> Cada cuanto se baja el escudo cuando está activado. </li>
			<li><strong>screamerShieldDamageOverTime:</strong> Cuánto daño se le quita al escudo en cada iteración. </li>
			<li><strong>screamerShieldRecoveryTimeStep:</strong> Cada cuanto se recupera el escudo cuando está en recarga. </li>
			<li><strong>screamerShieldRecoveryOverTime:</strong> Cuánto se recupera el escudo en cada iteración. </li>
			<li><strong>screamerExplotionDamage:</strong> Daño de la explosión al explotar el Screamer. </li>
			<li><strong>screamerExplotionRadius:</strong> Radio de la explosión al explotar el Screamer. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		//__________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void activate();

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** 
		Absorbe todos los proyectiles que le llegan de frente. Esta habilidad tiene
		un límite, si se sobrepasa explotas.
		*/
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

		//__________________________________________________________________

		/**
		Se dispara cuando el jugador deja de pulsar la tecla de uso de la habilidad
		primaria.

		Provoca que deje de usarse la habilidad primaria y comience la recuperacion
		de las defensas del Screamer.
		*/
		virtual void stopPrimarySkill();

		//__________________________________________________________________

		/**
		Resta al escudo del screamer la cantidad de puntos dados. Si absorbe demasiados
		puntos de daño, el personaje explotará.

		@param damage Daño que absorbe el escudo del Screamer.
		*/
		void absorbDamage(float damage);


	private:


		// =======================================================================
		//                          MÉTODOS PRIVADOS
		// =======================================================================


		/** Activa el shader del escudo y a la entidad */
		void activateScreamerShield();

		//__________________________________________________________________

		/** Desactiva el shader del escudo y a la entidad. */
		void deactivateScreamerShield();

		//__________________________________________________________________

		/**
		Crea una explosion mediante overlap.
		*/
		void createExplotion();

		//__________________________________________________________________

		/** Recoloca el escudo del screamer en la posición del punto de mira. */
		void refreshShieldPosition();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** true si la habilidad primaria está siendo usada. */
		bool _primarySkillIsActive;

		/** Puntos de escudo actuales de la habilidad primaria. */
		float _currentScreamerShield;

		/** Limite de aguante del escudo generado por la habilidad primaria. */
		float _screamerShieldThreshold;

		/** Cada cuanto baja la resistencia del escudo de la habilidad primaria cuando está activa. */
		float _screamerShieldDamageTimeStep;

		/** Cantidad de daño que se va restando al escudo de la habilidad primaria con el tiempo. */
		float _screamerShieldDamageOverTime;

		/** Timer para controlar cuando hay que restar energia al escudo del screamer. */
		float _screamerShieldDamageTimer;

		/** Cada cuanto se recupera el escudo del screamer si no esta usando la habilidad primaria. */
		float _screamerShieldRecoveryTimeStep;

		/** Cuantos puntos de escudo recupera el screamer con el tiempo. */
		float _screamerShieldRecoveryOverTime;

		/** Cronometro para calcular cada cuanto hay que subirle los puntos de escudo al Screamer. */
		float _screamerShieldRecoveryTimer;

		/** Daño de la explosión que provoca el screamer al soportar más daños de la cuenta con el escudo. */
		float _screamerExplotionDamage;

		/** Radio de la explosión si el Screamer explota. */
		float _screamerExplotionRadius;

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

	}; // class CScreamerServer

	REG_FACTORY(CScreamerServer);

} // namespace Logic

#endif // __Logic_ScreamerServer_H
