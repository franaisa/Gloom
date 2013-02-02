//---------------------------------------------------------------------------
// buffer.h
//---------------------------------------------------------------------------

/**
 * @file buffer.h
 *
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 *
 *
 * @author Juan A. Recio-García
 * @author Francisco Aisa García
 * @date Enero, 2013
 */
#include "BaseSubsystems\Math.h"
#ifndef __BUFFER_H
#define __BUFFER_H
namespace Net {

typedef unsigned char byte;

/**
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 */
class CBuffer{
public:

	/**
	 * Crea un buffer dinámico.
	 * @param initsize Tamaño inicial del buffer
	 * @param delta Indica cuanto crecerá el buffer cada vez que se necesite ampliarlo
     */
	CBuffer(size_t initsize=500, size_t delta=100);

	/**
	 * Constructor de copias. Realiza una copia dura (deep copy).
	 */
	CBuffer(const CBuffer& buffer);

	/*
	 * Operador de igualdad. Realiza una copia dura (deep copy).
	 */
	CBuffer& operator=(const CBuffer& source);

	/**
	 * Destructora
	 */
	~CBuffer();

	/**
	 * Devuelve el buffer como una secuencia de bytes
	 */
	byte* getbuffer();

	/**
	 * Devuelve el tamaño del buffer. 
	 * Se utilizará normalmente para saber cuantos bytes devuelve getbuffer()
	 */
	size_t getSize();

	/**
	 * Reinicia el buffer.
	 * No modifica su tamaño actual
	 */
	void reset();

	/**
	 * Escribe datos en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void write(void* data, size_t datalength);

	/**
	 * Escribe un vector3 en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(const Vector3& data);

	/**
	 * Escribe un float en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(float data);

	/**
	 * Escribe un entero en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(int data);

	/**
	 * Escribe un string en el buffer, comprimiendolo a CRC32
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(const std::string& data);

	/**
	 * Escribe un unsigned char en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(unsigned char data);

	/**
	 * Escribe un boleano en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(bool data);

	/**
	 * Escribe un caracter en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(char data);

	/**
	 * Escribe un caracter en el buffer
	 * @param data son los datos a escribir
	 * @param datalenght es el tamaño de los datos a escribir (número de bytes)
	 */
	void serialize(const Matrix4& data);

	/**
	 * Lee datos del buffer.
	 * Al hacer esto el buffer se "vacia"
	 * \param data es un puntero indicando a donde se deben copiar los datos desde el buffer
	 * \param datalength es el número de datos (bytes) a leer
	 */
	void read (void* data, size_t datalength);


protected:
	void realloc();

	byte* _begin;
	byte* _current;
	size_t _maxsize;
	size_t _size;
	size_t _delta;

private:
	/**
	 * Realiza una "deep copy" de un buffer dado por parametro. Si el buffer tiene suficiente
	 * capacidad como para solo copiar datos, entonces se realiza una copia directa. En caso
	 * contrario se libera memoria y se reserva un tamaño suficiente para realizar la copia.
	 */
	void clone(const CBuffer& source);

};

} // namespace Net

#endif // _CBUFFER_H