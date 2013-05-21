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

	void CMessageAudio::setNotIfPlay(bool notIfPlay){
		_notIfPlay=notIfPlay;
	}//
	//----------------------------------------------------------

	bool CMessageAudio::getNotIfPlay(){
		return _notIfPlay;
	}//
	//----------------------------------------------------------

	void CMessageAudio::setIsPlayer(bool isPlayer){
		_isPlayer=isPlayer;
	}//
	//----------------------------------------------------------

	bool CMessageAudio::getIsPlayer(){
		return _isPlayer;
	}//
	//----------------------------------------------------------


	Net::CBuffer CMessageAudio::serialize() {
		Net::CBuffer buffer((sizeof(int)*3) + (sizeof(float) * 3) + sizeof(bool));
		buffer.serialize(std::string("CMessageAudio"), true);
		buffer.serialize(_ruta, false);
		buffer.serialize(_id, false);
		buffer.serialize(_position);
		buffer.serialize(_notIfPlay);

		return buffer;
	}//
	//----------------------------------------------------------

	void CMessageAudio::deserialize(Net::CBuffer& buffer) {
		buffer.deserialize(_ruta);
		buffer.deserialize(_id);
		buffer.deserialize(_position);
		buffer.deserialize(_notIfPlay);
	}

};
