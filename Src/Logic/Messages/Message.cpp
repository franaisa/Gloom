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
	
}

	
