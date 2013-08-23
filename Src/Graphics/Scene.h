//---------------------------------------------------------------------------
// Scene.h
//---------------------------------------------------------------------------

/**
@file Scene.h

Contiene la declaración de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Scene_H
#define __Graphics_Scene_H

#include <list>
#include "BaseSubsystems/Math.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"
#include <OgreTexture.h>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Root;
	class Viewport;
	class SceneManager;
	class StaticGeometry;
	class Light;
	class SceneNode;
	class ParticleSystem;
}
namespace Graphics 
{
	class CServer;
	class CCamera;
	class CEntity;
	class CStaticEntity;
	class CParticle;
	class CompositorManager;
	class CCompositorListener;
	class CPoolParticle;
	class CMotionBlur;
}

namespace Graphics 
{
	/**
	Clase que controla todos los elementos de una escena. Puede ser de 
	un estado como el menú o un nivel del juego. Su equivalente en la 
	lógica del juego en el caso de ser un nivel sería el mapa. la escena 
	es un contenedor de todas las entidades gráficas que pueden ser 
	reenderizadas por un viewport asociado a esa escena. Se asume que una
	escena solo va a ser reenderizada desde un punto de vista por lo que
	por defecto se crea una única cámara y cuando se activa la escena se
	crea un único viewport mostrado en la totalidad de la ventana de 
	reenderizado. En función del tipo de juego puede quererse hacer más de
	un reenderizado de la misma escena a la vez y mostrarlo en diferentes 
	viewports en la ventana. Por ejemplo en un juego de coches para mostrar
	la imagen de los retrovisores del coche.
	<p>
	Trás crear la escena se deben añadir las diferentes entidades y 
	entidades estáticas. Al añadir las entidades a la escena se fuerza su
	carga. Las entidades estáticas se almacenan en una estructura. Una vez 
	añadidas todas las entidades se debe crear la geometría estática ya 
	que hasta que no se invoque a buildStaticGeometry() las entidades no
	pasarán a formar parte de la geometría estática. Al activar la escena
	se fuerza la creación de la geometría estática si no se había creado.
	<p>
	@remarks Una vez activada la escena las modificaciones realizadas en
	los valores de las entidades estáticas no tendrán efecto.
	<p>
	@remarks Solo el servidor gráfico (Graphics::CServer) puede crear o 
	liberar escenas.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CScene
	{
	public:

		/**
		Devuelve la cámara de la escena.

		@return Puntero a la cámara de la escena.
		*/
		CCamera *getCamera() {return _camera;}

		/**
		Devuelve el nombre de la escena.

		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}

		/**
		Añade una entidad gráfica a la escena.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gráfica que se quiere añadir a la escena.
		@return Cierto si la entidad se añadió y cargó correctamente.
		*/
		bool addEntity(CEntity* entity);

		/**
		Añade una entidad gráfica estática a la escena. No sirve
		addEntity porque estas entidades deben ser almacenadas en otra
		lista para cuando se cree la geometría estática de la escena poder 
		recuperarlas y cargarlas.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gráfica que se quiere añadir a la escena.
		@return Cierto si la entidad se añadió y cargó correctamente.
		*/
		bool addStaticEntity(CStaticEntity* entity);

		/**
		Elimina una entidad gráfica de la escena. 
		<p>
		@remarks Este método NO destruye la entidad, ésta solo deja de
		ser parte de la escena.

		@param entity Entidad gráfica que se quiere eliminar de la escena.
		*/
		void removeEntity(CEntity* entity);

		/**
		Elimina una entidad gráfica estática de la escena. 
		<p>
		@remarks Este método NO destruye la entidad, ésta solo deja de
		ser parte de la escena.

		@param entity Entidad gráfica estática que se quiere eliminar de 
		la escena.
		*/
		void removeStaticEntity(CStaticEntity* entity);

		/**
		Devuelve el objeto de ogre Scene manager, a veces sera necesario tenerlo

		@return devuelve la scene manager
		*/
		Ogre::SceneManager *getSceneMgr() {
			return _sceneMgr;
		}

		/**
		Devuelve el objeto que representa la pantalla

		@return el viewport que se muestra con la escena
		*/
		Ogre::Viewport *getViewport() {
			return _viewport;
		}

		/**
		Crea un scene node, se usa para los overlays.
		*/
		void createSceneNode(const std::string &name);

		/**
		Método que crea un compositor a partir de un nombre
		NOTA IMPORTANTE: el nombre del compositor debe ser el nombre que tiene en su 
		definición como compositor, sino petará.
		Además, el compositor se guarda en una lista desde la que podremos acceder a
		sus variables de shader y setearle parametros.

		@param name El nombre del compositor que queremos crear.

		*/
		void createCompositor(const std::string &name);

		void setCompositorVisible(const std::string &name, bool visible);

		void destroyCompositor(const std::string &name);

		/**
		Método que cambia una variable del compositor, se utiliza para cambiar el
		material del compositor en tiempo de ejecución.

		@param name El nombre del compositor que queremos referenciar.
		@param variable La variable que queremos cambiar en el compositor.
		@param value el valor que tiene la variable que estamos cambiando.
		*/
		void updateCompositorVariable(const std::string &name, const std::string &variable, const float &value);


		bool getCompositorVisible(const std::string &name);

		/**
		Añade una entidad al scene node. Dicha entidad tendra que tener el material las entradas:
		<ul>
		<li></li>
		</ul>
		*/
		//bool addEntityToSceneNode(std::string &nameSceneNode, CEntity* entity);

		/**
		Elimina una entidad de la sceneNode
		*/
		//void removeEntityToSceneNode(std::string &nameSceneNode, CEntity* entity);

		/**
		Devuelve el scene node. Este metodo solo deberia de invocarse en Overlay.cpp
		*/
		Ogre::SceneNode* getSceneNode(const std::string &nameSceneNode);
		
		/**
		crea una particula, impor
		*/
		CParticle *createParticle(const std::string &particleName, const Vector3 &position);
		CParticle *createParticle(const std::string &particleName, const Vector3 &position, const Vector3 &directionWithForce);

		/**
		Pone un tipo de particula o de una particula concreta en visible o invisible

		@param nameParticle, nombre de la particula que se quiere cambiar su visibilidad.
		@param visible, indica si se pone visible o invisible las particulas
		*/
		void changeVisibilityParticle(const std::string nameParticle, bool visibility);

		void changeAmbientLight(Vector3 Light);

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
		// SubClase que hago para hacer ciertas pruebas con el Glow guay.

			class GlowMaterialListener : public Ogre::MaterialManager::Listener
			{
			protected:
				Ogre::MaterialPtr mBlackMat;
			public:
				GlowMaterialListener()
				{
					mBlackMat = Ogre::MaterialManager::getSingleton().create("mGlowBlack", "Internal");
					mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,0);
					mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0,0,0,0);
					mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0,0,0);
					mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,0);
				}
 
				Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, Ogre::Material*mat, unsigned short, const Ogre::Renderable*)
				{
					if (schemeName == "glow")
					{
						//LogManager::getSingleton().logMessage(">> adding glow to material: "+mat->getName());
						return mBlackMat->getTechnique(0);
					}

					return NULL;
				}

				~GlowMaterialListener(){
					Ogre::MaterialManager::getSingleton().destroyResourcePool("mGlowBlack");
				}
			};

		/////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
	protected:


		/**
		Clase amiga. Solo el servidor gráfico puede crear o liberar escenas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**
		Constructor de la clase.
		*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CScene();

		/**
		Despierta la escena y crea un viewport que ocupa toda la
		pantalla.
		*/
		void activate();

		/**
		Duerme la escena y destruye su viewport para que no se siga 
		reenderizando.
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo. Llama a su vez a 
		todas las entidades.
		
		@param secs Número de segundos transcurridos desde la última 
		llamada.
		*/
		void tick(float secs);

		/**
		Añade las entidades estáticas a la geometría estática del nivel
		y la construye. Si la geometría estática ya ha sido construida
		no se vuelve a construir.
		<p>
		@remarks Una vez construida la geometría estática no se pueden 
		modificar los valores de las entidades estáticas.
		*/
		void buildStaticGeometry();

		/**
		Clase amiga. Solo las entidades y la cámara pueden acceder al 
		gestor de la escena de Ogre.
		*/
		friend class CEntity;
		friend class CCamera;

		/**
		Devuelve el gestor de la escena de Ogre

		@return Puntero al gestor de la escena de Ogre.
		*/

		//esto ha de ir descomentado
		//Ogre::SceneManager *getSceneMgr() {return _sceneMgr;}


		/**
		Clase amiga. Solo las entidades pueden acceder al gestor de la
		escena de Ogre.
		*/
		friend class CStaticEntity;

		/**
		Devuelve la geometría estática de la escena de Ogre

		@return Puntero a la geometría estática de la escena de Ogre.
		*/
		Ogre::StaticGeometry *getStaticGeometry() {return _staticGeometry;}
		
		/**
		Nombre de la escena.
		*/
		std::string _name;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Marco en la ventana de reenderizado donde se pinta lo captado por
		una cámara. Solo puede haber una cámara asociada a un viewport,
		sin embargo una ventana de reenderizado puede tener diferentes
		viewports al mismo tiempo.
		*/
		Ogre::Viewport *_viewport;
		
		/**
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		Ogre::SceneManager *_sceneMgr;
		
		/**
		Luz direccional que se crea por defecto en la escena. Gráficamente
		mejora la escena bastante respecto a tener solo luz ambiente ya que
		se ven mejor los volúmenes de las entidades.
		*/
		Ogre::Light *_directionalLight;

		/**
		Camara desde la que se verá la escena. Puede haber cámaras más
		sofisticadas y más tipos de cámaras desde el punto de vista lógico,
		ellas se encargarán de mover esta instancia.
		*/
		CCamera *_camera;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CStaticEntity*> TStaticEntityList;

		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CEntity*> TEntityList;

		/**
		Tipos para la lista de sceneNodes
		*/
		typedef std::map<std::string, Ogre::SceneNode *> TSceneNodeList;

		/**
		Tipo para la lista de compositor listeners
		*/
		typedef std::map<std::string, CCompositorListener *> TCompositorList;

		/**
		Lista de compositorListeners
		*/
		TCompositorList _compositorList;

		/**
		Lista de entidades estáticas.
		*/
		TStaticEntityList _staticEntities;

		/**
		Lista de entidades dinámicas.
		*/
		TEntityList _dynamicEntities;
		
		/**
		Lista de SceneNodes.
		*/
		TSceneNodeList _sceneNodes;

		/**
		Geometría estática de la escena.
		*/
		Ogre::StaticGeometry *_staticGeometry;

		/**
		Escena cuyo padre es la grande. Se usa principalmente para los overlays
		*/
		//Ogre::SceneNode *_sceneNode;
		/**
		Un material listener para añadir los scheme en los materiales. En este caso solo es de glow asi que lo llamo asi.
		Quizas en un futuro sea un material listener generico.
		*/
		GlowMaterialListener *_glowMaterialListener;

		/**
		Compositor manager para manejar eventos de compositor
		*/
		Ogre::CompositorManager* _compositorManager;

		/**
		Pool de particulas
		*/
		CPoolParticle *_poolParticle;

		std::map< std::string, std::vector<CParticle*> > _particlesMap;

		Ogre::TexturePtr _depthMapTexture;

		CMotionBlur* _motionBlur;

	}; // class CScene

} // namespace Graphics

#endif // __Graphics_Scene_H
