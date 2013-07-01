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
#include "Physics/DynamicEntity.h"

#include <vector>

// Predeclaración de tipos
namespace Physics {
	class CServer;
	class CGeometryFactory;
	class CMaterialManager;
};

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
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

		/**
		Este componente sólo acepta mensajes de tipo KINEMATIC_MOVE. Estos mensajes  
		sólo se utilizan para mover entidades de tipo cinemático.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el próximo tick sobre la entidad cinemática. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact (IPhysics *otherComponent, bool enter);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) { onContact(otherComponent, true); }

		void addForce(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void addTorque(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

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

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void loadRagdoll(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);

		CAnimatedGraphics* _animatedGraphicsComponent;

		// Servidor de física
		Physics::CServer* _server;

		Physics::CGeometryFactory* _geometryFactory;

		Physics::CMaterialManager* _materialManager;

		Physics::CDynamicEntity _physicEntity;

	}; // class CRagdoll

	REG_FACTORY(CRagdoll);

} // namespace Logic

#endif // __Logic_Ragdoll_H
