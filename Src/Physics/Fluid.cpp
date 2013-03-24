//---------------------------------------------------------------------------
// Fluid.cpp
//---------------------------------------------------------------------------

/**
@file Fluid.cpp

@see Graphics::CFluid

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Fluid.h"
//#include "Scene.h"

#include <assert.h>

#include <Physics/Server.h>
#include <PxPhysicsAPI.h> // Cambiarlo!! Solo incluir lo necesario

using namespace physx;

namespace Physics {

	CFluid::CFluid(unsigned int maxParticles, float restitution, float viscosity,
				   float stiffness, float dynamicFriction, float particleDistance) : _runOnGPU(false) {

		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = Physics::CServer::getSingletonPtr()->getActiveScene();
		assert(_scene && "No existe una escena fisica");
		
		// Creamos el fluido con los parametros pasados
		createFluid(maxParticles, restitution, viscosity, stiffness, dynamicFriction, particleDistance);

		// declare particle descriptor for creating new particles
		// based on numNewAppParticles count and newAppParticleIndices, newAppParticlePositions arrays.
		PxParticleCreationData particleCreationData;
		
		// Numero de particulas
		particleCreationData.numParticles = maxParticles;
		//
		//particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(newAppParticleIndices);
		//
		//particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(newAppParticlePositions);

		// create particles in *PxParticleSystem* ps
		bool success = _fluid->createParticles(particleCreationData);
	} // CFluid

	//________________________________________________________________________

	CFluid::~CFluid() {
		// Destruimos el actor de physx asociado al fluido y desligamos el 
		// actor de la escena

		// Fijamos los punteros a physx como nulos
		_physxSDK = NULL;
		_scene = NULL;
	} // ~CFluid

	//________________________________________________________________________
		
	void CFluid::tick(float msecs) {
	} // tick

	//________________________________________________________________________

	void CFluid::createFluid(unsigned int maxParticles, float restitution, float viscosity,
								         float stiffness, float dynamicFriction, float particleDistance) {	

		_fluid = _physxSDK->createParticleFluid(maxParticles, true);
		assert(_fluid && "PxPhysics::createParticleFluid returned NULL\n");
		_fluid->setGridSize(5.0f);
		_fluid->setMaxMotionDistance(0.3f);
		_fluid->setRestOffset(particleDistance*0.3f);
		_fluid->setContactOffset(particleDistance*0.3f*2);
		_fluid->setDamping(0.0f);
		_fluid->setRestitution(restitution);
		_fluid->setDynamicFriction(dynamicFriction);
		_fluid->setRestParticleDistance(particleDistance);
		_fluid->setViscosity(viscosity);
		_fluid->setStiffness(stiffness);
		_fluid->setParticleReadDataFlag(PxParticleReadDataFlag::eVELOCITY_BUFFER, true);
	
	#if PX_SUPPORT_GPU_PHYSX
		_fluid->setParticleBaseFlag(PxParticleBaseFlag::eGPU, _runOnGPU);
	#endif
		_scene->addActor(*_fluid);
		assert(_fluid->getScene() && "PxScene::addActor failed\n");

	#if PX_SUPPORT_GPU_PHYSX
		//check gpu flags after adding to scene, cpu fallback might have been used.
		_runOnGPU = _runOnGPU && (_fluid->getParticleBaseFlags() & PxParticleBaseFlag::eGPU);
	#endif
	}

} // namespace Physics

	/*
	=======================================================================================
	** Elementos que tienen que ser especificados en la creacion del sistema de particulas:
	=======================================================================================
	- maxParticles: Maximo numero de particulas que se pueden añadir al sistema. Cuanto menor
	menos memoria se consume
	- particleBaseFlags, PxParticleBaseFlag::ePER_PARTICLE_REST_OFFSET: Activa/desactiva per
	particle rest offset. Cuando esta desactivado se consume menos memoria.


	=============================================================================================
	** Elementos que no pueden variar una vez que el sistema de particulas se atacha a la escena:
	=============================================================================================
	- maxMotionDistance: Máxima distancia a la que una particula puede viajar durante un step
	de simulación. Valores altos pueden afectar al rendimiento, mientras que valores pequeños
	pueden hacer que las particulas apenas se muevan.
	- gridSize: Una pista para que la sdk de physx sepa como elegir el tipo de agrupacion
	para temas de paralelizacion y demás.
	- restOffset: Define la minima distancia entre particulas y actores rigidos.
	- contactOffset: Define la distancia a la que se crea contacto entre las particulas y los
	rigid actors. TIENE QUE SER MAYOR QUE RESTOFFSET.
	- particleReadDataFlags: Especifica un subconjunto de propiedades de simulacion que se
	retornan a la aplicacion tras la simulacion.
	- particleBaseFlags, PxParticleBaseFlag::eGPU: Activa/Desactiva la aceleracion por gpu.
	- particleBaseFlags, PxParticleBaseFlag::eCOLLISION_TWOWAY: Activa/desactiva la interacion
	mutua entre rigidbodies y particulas.


	==========================================================================================
	** Elementos que no pueden variar una vez que el sistema de FLUIDOS se atacha a la escena:
	==========================================================================================
	- restParticleDistance: Resolucion del sistema de fluidos.


	===========================================
	** Propiedades variables de PxParticleBase:
	===========================================
	- restitution: Restitucion del sistema de particulas en la colision.
	- dynamicFriction: Friccion dinamica.
	- staticFriction: Friccion estatica.
	- damping: Velocidad constante de damping, la cual se aplica a todas las particulas.
	- externalAcceleration: Aceleracion que se aplica a cada particula en cada step. La
	gravedad se puede desactivar usando PxActorFlag::eDISABLE_GRAVITY.
	- particleBaseFlags, PxParticleBaseFlag::eENABLED: Activa/Desactiva la simulacion de
	particulas.
	- particleBaseFlags, PxParticleBaseFlag::ePROJECT_TO_PLANE: Activa/desactiva el modo
	de proyeccion que mantiene a las particulas en un plano
	- projectionPlaneNormal, projectionPlaneDistance: Define el plano para el modelo de proyeccion.
	- particleMass: Mass utilizada para interaccion twoway.
	- simulationFilterData: filtro para filtrar colisiones entre particulas y rigid bodies.


	===========================================
	** Propiedades mutables de PxParticleFluid:
	===========================================
	- stiffness: Define la constante de gas. Valores bajos hacen que el fluido sea mas
	compresible, mientras que los valores altos lo hacen mas incompresible (?¿). Los valores
	altos hacen que el sistema sea inestable. Hay que mantener siempre valores entre 1 y 200.
	- viscosity: Controla la densidad del fluido. Cuanta mas viscosidad, mas pesado y lento, cuanta
	menos, mas se parece al agua. Valores razonables estan entre 5 y 300.
	*/
