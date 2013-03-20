/**
@file EntityFactory.h

Contiene la declaración de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García, Marco Antonio Gómez Martín
@date Agosto, 2010
*/

#ifndef __Physics_GeometryFactory_H
#define __Physics_GeometryFactory_H

#include "BaseSubsystems/Math.h"

#include "geometry/PxSphereGeometry.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxCapsuleGeometry.h"

// Predeclaración de clases para ahorrar tiempo de compilación

// Definición de la clase
namespace Physics {

	typedef physx::PxGeometry Geometry;
	typedef physx::PxBoxGeometry BoxGeometry;
	typedef physx::PxSphereGeometry SphereGeometry;
	typedef physx::PxCapsuleGeometry CapsuleGeometry;
	typedef physx::PxPlane PlaneGeometry;

	// Enumerado para indicar el tipo de geometria que queremos crear
	enum GeometryType { 
		eBOX,			// Caja
		eCAPSULE,		// Capsula
		eSPHERE,		// Esfera
		eCONVEX_MESH,	// Maya convexa
		eHEIGHT_FIELD,	// Terreno
		ePLANE,			// Plano
		eTRIANGLE_MESH	// Maya de triangulos
	};

	/**
	
	@author Francisco Aisa García
	@date Marzo, 2013
	*/
	class CGeometryFactory {
	public:

		/**
		Inicializa la base de datos de la factoría.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la base de datos. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Devuelve un puntero al único objeto de la clase.

		@return Factoría de entidades.
		*/
		static CGeometryFactory *getSingletonPtr() { return _instance; }

		// Crear una esfera
		physx::PxSphereGeometry createSphere(float radius);

		// Crear una caja
		physx::PxBoxGeometry createBox(const Vector3& dimensions);

		// Crear un plano
		physx::PxPlane createPlane(const Vector3 &point, const Vector3 &normal);

	protected:

		/**
		Única instancia de la clase.
		*/
		static CGeometryFactory *_instance;

		/** 
		Constructor de la clase, protegido, pues es un singleton.
		Registra al objeto como observer del cargador de mapas.
		*/
		CGeometryFactory();

		/**
		Destructor protegido, por ser singleton.
		*/
		~CGeometryFactory();

	}; // CGeometryFactory

} // namespace Physics

#endif // __Physics_GeometryFactory_H
