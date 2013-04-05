//---------------------------------------------------------------------------
// MutableEntity.h
//---------------------------------------------------------------------------

/**
@file MutableEntity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CMutableEntity

@author Francisco Aisa García
@date Abril, 2013
*/

#ifndef __Graphics_MutableEntity_H
#define __Graphics_MutableEntity_H

#include "BaseSubsystems/Math.h"
#include "Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación


namespace Graphics {
	
	/**
	
	@ingroup graphicsGroup

	@author Francisco Aisa García
	@date Abril, 2013
	*/

	class CMutableEntity : public CEntity {
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CMutableEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CMutableEntity();

		void getMeshInformation(const Ogre::MeshPtr& mesh,size_t &vertex_count, Ogre::Vector3* &vertices,
								size_t &index_count, unsigned* &indices,
								const Ogre::Vector3 &position = Ogre::Vector3::ZERO,
								const Ogre::Quaternion &orient = Ogre::Quaternion::IDENTITY,
								const Ogre::Vector3 &scale = Ogre::Vector3::UNIT_SCALE);

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
