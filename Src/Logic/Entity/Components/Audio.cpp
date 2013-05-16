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

	bool CAudio::accept(const std::shared_ptr<CMessage>& message)
	{
		return message->getMessageType() == Message::AUDIO;
	} // accept
	//---------------------------------------------------------

	void CAudio::process(const std::shared_ptr<CMessage>& message)
	{
		std::string ruta,id;
		Vector3 position;
		bool notIfPlay;
		bool localPlayer;

		switch(message->getMessageType())
		{
		case Message::AUDIO:
			{
				//Recogemos los datos
				std::shared_ptr<CMessageAudio> audioMsg = std::static_pointer_cast<CMessageAudio>(message);

				ruta=audioMsg->getRuta();
				id=audioMsg->getId();
				position=audioMsg->getPosition();
				notIfPlay=audioMsg->getNotIfPlay();
				localPlayer=audioMsg->getIsPlayer();
				//Le decimos al server de audio lo que queremos reproducir
				char *aux=new char[ruta.size()+1];
				aux[ruta.size()]=0;
				memcpy(aux,ruta.c_str(),ruta.size());
				//Si es local el sonido será stereo
				if(localPlayer)
					Audio::CServer::getSingletonPtr()->playSound(aux,id,notIfPlay);
				//En otro caso se trata de un sonido con posición 3D
				else
					Audio::CServer::getSingletonPtr()->playSound3D(aux,id,position,notIfPlay);	
			}
			break;

		}

	} // process
	//----------------------------------------------------------

} // namespace Logic


