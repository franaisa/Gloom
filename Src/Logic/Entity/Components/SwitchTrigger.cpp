/**
@file SwitchTrigger.cpp

Contiene la declaración del componente que envia un mensaje SWITCH a otra
entidad cuando recibe un mensaje TOUCHED / UNTOUCHED.
 
@see Logic::CSwitchTrigger
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "SwitchTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Messages/MessageSwitch.h"

namespace Logic 
{
	IMP_FACTORY(CSwitchTrigger);
	
	//---------------------------------------------------------

	bool CSwitchTrigger::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		// TODO: leer atributo del mapa "target"
		// Leer atributo del mapa "target" y almacenarlo en el atributo privado _targetName
		_targetName = entityInfo->getStringAttribute("target");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSwitchTrigger::activate()
	{
		// TODO: obtener un puntero a la entidad target
		// Ojo: necesitamos hacerlo en el activate, porque en el spawn de este componente
		// puede que aún no exista la entidad target
		// Pedir al mapa un puntero a la entidad


		return true;

	} // activate
	
	//---------------------------------------------------------

	void CSwitchTrigger::deactivate()
	{
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	bool CSwitchTrigger::accept(CMessage *message)
	{
		// TODO: recibir mensajes de tipo TOUCHED y UNTOUCHED
		return false;

	} // accept
	
	//---------------------------------------------------------

	void CSwitchTrigger::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
		case Message::TOUCHED:
		case Message::UNTOUCHED:
			if(_target)
			{
				// TODO: mandar un mensaje de tipo SWITCH a la entidad target
				// Mandamos un 1 si TOUCHED y un 0 si UNTOUCHED 
				CMessageSwitch *m=new CMessageSwitch();
				
			}
			break;
		}

	} // process


} // namespace Logic

