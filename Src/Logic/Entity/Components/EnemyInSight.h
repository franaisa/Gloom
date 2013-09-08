/**
@file EnemyInSight.h

@see Logic::CEnemyInSight
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Logic_EnemyInSight_H
#define __Logic_EnemyInSight_H

#include "Logic/Entity/Component.h"

namespace Physics {
	class CServer;
}

namespace Graphics {
	class CMovable2dText;
}

namespace Logic {

	class CEntity;

	/**

    @ingroup logicGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/

	class CEnemyInSight : public IComponent {
		DEC_FACTORY(CEnemyInSight);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto.*/
		CEnemyInSight();

		//________________________________________________________________________

		/** Destructor. */
		virtual ~CEnemyInSight();


		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>lightType:</strong> Tipo de luz, focal, puntual o direccional. </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================



	protected:


		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================
		

		virtual void onTick(unsigned int msecs);


		// =======================================================================
		//                          MIEMBROS PROTEGIDOS
		// =======================================================================

		std::map<CEntity*, int> _namesBeingShown;
		int _raycastTimer;
		unsigned int _raycastTimeStep;
		unsigned int _visibilityTimeStep;
		Physics::CServer* _physicsServer;
		float _heightShoot;

	}; // class CEnemyInSight

	REG_FACTORY(CEnemyInSight);

} // namespace Logic

#endif // __Logic_CharacterName_H
