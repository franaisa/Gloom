/**
@file Life.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	TODO  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
		DEC_FACTORY(CLife);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLife() : IComponent(), _life(100), _playerDead(false) {}

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Metodo que se llama al activar el componente.
		*/
		virtual void activate();

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		/**
		Al recibir daño quitamos vida
		*/
		void damaged(int damage);
		
		/**
		Sumar vida 
		*/
		void addLife(int life);
		
		/**
		Sumar escudo
		*/
		void addShield(int shield);

		void sendMessagePlayerDead();

	protected:

		/**
		Vida de la entidad(clase).
		*/
		int _classLife;

		/**
		Vida actual de la entidad
		*/
		int _life;
		/**
		Máxima vida de la entidad
		*/
		int _maxLife;
		/**
		MáximO escudo de la entidad
		*/
		int _maxShield;
		/**
		Cantidad de vida restada automáticamente a la entidad al pasar el tiempo en la entidad
		*/
		int _lifeDamage;
		/**
		Cada cuanto tiempo baja la vida en la entidad, usado en el tick
		*/
		unsigned int _lifeTimeDamage;
		/**
		Variable acumulativa usada en el tick en milisegundos.
		*/
		unsigned int _varLifeCumulative;

		/**
		Escudo de la entidad
		*/
		int _shield;
			/**
		Porcentaje de daño que absorve el escudo a la entidad
		*/
		int _porcentShield;

			/**
		Indica si el jugador está muerto.
		*/
		bool _playerDead;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
