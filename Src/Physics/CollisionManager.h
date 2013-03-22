/**
@file CollisionManager.h

Contiene la declaración del gestor de colisiones.

@see Physics::CCollisionManager

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_CollisionManager_H_
#define __Physics_CollisionManager_H_

#include <PxSimulationEventCallback.h> 
#include <characterkinematic/PxController.h>

namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades físicas (incluidos controladores).
	Cuando se produce una colisión/contacto/solapamiento notifica a los componentes 
	lógicos correspondientes. Notar que dichos eventos se disparan en funcion del filtro
	que haya sido asignado a las shapes de cada actor (@see Physics::CServer).

	@ingroup physicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CCollisionManager : public physx::PxSimulationEventCallback, public physx::PxUserControllerHitReport {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================
		
		
		/** Constructor por defecto. */
		CCollisionManager();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CCollisionManager();


		// =======================================================================
		//             METODOS HEREDADOS DE PxSimulationEventCallback
		// =======================================================================


		/** 
		Método invocado cuando se rompe un constraint.
		Se dispara por ejemplo cuando se rompe una articulación debido a una fuerza
		superior al threshold fijado.

		IMPORTANTE: De momento no está implementado, ni siquiera está contemplado
		en el filter shader.

		@param constraints Información sobre los constraints que se han roto.
		@param count Número de constraints que se han roto.
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count);

		//________________________________________________________________________
 
		/** 
		Método invocado cuando se despiertan actores físicos. Notar que el borrado/creacion 
		de actores de la escena o la activacion/desactivacion de actores puede provocar
		que se dispare este método.

		IMPORTANTE: De momento no está implementado.
		
		@param actors Array de punteros a los actores que se acaban de despertar.
		@param count Número de actores que se acaban de despertar.
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);

		//________________________________________________________________________
  
		/** 
		Método invocado cuando se duermen actores físicos. Notar que el borrado/creacion 
		de actores de la escena o la activacion/desactivacion de actores puede provocar
		que se dispare este método.

		IMPORTANTE: De momento no está implementado.
		
		@param actors Array de punteros a los actores que se acaban de dormir.
		@param count Número de actores que se acaban de dormir.
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count);

		//________________________________________________________________________
   
		/**
		Método invocado cuando se produce un contacto entre dos shapes físicos. Se 
		dispara en función del filter shader. Notar que el borrado/creacion de actores 
		de la escena o la activacion/desactivacion de actores puede provocar que se 
		dispare este método.

		Comprueba que las shapes son válidas (que no hayan sido borradas) y avisa
		a los componentes lógicos asociados de dicho evento.

		@param pairHeader Informacion sobre los dos actores a los cuales pertenecen las shapes
		que han disparado el onContact.
		@param pairs Array de parejas de actores para los cuales se ha disparado el onContact.
		@param nbPairs Número de parejas sobre las cuales se ha disparado el onContact.
		*/
		void onContact(const physx::PxContactPairHeader &pairHeader, 
			           const physx::PxContactPair *pairs, physx::PxU32 nbPairs);

		//________________________________________________________________________
  
		/**
		Método invocado cuando una entidad física entra o sale de un trigger. Notar que 
		el borrado/creacion de actores de la escena o la activacion/desactivacion de 
		actores puede provocar que se dispare este método.

		Comprueba que las shapes son válidas (que no hayan sido borradas) y avisa
		a los componentes lógicos asociados de dicho evento.

		@param pairs Pareja de actores sobre la que se ha disparado el onTrigger.
		@param count Número de parejas sobre las que se ha disparado el onTrigger.
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);


		// =======================================================================
		//             METODOS HEREDADOS DE PxUserControllerHitReport
		// =======================================================================


		/**
		Método invocado cuando un character controller golpea una shape. Notifica
		a los componentes lógicos asociados a los actores del golpe.

		@param hit Informacion sobre el contacto con la shape.
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);

		//________________________________________________________________________
  
		/**
		Método invocado cuando un character controller golpea otro character controller.

		IMPORTANTE: De momento no está implementado.

		@param Información sobre el controller contra el que hemos golpeado.
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit);

		//________________________________________________________________________
   
		/**
		Método invocado cuando un character controller golpea un obstáculo definido por 
		el usuario. Es especialmente útil cuando queremos que cosas muy particulares
		choquen contra el player (por ejemplo paredes invisibles que delimitan el 
		escenario). Es mucho más eficiente que utilizar filter shaders.

		IMPORTANTE: De momento no está implementado.

		@param Información sobre el obstáculo contra el que hemos golpeado.
		*/ 
		void onObstacleHit(const physx::PxControllerObstacleHit &hit);
   
	}; // CCollisionManager

}; // namespace Physics

#endif // __Physics_CollisionManager_H_