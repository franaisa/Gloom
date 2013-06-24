//---------------------------------------------------------------------------
// AnimatedEntity.h
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.h

Contiene la declaración de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author Rubén Mulero
@date June, 2013
*/

#ifndef __Graphics_AnimatedEntity_H
#define __Graphics_AnimatedEntity_H

#include "Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class AnimationState;
}

namespace Graphics 
{
	class CScene;
	class CEntity;
}
	
namespace Graphics 
{
	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en una entidad animada para enterarse de cuando
	terminan las animaciones de ésta.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CAnimatedEntityListener 
	{
	public:

		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		virtual void animationFinished(const std::string &animation) {}

	}; // CAnimatedEntityListener

	/**
	Clase que representa una entidad gráfica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. Añade la posibilidad de activar una animación 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animación activa.
	Podría ser más sofisticada permitiendo interpolación de animaciones o avisando
	mediante observers cuando una animación termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gráfico.
	
	@ingroup graphicsGroup

	@author Rubén Mulero
	@date June, 2013
	*/
	class CAnimatedEntity : public CEntity
	{

		enum AnimState{
			FADE_IN,
			FADE_OUT,
			RUNNING
		};

		struct Animation{
			AnimState state;
			Ogre::AnimationState* animation;
			float fadeTime;
		};

	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CAnimatedEntity(const std::string &name, const std::string &mesh):
					CEntity(name,mesh), _weapon(0), _graphicsWeapon(0) {}

		/**
		Destructor de la aplicación.
		*/
		virtual ~CAnimatedEntity() {}

		/**
		Activa una animación a partir de su nombre.

		@param anim Nombre de la animación a activar.
		@param loop true si la animación debe reproducirse cíclicamente.
		@param fadeTime El tiempo de fade-in fade-out de la animación

		@return true si la animación solicitada fue correctamente activada.
		*/
		bool setAnimation(const std::string &anim, bool loop, float fadeTime = 0.15f);

		/**
		Le pone un arma en la mano al monigote

		
		*/
		void attachWeapon(CEntity &arma, unsigned int id);
		
		/**
		Desactiva una animación a partir de su nombre.

		@param anim Nombre de la animación a activar.
		@return true si la animación solicitada fue correctamente desactivada.
		*/
		bool stopAnimation(const std::string &anim);
		
		/**
		Desactiva todas las animaciones de una entidad.
		*/
		void stopAllAnimations();

		/**
		Función que registra al oyente de la entidad gráfica. Por 
		simplicidad solo habrá un oyente por entidad.
		*/
		void addObserver(CAnimatedEntityListener *observer){
			_observers.push_back(observer);
		}

		/**
		Función que quita al oyente de la entidad gráfica. Por 
		simplicidad solo habrá un oyente por entidad.
		*/
		void removeObserver(CAnimatedEntityListener *observer){_observers.remove(observer);}

		void changeMaterialToWeapon(const std::string& materialName);

		std::string getWeaponMaterial();

		/**
		Orienta el bone en la orientación que se le pasa como parámetro

		@param bone El hueso que estamos modificando
		@param orientation la orientación que le estamos aplicando al hueso
		*/
		void moveBone(const std::string &bone, float pitch);

		/**
		Hace que la orientación del hueso no esté atachada a la orientación del padre

		@param bone El hueso que queremos liberar
		*/
		void freeBoneOrientation(const std::string &bone);

		/**
		Hace que la orientación del hueso esté atachada a la orientación del padre.

		@param bone El hueso que atachar.
		*/
		void lockBoneOrientation(const std::string &bone);

	protected:

		/**
		Objeto oyente que es informado de cambios en la entidad como 
		la terminación de las animaciones. Por simplicidad solo habrá
		un oyente por entidad.
		*/
		std::list<CAnimatedEntityListener*> _observers;

		// Cada entidad debe pertenecer a una escena. Solo permitimos
		// a la escena actualizar el estado.
		friend class CScene;
		
		/**
		Actualiza el estado de la entidad cada ciclo.
		
		@param secs Número de milisegundos transcurridos desde la última 
		llamada.
		*/
		virtual void tick(float secs);

		Ogre::Entity *_weapon;

		Graphics::CEntity *_graphicsWeapon;

		Ogre::SceneNode* _ObjectentityNode;

		/**
		Estructura de datos de las animaciones que hay ejecutandose
		*/
		std::map< std::string, Animation > _runningAnimations;

		typedef std::pair<std::string, Animation> TAnim;

		/**
		lista de animaciones que deben ser eliminadas de nuestra
		lista de animaciones ejecutandose
		*/
		std::list<std::string> _deletedAnims;
	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
