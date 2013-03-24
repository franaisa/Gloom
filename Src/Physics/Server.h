/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"

#include "GeometryFactory.h"
#include "MaterialManager.h"

#include <PxFiltering.h>

// Predeclaración de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;
	class PxRigidBody;
	class PxControllerBehaviorCallback;

	namespace pxtask {
		class CudaContextManager;
	};

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};

namespace Physics {

	/**
	Esta función es la que realmente se encarga que ocurra la magia de las notificaciones.
	Solo seran notificados de colisiones y triggers aquellos actores cuyos grupos de colisión
	hayan sido incluidos en la lista de grupos de colisión del otro actor con el que colisionan.

	Las máscaras y grupos de colisión se fijan en la función setupFiltering.

	@param attributes0 Información general sobre el primer objeto.
	@param filterData0 Datos de filtro del primer objeto.
	@param attributes1 Información general sobre el segundo objeto.
	@param filterData1 Datos de filtro del segundo objeto.
	@param pairFlags Flags de la pareja.
	@param constantBlock Puntero al bloque de datos.
	@param constantBlockSize Tamaño del bloque de datos.
	@return Los flags que indican que tipo de notificación debe dispararse.
	*/
	physx::PxFilterFlags customFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
											physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
											physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	//________________________________________________________________________

	/**
	Clase principal del proyecto de físicas que se encarga de la inicialización,
	gestión y liberación del motor de físicas.
	
	El resto de clases del proyecto se apoyan en la comunicación con esta clase
	para interactuar con el motor.

	De cara a la lógica, el servidor abstrae la inicialización y gestión de PhysX, 
	sirviendo de puente para la comunicación entre la física y el resto de proyectos.

	Es importante notar que esta clase no se encarga del manejo de las entidades de PhysX,
	tan solo de su simulación; cada uno de los distintos tipos de entidades físicas que la 
	lógica puede utilizar se encuentran abstraidos en clases distintas que se comunican 
	con el servidor (por motivos de coherencia y modularidad).
	
	IMPORTANTE: Esta clase está implementada como un Singleton de inicialización explícita: es necesario 
	invocar al método Init() al principio de la aplicación y al método Release() al final.

	@ingroup physicGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CServer {
	public:


		// =======================================================================
		//           MÉTODOS DE INICIALIZACIÓN Y LIBERACIÓN DE RECURSOS
		// =======================================================================


		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor físico.
		*/
		static CServer* getSingletonPtr() { return _instance; }

		//________________________________________________________________________
		
		/**
		Inicializa el servidor físico. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera el servidor físico. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		//________________________________________________________________________

		/**
		Elimina una entidad física de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor físico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor* actor);


		// =======================================================================
		//								   TICK
		// =======================================================================


		/**
		Realiza la simulación física y actualiza la posición de todas las 
		entidades físicas.

		La simulación física se hacen en pequeños steps de tiempo para evitar
		que se produzcan situaciones inesperadas al haber ticks muy grandes.

		@param secs Millisegundos transcurridos desde la última actualización.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);


		// =======================================================================
		//                 MÉTODOS DE GESTIÓN DE LA ESCENA FÍSICA
		// =======================================================================


		/**
		Crea la escena física. De momento asumimos que solo vamos a tener una escena
		física.

		En caso contrario sería conveniente crear un gestor de escenas y abstraer
		la escena de PhysX en una nueva clase.
		*/
		void createScene();

		//________________________________________________________________________

		/**
		Destruye la escena física.
		*/
		void destroyScene();


		// =======================================================================
		//            MÉTODOS PARA LA GESTIÓN DE LOS GRUPOS DE COLISIÓN
		// =======================================================================


		/**
		Establece si debe haber colisiones entre dos grupos de colisión. Al crear los objetos
		físicos se establece el grupo de colisión al que pertenecen. Usando este método es
		posible activar / desactivar las colisiones entre grupos.

		IMPORTANTE: Activar los grupos de colisión solo funciona para entidades y controladores.
		Los triggers no están incluidos. Para desactivar triggers y notificaciones hay
		que hacer uso del filter shader.

		@param group1 Primer grupo de colisión.
		@param group2 Segundo grupo de colisión
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);

		//________________________________________________________________________
		
		/**
		Dado un actor y su grupo de colisión, establece con que grupos de colisión
		debe haber notificaciones. Es importante notar que para que dos actores sean
		notificados de una colisión o de la activación/desactivación de un trigger,
		los grupos de colisión de ambos deben estar incluidos en la lista de grupos
		con los que colisionan.

		Esta función hace uso de máscaras de bits. Dichas máscaras se asignan en función
		de los grupos de colisión asignados a las entidades. Para llevar a cabo
		esta tarea existe otra función (customFilterShader) que se pasa como callback a
		PhysX.

		@param actor Actor de PhysX al que queremos configurar sus filtros.
		@param group Grupo de colisión del actor.
		@param groupList Grupos de colisión con los que queremos que el actor sea notificado
		en caso de colisión.
		*/
		void setupFiltering(physx::PxRigidActor* actor, int group, const std::vector<int>& groupList);

		
		// =======================================================================
		//                     QUERIES DE OVERLAP Y RAYCAST
		// =======================================================================


