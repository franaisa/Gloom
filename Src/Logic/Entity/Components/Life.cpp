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

		if(entityInfo->hasAttribute("life"))
			_life = entityInfo->getFloatAttribute("life");


		return true;

	} // spawn
	
	//---------------------------------------------------------

	/*bool CLife::accept(CMessage *message)
	{
		return message->getMessageType() == Message::DAMAGED;

	}*/ // accept
	
	//---------------------------------------------------------

	/*void CLife::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::DAMAGED:
			{
				// Disminuir la vida de la entidad
				
				_life -= ((CMessageDamaged*)message)->getDamage();
				printf("\nAh!, ya solo me queda %i de vida", _life);

				// Si han matado al jugador salir de la partida
				
				//if ((_life <= 0) && (_entity->isPlayer())) {
				//	Application::CBaseApplication::getSingletonPtr()->setState("menu");
				//}
				
				// @todo Poner la animación de herido.
				// @todo Si la vida es menor que 0 poner animación de morir.

			}
			break;
		}

	}*/ // process


} // namespace Logic

