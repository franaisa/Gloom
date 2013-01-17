/**
@file DamageTrigger.cpp

Contiene la implementación del componente que envia un mensaje DAMAGED cuando su 
entidad es tocada. El mensaje se envía a la entidad que se ha tocado.
 
@see Logic::CDamageTrigger
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "DamageTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CDamageTrigger);
	
	//---------------------------------------------------------

	bool CDamageTrigger::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// TODO: 1. leer atributo del mapa "damage" y almacenarlo en el atributo
		// privado damage

		return true;

	} // spawn

	//---------------------------------------------------------

	bool CDamageTrigger::accept(CMessage *message)
	{
		// TODO: 2. Aceptar mensajes de tipo TOUCHED
		return false;

	} // accept
	
	//---------------------------------------------------------

	void CDamageTrigger::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			// TODO: 3. Procesar mensajes de tipo TOUCHED
			// Enviar un mensaje de tipo DAMAGED a la entidad tocada
		}

	} // process


} // namespace Logic

