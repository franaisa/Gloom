/**
@file AnimationManager.h

Contiene la declaración del componente que controla las animaciones
que posee esta entidad en cualquier ordenador

@see Logic::AnimationManager

@author Rubén Mulero
@date June, 2013
*/

#include "Logic/Entity/Component.h"

#ifndef __Logic_AnimationManager_H
#define __Logic_AnimationManager_H
//declaración de la clase
namespace Logic 
{
	//forward declarations
	class CAvatarController;
/**
	Componente que se encarga de las animaciones que tiene que poner la entidad
	en caso de que posea el componente adecuado para poner animaciones
	<p>
	Se encarga de, según el movimiento del personaje y sus acciones, enviar 
	mensajes de SET_ANIMATION y STOP_ANIMATION por la red para que los otros
	clientes que tengan esta entidad pongan las animaciones adecuadas
	
    @ingroup logicGroup
*/
	class CAnimationManager: public IComponent
	{
		DEC_FACTORY(CAnimationManager);
	public:

		struct Animation{
			std::string animation;
			int rewind;
		};



		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimationManager();

		/**
		destructor
		*/
		virtual ~CAnimationManager() {}

		// =======================================================================
		//                    METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		/**
		Inicialización del componente utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity).

		Inicialización del componente a partir de la información extraida de la entidad
		leida del mapa:
		<ul>
			<li><strong>defaultAnimation</strong> Animación por defecto que pone la entidad al spawnear en el mapa </li>
		</ul>

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		//________________________________________________________________________

		/**
		Metodo que se llama al activar el componente.
		Resetea los valores de inercia y desplazamiento.
		*/
		//virtual void onActivate();

		//________________________________________________________________________

		/** 
		Este componente acepta los siguientes mensajes:

		<ul>
			<li>PLAYER_DEAD</li
		</ul>
		
		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		//________________________________________________________________________

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	protected:

		// =======================================================================
		//                          MÉTODOS PROTEGIDOS
		// =======================================================================


		/**
		Setea el puntero al componente del controlador físico para evitar overhead
		de mensajes en el tick.

		@param unsigned int msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onStart();

		//________________________________________________________________________

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick. Son siempre
		constantes.
		*/
		virtual void onFixedTick(unsigned int msecs);

		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================

		void sendDeadMessage(TEntityID killer);

		std::string getMotionAnimation(const Vector3 &displacementDir);


		void initAnimationCommands();


	private:

		// =======================================================================
		//                           MIEMBROS DE CLASE
		// =======================================================================
		/**
		Tabla que representa las animaciones que el personaje debe poner
		cuando se está moviendo por el suelo
		*/

		Vector3 _lastDisplacementAnimation;

		CAvatarController* _avatarController;

		bool _flying;

		typedef std::pair<std::string, Animation> TAnimation;
		std::map<std::string, Animation> _animations;
	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimationManager);

} // namespace Logic
#endif