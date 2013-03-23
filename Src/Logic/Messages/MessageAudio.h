#ifndef __Logic_MessageAudio_H
#define __Logic_MessageAudio_H

#include "Message.h"

namespace Logic {

	class CMessageAudio : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAudio);
	public:
		CMessageAudio();
		void setRuta(std::string ruta);
		std::string getRuta();
		void setId(std::string id);
		std::string getId();
		void setPosition(Vector3 position);
		Vector3 getPosition();
		~CMessageAudio(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		std::string _ruta;
		std::string _id;
		Vector3 _position;
	};
	REG_FACTORYMESSAGE(CMessageAudio);

};

#endif