/**
@file Audio.cpp

Contiene la implementación del componente que controla el sonido de una entidad.
 
@see Logic::CAudio
@see Logic::IComponent

@author Jose Antonio García Yáñez
@date Marzo, 2013
*/

#include "Audio.h"

#include "../../../Audio/Server.h"

#include "../../Messages/MessageAudio.h"

namespace Logic 
{
	IMP_FACTORY(CAudio);
	
	//---------------------------------------------------------
	
	bool CAudio::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		return true;

	} // spawn
	//---------------------------------------------------------

	void CAudio::activate()
	{
		IComponent::activate();

	} // activate
	//---------------------------------------------------------

	bool CAudio::accept(CMessage *message)
	{
		return message->getMessageType() == Message::AUDIO;
	} // accept
	//---------------------------------------------------------

	void CAudio::process(CMessage *message)
	{
		std::string ruta,id;
		Vector3 position;


		switch(message->getMessageType())
		{
		case Message::AUDIO:
			{
				//Recogemos los datos
				ruta=((CMessageAudio*)message)->getRuta();
				id=((CMessageAudio*)message)->getId();
				position=((CMessageAudio*)message)->getPosition();
				//Le decimos al server de audio lo que queremos reproducir
				char *aux=new char[ruta.size()+1];
				aux[ruta.size()]=0;
				memcpy(aux,ruta.c_str(),ruta.size());
				Audio::CServer::getSingletonPtr()->playSound3D(aux,id,position);	
			}
			break;

		}

	} // process
	//----------------------------------------------------------

	void CAudio::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

	} // tick
	//----------------------------------------------------------

} // namespace Logic


