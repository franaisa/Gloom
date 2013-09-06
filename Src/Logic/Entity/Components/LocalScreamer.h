/**
@file LocalArchangel.h

Contiene la declaración del componente que controla los efectos de
cámara de la entidad screamer.

@see Logic::IComponent

@author Javier Fernández Villanueva
@date Agosto, 2013
*/
#ifndef __Logic_LocalScreamer_H
#define __Logic_LocalScreamer_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CScene;
}

//declaración de la clase
namespace Logic 
{
	/**
	Componente que se encarga del manejo gráfico del shadow a nivel de jugador
	local, cambiando y creando los compositors adecuados para que la cámara
	muestre el feedback que representa a la clase Shadow

	@ingroup logicGroup

	@author Rubén Mulero Guerrero
	@date May, 2010

	*/

	class CLocalScreamer: public IComponent
	{
		DEC_FACTORY(CLocalScreamer);

	public:
		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CLocalScreamer(): _scene(0) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CLocalScreamer(){}
		
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
	private:

		Graphics::CScene* _scene;

		std::string _effect;
		std::string _strengthEffect;

		int _timestamp;
		
	};//class CLocalScreamer
	REG_FACTORY(CLocalScreamer);
}

#endif