//---------------------------------------------------------------------------
// Cloth.cpp
//---------------------------------------------------------------------------

/**
@file Cloth.cpp

@see Physics::CCloth

@author Francisco Aisa García
@date Marzo, 2013
*/

#include "Cloth.h"
//#include "Scene.h"

#include <assert.h>
#include <stdlib.h>
#include <Physics/Server.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxExtensionsAPI.h>   

using namespace std;
using namespace physx;

namespace Physics {

	CCloth::CCloth() {

		// Obtenemos la sdk de physics y comprobamos que ha sido inicializada
		_physxSDK = Physics::CServer::getSingletonPtr()->getPhysxSDK();
		assert(_physxSDK && "No se ha inicializado physX");
		// Obtenemos la escena de physx y comprobamos que ha sido inicializada
		_scene = Physics::CServer::getSingletonPtr()->getActiveScene();
		assert(_scene && "No existe una escena fisica");

		_cooking = Physics::CServer::getSingletonPtr()->getCooking();
		assert(_cooking && "No se ha inicializado el sistema de cocinado");

		createCloth();
	
	} // CCloth

	//________________________________________________________________________

	CCloth::~CCloth() {
		

		// Destruimos el actor de physx asociado al fluido y desligamos el 
		// actor de la escena

		// Fijamos los punteros a physx como nulos
		_physxSDK = NULL;
		_scene = NULL;
	} // ~CCloth

	void CCloth::createCloth() {
		//Create cloth
		PxClothMeshDesc meshDesc;
		meshDesc.setToDefault();
 
		//Fill the geometry
		int w = 8, h=7;
		float hw = w / 2.0f;
		float hh = h / 2.0f;
		float d = 0.2f;
		int numX = (int)(w / d) + 1;    
		int numY = (int)(h / d) + 1;    
		meshDesc.points.count= (numX+1) * (numY+1);        
		meshDesc.triangles.count= numX*numY*2;    
		meshDesc.points.stride= sizeof(PxVec3);  
		meshDesc.triangles.stride= 3*sizeof(PxU32);  
		meshDesc.points.data= (PxVec3*)malloc(sizeof(PxVec3)*meshDesc.points.count);
		meshDesc.triangles.data= (PxU32*)malloc(sizeof(PxU32)*meshDesc.triangles.count*3);
		meshDesc.edgeFlags = 0;

		//Fill the geometry
		int i,j;    
		PxVec3 *p = (PxVec3*)meshDesc.points.data;   
 
		_pos.resize(meshDesc.points.count);
		_normal.resize(meshDesc.points.count);
		_indices.resize(meshDesc.triangles.count*3);

		for (i = 0; i <= numY; i++) {        
		   for (j = 0; j <= numX; j++) {            
		   p -> x = d*j-hw;
		   p -> y = float(h);
		   p -> z = d*i;             
		   p++;   
		   }    
		}   

		memcpy(&_pos[0].x, (meshDesc.points.data), sizeof(PxVec3)*meshDesc.points.count);

		//Fill the topology
		PxU32 *id = (PxU32*)meshDesc.triangles.data;  
		for (i = 0; i < numY; i++) {        
		   for (j = 0; j < numX; j++) {            
		   PxU32 i0 = i * (numX+1) + j;            
		   PxU32 i1 = i0 + 1;            
		   PxU32 i2 = i0 + (numX+1);            
		   PxU32 i3 = i2 + 1;            
		   if ((j+i)%2) {                
		  *id++ = i0; *id++ = i2; *id++ = i1;                
		  *id++ = i1; *id++ = i2; *id++ = i3;            
		   } else {                
		  *id++ = i0; *id++ = i2; *id++ = i3;                
		  *id++ = i0; *id++ = i3; *id++ = i1;            
		   }      
		   }    
		}
 
		memcpy(&_indices[0], meshDesc.triangles.data, sizeof(PxU32)*meshDesc.triangles.count*3);

		//Make sure everything is fine so far
		if(!(meshDesc.isValid()))
		   cerr << "Mesh invalid."<< endl;


		// COOK FABRIC
		PxDefaultMemoryOutputStream wb;
		bool status = _cooking->cookClothFabric(meshDesc, _scene->getGravity(), wb);
		if(!status) {
			cerr<<"Problem cooking mesh.\nExiting ..."<<endl;
			exit(1);
		}
	
		PxDefaultMemoryInputData rb(wb.getData(), wb.getSize());

		PxClothFabric* fabric= _physxSDK->createClothFabric(rb);

		// ASIGNAMOS PESOS Y POSICIONES AL CLOTH
		PxTransform tr;
		tr.p = PxVec3(0,10,0); tr.q = PxQuat::createIdentity();

		PxClothParticle* points=(PxClothParticle*)malloc(sizeof(PxClothParticle)*meshDesc.points.count);
		p = (PxVec3*)meshDesc.points.data;  
		for(size_t i=0;i < meshDesc.points.count;i++) {
			points[i].pos = *p;
		 //Fixing the top corner points
		 if(i==0 || i==numX) 
			 points[i].invWeight =0;
		else 
			 points[i].invWeight = 1.f;
			 p++;
		}

		PxClothCollisionData cd;
		cd.setToDefault();
 
		cd.numSpheres=1;
		cd.pairIndexBuffer=0;
 
		_box_collider.pos= PxVec3(0.0f,2.0f,0.0f);
		_box_collider.radius=1;
		cd.spheres=&_box_collider;

		_cloth = _physxSDK->createCloth(tr, *fabric, points, cd, PxClothFlag::eSWEPT_CONTACT);

		if(_cloth) { 
		PxClothPhaseSolverConfig bendCfg;  
		bendCfg.solverType= PxClothPhaseSolverConfig::eFAST;
		bendCfg.stiffness = 1;
		bendCfg.stretchStiffness = 0.5; 

		_cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eBENDING,  bendCfg) ; 
		_cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSTRETCHING, bendCfg) ; 
		_cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSHEARING, bendCfg) ; 
		_cloth->setPhaseSolverConfig(PxClothFabricPhaseType::eSTRETCHING_HORIZONTAL, bendCfg) ;
  
		_cloth->setDampingCoefficient(0.125f);    
		_scene->addActor(*_cloth); 
	}
	else
		cerr << "Cannot create cloth" << endl;
	}

} // namespace Physics
