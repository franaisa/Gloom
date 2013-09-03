/**
@file Ragdoll.h


@see Logic::CRagdoll
@see Logic::IComponent

@author Francisco Aisa García
@date Julio, 2013
*/

#ifndef __Logic_Ragdoll_H
#define __Logic_Ragdoll_H

#include "Physics.h"
#include "Physics/Aggregate.h"
#include "Graphics/Bone.h"

#include <vector>

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	// Predeclaración de clase
	class CAnimatedGraphics;

	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CRagdoll : public IPhysics {
		DEC_FACTORY(CRagdoll);
	public:
	
		/**
		Constructor por defecto.
		*/
		CRagdoll();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CRagdoll();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void process(const std::shared_ptr<CMessage>& message);


		virtual std::string getPhysicName();


		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact(IPhysics *otherComponent, const Physics::CContactPoint& contactPoint, bool enter);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal);

		void deactivateSimulation();

		void activateSimulation();
		
	protected:

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Si la entidad física es de tipo estático no hace nada. </li>
		<li> Si la entidad física es de tipo dinámico actualiza la posición y rotación de 
		     la entidad lógica usando la información proporcionada por el motor de física. </li>
		<li> Si la entidad física es de tipo cinemático, además solicita al motor de física
		     que mueva la entidad de acuerdo al último mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onFixedTick(unsigned int msecs);

		virtual void onStart();

		virtual void onActivate();

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void loadRagdoll(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);


		bool _ragdollHasControl;

		std::vector< std::pair< Graphics::CBone, Physics::CDynamicEntity* > > _ragdollBonesBuffer;

		Physics::CAggregate _aggregate;

	}; // class CRagdoll

	REG_FACTORY(CRagdoll);

} // namespace Logic

#endif // __Logic_Ragdoll_H
