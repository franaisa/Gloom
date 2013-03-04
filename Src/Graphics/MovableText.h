//---------------------------------------------------------------------------
// MovableText.h
//---------------------------------------------------------------------------

/**
@file MovableText.h

Contiene la declaración de la clase que permite renderizar texto movible.

@see Graphics::CMovableText

@author Francisco Aisa García
@date Marzo, 2013
*/

#ifndef __Graphics_MovableText_H
#define __Graphics_MovableText_H

#include "BaseSubsystems/Math.h"
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreImage.h>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre {
	class Entity;
	class SceneNode;
	class Billboard;
	class BillboardSet;
	class Font;
	class Texture;
}

namespace Graphics {
	class CScene;
}

namespace Graphics {
	
	/**
	
	@ingroup graphicsGroup

	@author Francisco Aisa García
	@date Marzo, 2013
	*/

	class CMovableText {
	public:

		/**
		Constructor de la clase.

		@param type Tipo del billboard set.
		*/
		CMovableText(const std::string& bbSetName, const Ogre::Vector3& position);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CMovableText();

		/**
		Cambia la posición de la entidad.

		@param position Nueva posición para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Ogre::Vector3& position);


		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad está puesta invisible, no debería dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.
		 <p>
		 El método cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		 Escala la entidad.

		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Ogre::Vector3 &scale);

		/**
		 Escala la entidad.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/**
		 Devuelve el valor de la escala de la entidad.

		 @return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		// Añadir color y fuente
		/**
		@deprecated Los billboards no son buenos para poner texto en pantalla ya que a medida que la distancia
		aumenta se hacen realmente dificiles de leer. La mejor solucion es un overlay que se mueve con la entidad
		*/
		void setText(const std::string& text, const Ogre::Vector3& position, const Ogre::ColourValue& color, 
					 const std::string& fontName = "Verdana", float width = 0, float height = 0);

	protected:

		// CScene es la única que puede añadir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode* _billboardNode;

		Ogre::BillboardSet* _billboardSet;

		std::list<Ogre::Billboard*> _attachedBillboards;

		Ogre::Vector3 _position;

		/**
		Quizás es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene* _scene;

		/**
		Nombre del billboard set. Tiene que ser único.
		*/
		std::string _bbSetName;

		/**
		Indica si la entidad ha sido cargada en el motor gráfico.
		*/
		bool _loaded;

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
