//---------------------------------------------------------------------------
// Screamer.h
//---------------------------------------------------------------------------

/**
@file Screamer.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Screamer".

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_Screamer_H
#define __Logic_Screamer_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {

	// Predeclaración de clases.
	class CEntity;

	/**
	Clase que implementa las habilidades propias
	del personaje "Screamer".

	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CScreamer : public CPlayerClass {
		DEC_FACTORY(CScreamer);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CScreamer();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CScreamer();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		De momento no lee nada nuevo, tan solo llama a la clase padre.

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

		/** Recoloca el escudo del screamer en la posición del punto de mira. */
		void refreshShieldPosition();

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

		/** Puntero a la entidad que representa al escudo generado por la habilidad primaria. */
		Logic::CEntity* _screamerShield;

	}; // class CScreamer

	REG_FACTORY(CScreamer);

} // namespace Logic

#endif // __Logic_Screamer_H
