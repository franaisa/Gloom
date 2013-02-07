#ifndef __Logic_SpawnPlayer_H
#define __Logic_SpawnPlayer_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la muerte del jugador, y devolverá a la vida en una posición random dada por un manager de respawn al jugador pasado cierto tiempo.
	<p>
	Poseera un booleano que controla si la entidad esta muerta o no, además del tiempo que le queda para spawnear otra vez.

*/
	class CSpawnPlayer : public IComponent
	{
		DEC_FACTORY(CSpawnPlayer);

	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/

		CSpawnPlayer() : IComponent(){}
		

	
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		*/
		virtual void activate();

		/**
		Este componente sólo acepta mensajes de tipo PLAYER_DEAD (por ahora).
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo PLAYER_DEAD desactivamos los componentes pertinentes y activamos el contador de tiempo para que
		en el tick si esta muerta y ha pasado cierto tiempo devolvamos a la vida al jugador.
		*/
		virtual void process(CMessage *message);

		/**
		Método llamado en cada frame que actualiza el estado del componente de la vida,
		<p>
		la cual bajará cada n milisegundos.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
	

	protected:

		/**
		Método que hace las acciones correspondientes a una muerte.
		*/
		void dead();


		/**
		Nos lleva el estado de la entidad, viva(false) o muerta(true).
		*/
		bool _isDead;

		/**
		Nos lleva el tiempo que permanecerá muerta.
		*/
		unsigned int _timeSpawn;

		/**
		Nos lleva el tiempo que lleva muerta.
		*/
		unsigned int _actualTimeSpawn;

		/**
		Nos sirve para no actualizar continuamente el hud con el tiempo de spawn.
		*/
		int _timeToSendHud;

	}; // class CShoot

	REG_FACTORY(CSpawnPlayer);

} // namespace Logic

#endif // __Logic_SpawnPlayer_H
