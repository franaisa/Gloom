/**
@file Message.cpp

Contiene la implementación de la clase Message y sus clases hijas, que serán los mensajes. Es una colección de componentes.

@author Jose Antonio García Yáñez
@date Enero, 2013
*/

#include "Message.h"


namespace Logic 
{

	TMessageType CMessage::getMessageType(){
		return _type;
	}//
	//----------------------------------------------------------



	CMessageTransform::CMessageTransform(TMessageType type){
		_type=type;
	} //
	//----------------------------------------------------------

	Matrix4 CMessageTransform::getTransform(){
		return _transform;
	}//
	//----------------------------------------------------------

	void CMessageTransform::setTransform(Matrix4 transform){
		_transform=transform;
	}//
	//----------------------------------------------------------



	CMessageControl::CMessageControl(TMessageType type){
		_type=type;
	} //
	//----------------------------------------------------------

	void CMessageControl::setString(std::string string){
		_string=string;
	}//
	//----------------------------------------------------------

	std::string CMessageControl::getString(){
		return _string;
	}//
	//----------------------------------------------------------

	void CMessageControl::setMouse(float mouse[]){
		_mouse[0]=mouse[0];
		_mouse[1]=mouse[1];
	}//
	//----------------------------------------------------------

	float* CMessageControl::getMouse(){
		return _mouse;
	}//
	//----------------------------------------------------------


	CMessageTouched::CMessageTouched(TMessageType type){
		_type=type;
	} //
	//----------------------------------------------------------

	CEntity* CMessageTouched::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------
	
	void CMessageTouched::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------




	CMessageUntouched::CMessageUntouched(TMessageType type){
		_type=type;
	} //
	//----------------------------------------------------------

	CEntity* CMessageUntouched::getEntity(){
		return _entity;
	}//
	//----------------------------------------------------------

	void CMessageUntouched::setEntity(CEntity *c){
		 _entity=c;
	}//
	//----------------------------------------------------------




	CMessageSetAnimation::CMessageSetAnimation(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	std::string CMessageSetAnimation::getString(){
		return _string;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setString(std::string string){
		_string=string;
	}//
	//----------------------------------------------------------

	bool CMessageSetAnimation::getBool(){
		return _bool;
	}//
	//----------------------------------------------------------

	void CMessageSetAnimation::setBool(bool boolean){
		_bool=boolean;
	}//
	//----------------------------------------------------------




	CMessageStopAnimation::CMessageStopAnimation(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	std::string CMessageStopAnimation::getString(){
		return _string;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::setString(std::string string){
		_string=string;
	}//
	//----------------------------------------------------------

	bool CMessageStopAnimation::getBool(){
		return _bool;
	}//
	//----------------------------------------------------------

	void CMessageStopAnimation::setBool(bool boolean){
		_bool=boolean;
	}//
	//----------------------------------------------------------




	CMessageSwitch::CMessageSwitch(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	void CMessageSwitch::setChange(int change){
		_change=change;
	}//
	//----------------------------------------------------------

	int CMessageSwitch::getChange(){
		return _change;
	}//
	//----------------------------------------------------------
		



	CMessageDamaged::CMessageDamaged(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setDamage(float damage){
		_damage = damage;
	}//
	//----------------------------------------------------------

	float CMessageDamaged::getDamage(){
		return _damage;
	}//
	//----------------------------------------------------------
		

	CMessageAvatarWalk::CMessageAvatarWalk(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	void CMessageAvatarWalk::setDirection(Vector3 direction){
		_direction = direction;
	}//
	//----------------------------------------------------------

	Vector3 CMessageAvatarWalk::getDirection(){
		return _direction;
	}//
	//----------------------------------------------------------



	CMessageKinematicMove::CMessageKinematicMove(TMessageType type){
		_type=type;
	}//
	//----------------------------------------------------------

	void CMessageKinematicMove::setMovement(Vector3 movement){
		_movement = movement;
	}//
	//----------------------------------------------------------

	Vector3 CMessageKinematicMove::getMovement(){
		return _movement;
	}//
	//----------------------------------------------------------

}