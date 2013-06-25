#include "MessageAudio.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAudio);

	CMessageAudio::CMessageAudio() : CMessage(Message::AUDIO),
									 _play3d(false),
									 _loop(false),
									 _stream(false) {
		// Nada que hacer
	} //
	//----------------------------------------------------------
	std::string CMessageAudio::getAudioName(){
		return _name;
	}//
	//----------------------------------------------------------

	void CMessageAudio::setAudioName(const std::string& name){
		this->_name = name;
	}//
	//----------------------------------------------------------

	void CMessageAudio::is3dSound(bool play3d) {
		this->_play3d = play3d;
	}
		
	void CMessageAudio::isLoopable(bool loop) {
		this->_loop = loop;
	}

	bool CMessageAudio::is3dSound() {
		return _play3d;
	}
		
	bool CMessageAudio::isLoopable() {
		return _loop;
	}

	void CMessageAudio::streamSound(bool stream) {
		this->_stream = stream;
	}
		
	bool CMessageAudio::streamSound() {
		return _stream;
	}

	Net::CBuffer CMessageAudio::serialize() {
		Net::CBuffer buffer;
		buffer.serialize( std::string("CMessageAudio"), true );
		buffer.serialize(_name, false);
		buffer.write( &_play3d, sizeof(_play3d) );
		buffer.write( &_loop, sizeof(_loop) );
		buffer.write( &_stream, sizeof(_stream) );

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAudio::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_name);
		buffer.read( &_play3d, sizeof(_play3d) );
		buffer.read( &_loop, sizeof(_loop) );
		buffer.read( &_stream, sizeof(_stream) );
	}

};
