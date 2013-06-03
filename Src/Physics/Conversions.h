/**
@file Conversions.h

Funciones auxiliares para convertir entre los tipos de PhysX y los tipos de la lógica. 

@author Francisco Aisa García.
@date Marzo, 2013.
*/

#ifndef	__Physics_Conversions_H_
#define	__Physics_Conversions_H_

#include "BaseSubsystems/Math.h"

#include <foundation/PxVec3.h> 
#include <foundation/PxTransform.h>
#include <foundation/PxMat44.h>
#include <characterkinematic/PxExtended.h>

namespace Physics {

	/**
	Transforma un vector lógico al equivalente en PhysX.

	@param v Vector lógico a transformar.
	@return Vector de physX equivalente.
	*/
	inline physx::PxVec3 Vector3ToPxVec3(const Vector3 &v) {
		return physx::PxVec3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un vector de PhysX al equivalente lógico.

	@param v Vector de physX que queremos transformar.
	@return El vector lógico equivalente.
	*/
	inline Vector3 PxVec3ToVector3(const physx::PxVec3 &v) {
		return Vector3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un quaternion de Physx al equivalente lógico.

	@param q Quaternion de physx que queremos transformar.
	@return Quaternion lógico equivalente.
	*/
	inline Quaternion PxQuatToQuaternion(const physx::PxQuat &q) {
		return Quaternion(q.w, q.x, q.y, q.z);
	}

	//________________________________________________________________________

	/**
	Transforma un quaternion de la lógica al equivalente en Physx.

	@param q Quaternion de la logica que queremos transformar.
	@return Quaternion físico equivalente.
	*/
	inline physx::PxQuat QuaternionToPxQuat(const Quaternion &q) {
		return physx::PxQuat(q.x, q.y, q.z, q.w);
	}

	//________________________________________________________________________


	/**
	Transforma un vector lógico al equivalente vector extendido de PhysX.

	@param v Vector lógico que queremos transformar.
	@return Vector de physX equivalente.
	*/
	inline physx::PxExtendedVec3 Vector3ToPxExtendedVec3(const Vector3 &v) {
		return physx::PxExtendedVec3(v.x, v.y, v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un vector extendido de PshysX al equivalente lógico.

	@param v Vector de physX que queremos transformar.
	@return Vector lógico equivalente.
	*/
	inline Vector3 PxExtendedVec3ToVector3(const physx::PxExtendedVec3 &v) {
		return Vector3((float) v.x, (float) v.y, (float) v.z);
	}

	//________________________________________________________________________

	/**
	Transforma un Transform de PhysX a una matriz 4x4 lógica equivalente.

	@param t Transform de physX.
	@return Matriz de rotación equivalente a la de physX.
	*/
	inline Matrix4 PxTransformToMatrix4(const physx::PxTransform &t) {
		physx::PxMat44 m(t);

		return Matrix4(m(0,0), m(0,1), m(0,2), m(0,3), 
					   m(1,0), m(1,1), m(1,2), m(1,3),
					   m(2,0), m(2,1), m(2,2), m(2,3),
					   0, 0, 0, 1);
	}

	//________________________________________________________________________

	/**
	Transforma una matriz 4x4 lógica en un Transform de PhysX equivalente.

	@param m Matriz lógica que queremos transformar.
	@return Transform de physX equivalente a la matriz de rotación dada.
	*/
	inline physx::PxTransform Matrix4ToPxTransform(const Matrix4 &m) {
		physx::PxMat44 pm(physx::PxVec4(m[0][0], m[1][0], m[2][0], m[3][0]),
						  physx::PxVec4(m[0][1], m[1][1], m[2][1], m[3][1]),
						  physx::PxVec4(m[0][2], m[1][2], m[2][2], m[3][2]),
						  physx::PxVec4(m[0][3], m[1][3], m[2][3], m[3][3]));

		return physx::PxTransform(pm);
	}

}; // namespace Physics

#endif // __Physics_Conversions_H_