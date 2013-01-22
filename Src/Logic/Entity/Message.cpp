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

	void CMessage::addSmartP(){
		_smartP+=1;
	}//
	//----------------------------------------------------------

	void CMessage::subSmartP(){
		_smartP-=1;
		if(_smartP==0){
			delete this;
		}
	}//
	//----------------------------------------------------------



	CMessageTransform::CMessageTransform(TMessageType type){
		_type=type;
		_smartP=0;
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
		_smartP=0;
	} //
	//----------------------------------------------------------

	void CMessageControl::setType(ControlType controlType){
		_controlType=controlType;
	}//
	//----------------------------------------------------------

	ControlType CMessageControl::getType(){
		return _controlType;
	}//
	//----------------------------------------------------------

	CMessageMouse::CMessageMouse(TMessageType t):CMessageControl(t){

	} //
	//----------------------------------------------------------

	void CMessageMouse::setMouse(float mouse[]){
		_mouse[0]=mouse[0];
		_mouse[1]=mouse[1];
	}//
	//----------------------------------------------------------

	float* CMessageMouse::getMouse(){
		return _mouse;
	}//
	//----------------------------------------------------------




	CMessageTouched::CMessageTouched(TMessageType type){
		_type=type;
		_smartP=0;
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
		_smartP=0;
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
		_smartP=0;
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
		_smartP=0;
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
		_smartP=0;
	}//
	//----------------------------------------------------------

	void CMessageSwitch::setChange(unsigned char change){
		_change=change;
	}//
	//----------------------------------------------------------

	unsigned char CMessageSwitch::getChange(){
		return _change;
	}//
	//----------------------------------------------------------
		



	CMessageDamaged::CMessageDamaged(TMessageType type){
		_type=type;
		_smartP=0;
	}//
	//----------------------------------------------------------

	void CMessageDamaged::setDamage(unsigned char damage){
		_damage = damage;
	}//
	//----------------------------------------------------------

	unsigned char CMessageDamaged::getDamage(){
		return _damage;
	}//
	//----------------------------------------------------------
		



	CMessageAvatarWalk::CMessageAvatarWalk(TMessageType type){
		_type=type;
		_smartP=0;
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
		_smartP=0;
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