/**
@file LocalHound.h

Contiene la declaración del componente que controla los efectos de
cámara de la entidad archangel.

@see Logic::IComponent

@author Antonio Jesus Narvaez
@date Mayo, 2013
*/
#ifndef __Logic_LocalHound_H
#define __Logic_LocalHound_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/PlayerClass.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CScene;
}

//declaración de la clase
namespace Logic 
{
	//forward declarations 
	class CPhysicController;
	class CAvatarController;

	/**
	Componente que se encarga del manejo gráfico del shadow a nivel de jugador
	local, cambiando y creando los compositors adecuados para que la cámara
	muestre el feedback que representa a la clase Shadow

	@ingroup logicGroup

	@author Rubén Mulero Guerrero
	@date May, 2010

	*/

	class CLocalHound: public CPlayerClass
	{
		DEC_FACTORY(CLocalHound);

	public:
		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CLocalHound(): _scene(0),CPlayerClass("hound") {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CLocalHound(){}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deberá cargar e invoca al método virtual
		createGraphicsEntity() para generar la entidad gráfica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		*/
		virtual void onActivate();

		/**
		Método que desactiva el componente, de manera que no recibe mensajes
		ni hace tick
		*/
		//virtual void deactivate();


		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		// =======================================================================
		//                  METODOS HEREDADOS DE CPlayerClass
		// =======================================================================


		/** Beserker, el personaje dispara más rápido y hace más daño. */
		virtual void primarySkill();

		virtual void secondarySkill(){}
	protected:

		/**
		Invocado al despertar un componente.
		*/
		virtual void onWake();

		/**
		Método llamado en cada frame que actualiza el estado del componente de la particula.
				

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		virtual void onFixedTick(unsigned int msecs);

	private:

		Graphics::CScene* _scene;

		std::string _effect;
		std::string _strengthEffect;

		int _timestamp;
		float _offsetTimeSin;

		/** Variable boolena para comprobar si se esta haciendo la primary skill */
		bool _doingPrimarySkill;

		float _maxDefaultVelocity;

		float _bitetVelocity;

		int _biteTimer;

		float _biteDuration;

		float _biteMaxVelocity;

		bool charge;

		CPhysicController* _physicController;
		CAvatarController* _avatarController;
		
	};//class CLocalHound
	REG_FACTORY(CLocalHound);
}

#endif