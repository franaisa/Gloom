//---------------------------------------------------------------------------
// CharacterSound.h
//---------------------------------------------------------------------------

/**
@file CharacterSound.h


@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_CharacterSound_H
#define __Logic_CharacterSound_H

#include "BaseSubsystems/Math.h"
#include "Logic/Entity/Component.h"
#include "AvatarController.h"

#include <string>

namespace Audio {
	class CServer;
}

namespace Logic {

	/**


	@ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
	*/

	class CCharacterSound : public IComponent, public CAvatarController::IObserver {
		DEC_FACTORY(CCharacterSound);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CCharacterSound();

		//__________________________________________________________________

		/** Destructor virtual. */
		virtual ~CCharacterSound();
		

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================
	

		/**
		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>primarySkillCooldown:</strong> Tiempo de recarga de la habilidad primaria. </li>
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


		virtual void onLand();
		virtual void onWalk();
		virtual void onJump();
		virtual void onDodge();
		virtual void onIdle();
		virtual void onAir();

		//virtual void downCollision(bool contacting) { std::cout << "downCollision = " << (contacting ? "true" : "false") << std::endl; }
		//virtual void topCollision(bool contacting) { std::cout << "topCollision = " << (contacting ? "true" : "false") << std::endl; }
		//virtual void sideCollision(bool contacting) { std::cout << "sideCollision = " << (contacting ? "true" : "false") << std::endl; }

	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================

		virtual void onStart();

		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================


		int _footstepTimer;

		unsigned int _footstepTimeStep;

		Audio::CServer* _audioServer;

		CAvatarController* _avatarController;

	}; // class CCharacterSound

	REG_FACTORY(CCharacterSound);

} // namespace Logic

#endif // __Logic_Footsteps_H
