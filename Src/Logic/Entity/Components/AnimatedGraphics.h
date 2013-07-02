/**
@file AnimatedGraphics.h

Contiene la declaración del componente que controla la representación
gráfica de una entidad estática.

@see Logic::CAnimatedGraphics
@see Logic::CGraphics

@author Rubén Mulero Guerrero
@author Antonio Jesús Narváez Corrales

@date March, 2013
*/
#ifndef __Logic_AnimatedGraphics_H
#define __Logic_AnimatedGraphics_H

#include "Graphics.h"
#include "Graphics/AnimatedEntity.h"
#include "Graphics/Bone.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CAnimatedEntity;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representación gráfica animada de una 
	entidad. Especializa a la clase CGraphics para que la entidad gráfica
	creada sea animada.
	<p>
	Además de los mensajes de cambio de posición y de cambio de orientación
	que acepta CGraphics, acepta mensajes para poner o para una animación
	(SET_ANIMATION y STOP_ANIMATION).
	
    @ingroup logicGroup

	@author Rubén Mulero Guerrero
	@author Antonio Jesús Narváez Corrales

	@date March, 2013
*/
	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:
		
		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(), _animatedGraphicsEntity(0), _currentWeapon(0), _currentMaterialWeapon("original"),
				_defaultAnimation(""), _weapons(NULL), _pi(Ogre::Radian(Math::PI),Vector3::UNIT_Y) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica y la animada.
		*/
		virtual ~CAnimatedGraphics();
		
		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_ANIMATION y STOP_ANIMATION.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual void onActivate();

		/**
		Método que desactiva el componente.

		@return true si todo ha ido correctamente.
		*/
		virtual void onDeactivate();
		
		// =======================================================================
		//             MÉTODOS HEREDADOS DE CANIMATEDENTITYLISTENER
		// =======================================================================
		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void animationFinished(const std::string &animation);

		// =======================================================================
		//                           MÉTODOS PROPIOS
		// =======================================================================

		void changeWeapon(int newWeapon);

		virtual void changeMaterial(const std::string& materialName);

		Graphics::CAnimatedEntity * getAnimatedEntity(){return _animatedGraphicsEntity;}

		Graphics::CBone getBone(const std::string& boneName) const;

	protected:

		/**
		Método virtual que construye la entidad gráfica animada de la entidad. 
		Sobreescribe el método de la clase CGraphics.
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);
		
		virtual void onTick(unsigned int msecs);

	private:

		/**
		estructura de la siguiente animacion a ejecutar
		*/
		struct Animation{
			std::string animation;
			bool loop;
			bool exclude;
		};

		/**
		Entidad gráfica animada.
		*/
		Graphics::CAnimatedEntity *_animatedGraphicsEntity;

		/**
		Entidades gráficas de las armas.
		*/
		Graphics::CEntity **_weapons;

		/**
		Animación por defecto de una entidad gráfica animada.
		*/
		std::string _defaultAnimation;

		std::string _originalMaterialWeapon;

		int _currentWeapon;

		std::string _currentMaterialWeapon;

		bool _insertAnimation;

		Animation nextAnim;

		Quaternion _pi;

	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
