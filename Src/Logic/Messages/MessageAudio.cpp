#include "MessageAudio.h"

#include <string>

namespace Logic {

	IMP_FACTORYMESSAGE(CMessageAudio);

	CMessageAudio::CMessageAudio() : CMessage(Message::AUDIO) {
		// Nada que hacer
	} //
	//----------------------------------------------------------
	std::string CMessageAudio::getRuta(){
		return _ruta;
	}//
	//----------------------------------------------------------

	void CMessageAudio::setRuta(std::string ruta){
		_ruta=ruta;
	}//
	//----------------------------------------------------------

	void CMessageAudio::setId(std::string id){
		_id=id;
	}//
	//----------------------------------------------------------

	std::string CMessageAudio::getId(){
		return _id;
	}//
	//----------------------------------------------------------

	void CMessageAudio::setPosition(Vector3 position){
		_position=position;
	}//
	//----------------------------------------------------------

	Vector3 CMessageAudio::getPosition(){
		return _position;
	}//
	//----------------------------------------------------------

	Net::CBuffer* CMessageAudio::serialize() {
		assert(_tempBuffer == NULL);
		//
		_tempBuffer = new Net::CBuffer((sizeof(int)*3) + (sizeof(float) * 3));
		_tempBuffer->serialize(std::string("CMessageAudio"), true);
		_tempBuffer->serialize(_ruta, false);
		_tempBuffer->serialize(_id, false);
		_tempBuffer->serialize(_position);

		return _tempBuffer;
	}//
	//----------------------------------------------------------

	void CMessageAudio::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ruta);
		buffer.deserialize(_id);
		buffer.deserialize(_position);
	}

};
