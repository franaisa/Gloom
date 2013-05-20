/**
@file PhysicDynamicEntity.h


@see Logic::CPhysicDynamicEntity
@see Logic::IComponent

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Logic_PhysicDynamicEntity_H
#define __Logic_PhysicDynamicEntity_H

#include "Physics.h"
#include "Physics/DynamicEntity.h"

#include <vector>

// Predeclaración de tipos
namespace physx {
	class PxRigidActor;
	class PxControllerShapeHit;
};

namespace Physics {
	class CServer;
	class CGeometryFactory;
	class CMaterialManager;
};

// Los componentes pertenecen al namespace Logic
namespace Logic {	
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CPhysicDynamicEntity : public IPhysics {
		DEC_FACTORY(CPhysicDynamicEntity);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicDynamicEntity();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicDynamicEntity();
		
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

	    //void onShapeHit(const physx::PxControllerShapeHit &hit);

		virtual void onShapeHit(IPhysics *otherComponent, const Vector3& colisionPos, const Vector3& colisionNormal) { onContact(otherComponent, true); }

		//Metodo que devuelve el booleano _inTrigger que nos indica si hay alguien dentro del trigger
		bool getInTrigger(){ return _inTrigger; };

		//Metodo que devuelve el booleano _inContact que nos indica si hay alguien tocando la entidad
		bool getInContact(){ return _inContact; };

		bool getInControllerContact(){ return _inControllerContact; };

		void setPosition(const Vector3 &position, bool makeConversionToLogicWorld);

		void setTransform(const Matrix4 &transform, bool makeConversionToLogicWorld);

		void addForce(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		/**
		Limpia las fuerzas acumuladas que podria llevar la entidad.
		*/
		void clearForce(Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void addTorque(const Vector3& force, Physics::ForceMode mode = Physics::eFORCE, bool autowake = true);

		void deactivateSimulation();

		void activateSimulation();
		
		void move(const Vector3& disp);

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

	private:

		void readCollisionGroupInfo(const Map::CEntity *entityInfo, int& group, std::vector<int>& groupList);

		/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		void createPhysicEntity(const Map::CEntity *entityInfo);
				
		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de la información de mapa. 
		*/
		void createRigid(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);
		
		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		void createFromFile(const Map::CEntity *entityInfo, int group, const std::vector<int>& groupList);

		// Servidor de física
		Physics::CServer* _server;

		Physics::CGeometryFactory* _geometryFactory;

		Physics::CMaterialManager* _materialManager;

		// Actor que representa la entidad física en PhysX
		//physx::PxRigidActor *_actor;

		Physics::CDynamicEntity _physicEntity;

		// Vector de deplazamiento recibido en el último mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades físicas cinemáticas.
		Vector3 _movement;

		//Booleano que controla si hay alguien dentro del posible trigger que puede tener la entidad, por defecto desactivado
		bool _inTrigger;

		//Booleano que controla si hay alguien en contacto con la entidad, por defecto desactivado
		bool _inContact;

		bool _inControllerContact;

		bool _sleepUntil;

		bool _isTrigger;

		bool _noGravity;
	}; // class CPhysicDynamicEntity

	REG_FACTORY(CPhysicDynamicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