		/**
		Lanza un rayo y devuelve la primera entidad lógica contra la que interseca. Si el rayo
		no choca contra ninguna entidad devuelve NULL.
		 
		@param ray Rayo lanzado.
		@param maxDist distancia máxima de la primera intersección.
		@return Primera entidad lógica alcanzada o NULL.
		*/
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist) const;

		//________________________________________________________________________

		/**
		Lanza un rayo y devuelve la primera entidad lógica contra la que interseca que pertenezca 
		al grupo de colisión indicado. Si el rayo no choca contra ninguna entidad de ese grupo
		devuelve NULL.
		 
		@param ray Rayo lanzado.
		@param maxDist distancia máxima de la primera intersección.
		@param group Grupo de colisión de la entidad buscada.
		@return Primera entidad lógica alcanzada de ese grupo o NULL.
		*/
		Logic::CEntity* raycastClosest (const Ray& ray, float maxDist, int group) const;

		//________________________________________________________________________

		/**
		Lanza un rayo y devuelve la primera entidad lógica contra la que interseca que NO es la indicada en el ID.
		Si el rayo no choca contra ninguna entidad devuelve NULL.
		 
		@param ray Rayo lanzado.
		@param maxDist distancia máxima de la primera intersección.
		@param id Id con la que no puede chocar.
		@return Primera entidad lógica alcanzada o NULL.
		*/
		Logic::CEntity* raycastClosestInverse (const Ray& ray, float maxDist, unsigned int id) const;

		//________________________________________________________________________

		/**
		Dada una geometría, realiza una query de overlap y devuelve un vector con los punteros
		a las entidades que colisionan con dicha geometría.

		IMPORTANTE: Por motivos de eficiencia esta función aplico un filtro a esta función para que
		solo tenga en cuenta a las entidades dinámicas.

		@param geometry Geometría para la query de overlap. Mirar la documentación para ver cuales
		están soportadas.
		@param position Posición en la que queremos situar el centro de la geometría.
		@param entitiesHit Vector de entidades que colisionan con la geometría dada. Este vector
		NO DEBE SER INICIALIZADO!! ya que la propia función se encarga de inicializarlo y rellenarlo.
		El cliente es el responsable de liberar la memoria reservada en el vector pasado. En caso
		de no haber colisiones, el número de colisiones detectadas devuelto es 0 y por lo tanto
		no se reserva memoria (no hay que liberar memoria - pete asegurado si se libera).
		@param nbHits Número de entidades que colisionan contra la geometría dada, 0 en caso de
		no haber contacto con ninguna entidad.
		*/
		void overlapMultiple(const physx::PxGeometry& geometry, const Vector3& position, Logic::CEntity** & entitiesHit, int& nbHits);

		//________________________________________________________________________

		/**
		Dada una geometría, realiza una query de overlap y devuelve true si la geometría dada
		colisiona contra algún actor (dinámico o estático).

		Es mucho más eficiente que overlapMultiple. Usar cuando no nos interese saber contra
		qué colisiona la geometría.

		@param geometry Geometría para la query de overlap. Mirar la documentación para ver que geometrías
		están soportadas.
		@param position Posición donde queremos colocar el centro de la geometría dada.
		@return True si existe colisión con algún actor, falso en caso contrario.
		*/
		bool overlapAny(const physx::PxGeometry& geometry, const Vector3& position);


		// =======================================================================
		//               MÉTODOS PARA LA OBTENCIÓN DE INFO DE PHYSX
		// =======================================================================


		/**
		Devuelve un puntero a la escena actual de PhysX.

		@return Puntero a la escena de PhysX.
		*/
		physx::PxScene* getActiveScene() const { return _scene; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al core de PhysX.

		@return Puntero al core de PhysX.
		*/
		physx::PxPhysics* getPhysxSDK() const { return _physics; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al gestor de controladores de PhysX.

		@return Puntero al gestor de controladores de PhysX.
		*/
		physx::PxControllerManager* getControllerManager() const { return _controllerManager; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al sistema de cocinado de PhysX.

		@return Puntero al sistema de cocinado de PhysX.
		*/
		physx::PxCooking* getCooking() const  { return _cooking; }

		//________________________________________________________________________

		/**
		Devuelve un puntero al gestor de colisiones.

		@return Puntero al gestor de colisiones.
		*/
		CCollisionManager* getCollisionManager() const { return _collisionManager; }

	private:	


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase. */
		CServer();

		//________________________________________________________________________

		/** Destructor de la clase. */
		virtual ~CServer();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================


		/** Instancia única de la clase (por ser singleton). */
		static CServer* _instance;

		/** Puntero al gestor de errores. */
		physx::PxErrorCallback* _errorManager;

		/** Puntero al gestor de memoria de PhysX. */
		physx::PxAllocatorCallback* _allocator;

		/** Foundation de PhysX. */
		physx::PxFoundation* _foundation;

		/** Profile zone manager. */
		physx::PxProfileZoneManager* _profileZoneManager;

		/** CPU dispatcher de PhysX. Puede ser sobreescrito. */
		physx::PxDefaultCpuDispatcher* _cpuDispatcher;
		
		/** Manejador del procesamiento de cálculos en GPU. */
		physx::pxtask::CudaContextManager* _cudaContextManager;
		
		/** Para la gestión del debugger. */
		physx::debugger::comm::PvdConnection* _pvdConnection;
		
		/** Puntero al sistema de cocinado de PhysX. */
		physx::PxCooking* _cooking;

		/** Puntero al core de PhysX. */
		physx::PxPhysics* _physics;

		/** Puntero a la escena activa de PhysX. */
		physx::PxScene* _scene;

		/** Puntero al gestor de controladores de PhysX. */
		physx::PxControllerManager* _controllerManager;

		/** Puntero al gestor de colisiones. */
		CCollisionManager* _collisionManager;

		/** Tiempo real acumulado. Sirve para comprobar si tenemos que realizar un o varios steps de simulación física. */
		unsigned int _acumTime;

		/** Tamaño del timestep que tomamos para realizar una simulación. */
		unsigned int _fixedTime;

	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
