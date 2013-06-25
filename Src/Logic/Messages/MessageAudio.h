#ifndef __Logic_MessageAudio_H
#define __Logic_MessageAudio_H

#include "Message.h"

namespace Logic {

	class CMessageAudio : public CMessage{
	DEC_FACTORYMESSAGE(CMessageAudio);
	public:
		CMessageAudio();
		virtual ~CMessageAudio(){};

		void setAudioName(const std::string& name);
		void is3dSound(bool play3d);
		void isLoopable(bool loop);
		void streamSound(bool stream);

		std::string getAudioName();
		bool is3dSound();
		bool isLoopable();
		bool streamSound();
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

	protected:
		std::string _name;
		bool _loop;
		bool _play3d;
		bool _stream;
	};
	REG_FACTORYMESSAGE(CMessageAudio);

};

#endif