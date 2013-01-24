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

	CMessage::CMessage(TMessageType t) : _type(t), _smartP(0) {
		// Nada que hacer
	}//
	//----------------------------------------------------------
	
	// Inicializa los valores de sus campos a partir de una ristra de bytes
	// con datos serializados
	// dataSize TIENE QUE INDICAR EL TAMAÑO VERDADERO DEL BUFFER QUE SE PASA
	// por parametro:
	//     por ejemplo: si el buffer son 2 enteros, datasize debe ser sizeof(int) + sizeof(int)
	//                  o en su defecto buffer.getSize();
	// La idea es que en el constructor, los hijos inicializen solo la parte que les
	// corresponde haciendo uso del read.
	CMessage::CMessage(Net::byte* serializedData, size_t dataSize) : _tempBuffer(dataSize), _smartP(0) {
		// Volcamos la ristra de bytes al buffer para posteriormente deserializar
		_tempBuffer.write(serializedData, dataSize);
		// Reseteamos el puntero de lectura (ya que el mismo puntero sirve para lectura y escritura)
		_tempBuffer.reset();

		// Leemos el primer dato del buffer, que para todos los mensajes
		// siempre indicará su tipo
		_tempBuffer.read(&_type, sizeof(TMessageType));
	}//
	//----------------------------------------------------------

	Net::CBuffer CMessage::serialize() {
		// Si hemos utilizado el buffer en otra ocasion, recolocamos
		// el puntero de escritura al principio
		if(_tempBuffer.getSize() > 0)
			_tempBuffer.reset();
		
		// Escribimos el tipo del mensaje
		_tempBuffer.write(&_type, sizeof(_type));
		// Reseteamos el puntero de lectura
		_tempBuffer.reset();

		return _tempBuffer;
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



	CMessageTransform::CMessageTransform(TMessageType type) : CMessage(type) {
		// Nada que hacer
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



	CMessageControl::CMessageControl(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageTouched::CMessageTouched(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageUntouched::CMessageUntouched(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageSetAnimation::CMessageSetAnimation(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageStopAnimation::CMessageStopAnimation(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageSwitch::CMessageSwitch(TMessageType type) : CMessage(type) {
		// Nada que hacer
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
		



	CMessageDamaged::CMessageDamaged(TMessageType type) : CMessage(type) {
		// Nada que hacer
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
		



	CMessageAvatarWalk::CMessageAvatarWalk(TMessageType type) : CMessage(type) {
		// Nada que hacer
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




	CMessageKinematicMove::CMessageKinematicMove(TMessageType type) : CMessage(type) {
		// Nada que hacer
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