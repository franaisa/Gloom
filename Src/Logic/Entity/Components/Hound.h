//---------------------------------------------------------------------------
// Hound.h
//---------------------------------------------------------------------------

/**
@file Hound.h

Contiene la declaración de la clase que
implementa las habilidades del personaje
"Hound".

@author Jose Antonio García Yáñez.
@author Rubén Mulero Guerrero.
@date Marzo, 2013
*/

#ifndef __Logic_Hound_H
#define __Logic_Hound_H

#include "BaseSubsystems/Math.h"
#include "PlayerClass.h"

namespace Logic {
	//forward declarations 
	class CPhysicDynamicEntity;
	class CAvatarController;
	class CPhysicController;

	/**
	Clase que implementa las habilidades propias
	del personaje "Hound".

	@ingroup logicGroup

	@author Jose Antonio García Yáñez.
	@date Marzo, 2013
	*/

	class CHound : public CPlayerClass {
		DEC_FACTORY(CHound);
	public:
		

		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CHound();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CHound();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>houndBerserkerDuration:</strong> Tiempo que dura el efecto 
			beserker desde que se activa. </li>
			<li><strong>houndBerserkerDamagePercent:</strong> Porcentaje de aumento del
			daño. </li>
			<li><strong>houndBerserkerCooldownPercent:</strong> Porcentaje de reducción
			de cooldowns. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//__________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>TOUCHED</li>
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

		/**
		Metodo que se llama al activar el componente.
		Restea los campos de la clase a los valores por defecto.
		*/
		virtual void onActivate();

		
		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Beserker, el personaje dispara más rápido y hace más daño. */
		virtual void primarySkill();

		//__________________________________________________________________

		/** Habilidad por definir. */
		virtual void secondarySkill();

		/**
		Se dispara cuando se deja de pulsar la tecla que dispara la habilidad primaria.
		Notar que este método no se ha hecho virtual puro (abstracto) porque muchas
		de las habilidades no necesitarán tenerlo en cuenta. No obstante, dado que
		el mensaje que se recibe para informarnos de esta acción es un mensaje de
		control, he decidido tenerlo en cuenta en la clase padre.

		Para que el uso de este método tenga sentido lo normal es que el cooldown
		de la habilidad sea 0.
		*/
		virtual void stopPrimarySkill();

		/** Metodo a invocar cuando haya pasado el tiempo de la secondary skill*/
		virtual void stopSecondarySkill();

	protected:

		/**
		Método llamado en cada frame.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		//virtual void onFixedTick(unsigned int msecs);

		virtual void onTick(unsigned int msecs);

		virtual void onStart(unsigned int msecs);

	private:


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Tiempo que dura el berserker */
		float _berserkerDuration;

		/** Timer que controla la duración del berserker. */
		float _berserkerTimer;
		
		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingPrimarySkill;

		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingSecondarySkill;

		float _maxDefaultVelocity;

		float _bitetVelocity;

		int _biteTimer;

		float _biteDuration;

		float _biteDamage;

		float _biteMaxVelocity;

		bool charge;
		/**
		Nombre del material original de la entidad
		*/
		std::string _materialName;

		CEntity* _biteTrigger;

		CPhysicDynamicEntity* _trigger;
		CAvatarController* _avatarController;
		CPhysicController* _physicController;
	}; // class CHound

	REG_FACTORY(CHound);

} // namespace Logic

#endif // __Logic_Hound_H
