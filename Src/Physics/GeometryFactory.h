/**
@file GeometryFactory.h

Contiene la declaración de la factoría de geometrías
físicas.

@see Logic::CGeometryFactory

@author Francisco Aisa García.
@date Marzo, 2013.
*/

#ifndef __Physics_GeometryFactory_H
#define __Physics_GeometryFactory_H

#include "BaseSubsystems/Math.h"

// La factoria devuelve tipos estaticos (nada de punteros)
// por ello incluimos las cabeceras y no hacemos predeclaraciones
#include "geometry/PxSphereGeometry.h"
#include "geometry/PxBoxGeometry.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxCapsuleGeometry.h"

namespace Physics {

	// Alias para evitar acoplamiento en el proyecto de lógica
	typedef physx::PxGeometry Geometry;
	typedef physx::PxBoxGeometry BoxGeometry;
	typedef physx::PxSphereGeometry SphereGeometry;
	typedef physx::PxCapsuleGeometry CapsuleGeometry;
	typedef physx::PxPlane PlaneGeometry;

	/**
	Factoría de geometrías físicas. Los actores están formados de shapes, que
	a su vez están formados por geometrías. La factoría nos da la posibilidad
	de crear actores y hacer queries especificando la geometría que queremos.

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CGeometryFactory {
	public:


		// =======================================================================
		//                 METODOS DE INICIALIZACION Y LIBERACION
		// =======================================================================


		/**
		Inicializa la factoría de geometrías.

		@return false si no se ha podido inicializar.
		*/
		static bool Init();

		//________________________________________________________________________

		/**
		Libera los recursos reservados por la factoría.
		*/
		static void Release();


		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		/**
		Devuelve un puntero al único objeto de la clase.

		@return Factoría de entidades.
		*/
		static CGeometryFactory* getSingletonPtr() { return _instance; }

		//________________________________________________________________________

		/**
		Crea una esfera dado un radio.

		@param radius Radio de la esfera que queremos crear
		@return La geometría de una esfera.
		*/
		SphereGeometry createSphere(float radius);

		//________________________________________________________________________

		/**
		Crea una caja dadas unas dimensiones.

		@param dimensions Anchura, altura y profundidad de la caja.
		@return La geometría de una caja.
		*/
		BoxGeometry createBox(const Vector3& dimensions);

		//________________________________________________________________________

		/**
		Crea la geometría de un plano.

		@param point Punto por el que pasa el plano.
		@param normal Normal del plano.
		@return La geometría de un plano.
		*/
		PlaneGeometry createPlane(const Vector3 &point, const Vector3 &normal);

	private:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor de la clase, privado, pues es un singleton. */
		CGeometryFactory();

		//________________________________________________________________________

		/** Destructor privado, por ser singleton. */
		~CGeometryFactory();

		//________________________________________________________________________

		/** Única instancia de la clase. */
		static CGeometryFactory *_instance;

	}; // CGeometryFactory

} // namespace Physics

#endif // __Physics_GeometryFactory_H
