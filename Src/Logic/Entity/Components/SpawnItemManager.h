/**
@file SpawnItemManager.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_SpawnItemManager_H
#define __Logic_SpawnItemManager_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Febrero, 2013
*/
	class CSpawnItemManager : public IComponent
	{
		DEC_FACTORY(CSpawnItemManager);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSpawnItemManager() : IComponent(), _isRespawning(false), _timer(0) {}

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
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(CMessage *message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(CMessage *message);

		void itemGrabbed(CEntity* actor);

	protected:

		std::string _id;

		int _weaponType;

		int _reward;

		float _respawnTime;

		unsigned int _timer;

		bool _isRespawning;

	}; // class CSpawnItemManager

	REG_FACTORY(CSpawnItemManager);

} // namespace Logic

#endif // __Logic_Life_H
