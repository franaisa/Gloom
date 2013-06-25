#include "MessageAudio.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAudio);

	CMessageAudio::CMessageAudio() : CMessage(Message::AUDIO),
									 _play3d(false),
									 _loop(false),
									 _stream(false),
									 _stop(false) {
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

	void CMessageAudio::stopSound(bool stop) {
		this->_stop = stop;
	}

	bool CMessageAudio::stopSound() {
		return _stop;
	}

	Net::CBuffer CMessageAudio::serialize() {
		Net::CBuffer buffer;
		buffer.serialize( std::string("CMessageAudio"), true );
		buffer.serialize(_name, false);

		// Comprimimos en un byte todos los booleanos
		unsigned char booleanMask = 0;

		booleanMask |= _loop	? (1 << 0) : 0;
		booleanMask |= _play3d	? (1 << 1) : 0;
		booleanMask |= _stream	? (1 << 2) : 0;
		booleanMask |= _stop	? (1 << 3) : 0;

		buffer.write( &booleanMask, sizeof(booleanMask) );

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAudio::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_name);

		unsigned char booleanMask;
		buffer.read( &booleanMask, sizeof(booleanMask) );

		_loop	= booleanMask & (1 << 0);
		_play3d	= booleanMask & (1 << 1);
		_stream	= booleanMask & (1 << 2);
		_stop	= booleanMask & (1 << 3);
	}

};
