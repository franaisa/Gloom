/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

namespace Logic 
{
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// TODO: 1. Leer atributo "life" del fichero de mapa y almacenar el valor
		// en el atributo privado _life


		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLife::accept(CMessage *message)
	{
		
		// TODO: 2. Aceptar mensajes del tipo DAMAGED
		return false;

	} // accept
	
	//---------------------------------------------------------

	void CLife::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			// TODO: 3. Procesar mensajes de tipo DAMAGED
			// Disminuir la vida y mostrar un mensaje de herido en la consola.
			// Si el jugador pierde toda la vida volver al estado de juego "menu".
		}

	} // process


} // namespace Logic

