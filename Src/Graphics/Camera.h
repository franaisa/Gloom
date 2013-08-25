//---------------------------------------------------------------------------
// Camera.h
//---------------------------------------------------------------------------

/**
@file Camera.h

Contiene la declaración de la clase que maneja la cámara.

@see Graphics::CCamera

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#ifndef __Graphics_Camera_H
#define __Graphics_Camera_H

#include "BaseSubsystems/Math.h"
#include <OgreCamera.h>
#include <list>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Camera;
	class SceneNode;
}
namespace Graphics 
{
	class CScene;
	class CServer;
	class CEntity;
}
namespace Logic
{
	class CCamera;
}

namespace Graphics 
{
	/**
	Clase de la cámara extendida basada en Ogre.
	La cámara está compuesta por un único nodo.
	
	@ingroup graphicsGroup

	@author Jose Antonio García Yáñez
	@date Mayo, 2013
	*/
	class CCamera
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la cámara.
		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CCamera(const std::string &name, CScene *scene);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CCamera();
		

		/**
		Devuelve la posición de la cámara.

		@return Referencia a la posición del nodo que contiene la cámara de Ogre.
		*/
		const Vector3 &getCameraPosition();
		

		/**
		Devuelve la orientación de la cámara.

		@return Referencia al quaternion del nodo que contiene la cámara de Ogre.
		*/
		const Quaternion &getCameraOrientation();


		/**
		Cambia la posición de la cámara.

		@param newPosition Nueva posición para el nodo que contiene la cámara 
		de Ogre.
		*/
		void setCameraPosition(const Vector3 &newPosition);


		/**
			get & set de CameraDirection
		*/
		void setCameraDirection(const Vector3& direction);
		const Vector3 &getCameraDirection();


		/**
		Devuelve el quaternion en el mundo real de la cámara.
		*/
		const Quaternion& getRealOrientation ();


		/**
		Roll de camera. Gira la camara en el sentido contrario
		a las agujas del reloj.

		El giro comienza a partir de la z local (o el norte):
		- Un giro de 2pi deja a la camara sin rotar, un giro
		de pi dejaria la camara boca abajo.

		@param Radianes de giro de la camara.
		*/
		void rollCamera(float fRadian);


		/**
		Devuelve un puntero a la camara creada por Ogre.

		@return Puntero a la camara creada por Ogre.
		*/
		Ogre::Camera* getOgreCamera() { return _camera; }	


		/**
		Metodo que crea una entidad grafica asociada a la camara, de tal modo que la posicion sera relativa a la camara.
		¡IMPORTANTE! esto crea una entidad, el destructor de la camara no elimina dicha entidad, se debera de eliminar ella por separado

		@param nameEntity, nombre de la entidad que se creara
		@param nameMesh, nombre de la maya que se creara.
		@param position, Vector3 con la posicion 
		@return CEntity* devuelve un puntero a una entidad grafica. 
		*/
		Graphics::CEntity* CCamera::addEntityChild(const std::string &nameEntity, const std::string &nameMesh, Vector3 position = Vector3::ZERO);


		/**
		Establece la orientacion de la camara utilizando el quaternion pasado como parámetro.

		@param orientation Orientacion puesta en quaternion para setear.
		*/

		void setOrientation(const Quaternion& orientation);


		/**
		Método que cambia el FOV de la cámara por uno especificado por parametro
		*/
		void setFov(float radianAngle);

		/**
		Establece la orientacion para mirar al punto pasado por parametro.

		@param position Posicion hacia donde mirará la camara.
		*/
		void lookAt(const Vector3& position);

		/**
		Método que resetea el fov y lo pone por defecto
		*/
		void resetFov();

		Ogre::SceneNode* getSceneNode() { return _cameraNode; }

	protected:
		
		/**
		Clase amiga. Solo la escena tiene acceso a la cámara de Ogre para
		poder crear el viewport.
		*/
		friend class CScene;

		/**
		Devuelve la cámara de Ogre.

		@return puntero a la cámara de Ogre.
		*/
		Ogre::Camera *getCamera() {return _camera;}

		/**
		Nodos que contienen la cámara.
		*/
		Ogre::SceneNode *_cameraNode;
		
		/**
		La cámara de Ogre.
		*/
		Ogre::Camera *_camera;

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la cámara.
		*/
		std::string _name;

		float _defaultFov;

	}; // class CCamera

} // namespace Graphics

#endif // __Graphics_Camera_H
