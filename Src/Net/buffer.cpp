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
 * @date Octubre, 2006
 */

#include "buffer.h"
#include "memory.h"

#define DEBUG_BUFFER 1

namespace Net {

	CBuffer::CBuffer(size_t initsize, size_t delta)
	{
		_current = _begin = new byte[initsize];
		_maxsize = initsize;
		_size = 0;
		_delta = delta;
	}

	void CBuffer::clone(const CBuffer& source) {
		// Si ya habia datos reservados, comprobamos
		// si pueden alojar la memoria que se pide
		if(_begin != 0 || _maxsize < source._maxsize) {
			// Eliminamos la memoria reservada
			delete _begin;
				
			// Reservamos memoria para alojar los datos a copiar
			_begin = new byte[source._maxsize];
			_maxsize = source._maxsize;
		}
		else if(_begin == 0) {
			// Reservamos memoria para alojar los datos a copiar
			_begin = new byte[source._maxsize];
			_maxsize = source._maxsize;
		}

		// Copiamos los datos
		for(int i = 0; i < source._size; ++i) {
			_begin[i] = source._begin[i];
		}

		_current = source._current;
		_size = source._size;
		_delta = source._delta;
		
	}

	CBuffer::CBuffer(const CBuffer& source) : _begin(0) {
		clone(source);
	}

	CBuffer& CBuffer::operator=(const CBuffer& source) {
		if(this != &source) {
			clone(source);
		}
		
		return *this;
	}

	CBuffer::~CBuffer()
	{
		delete[] _begin;
	}

	byte* CBuffer::getbuffer()
	{
		return _begin;
	}

	size_t CBuffer::getSize()
	{
		return _size;
	}

	void CBuffer::reset()
	{
		_current = _begin;
	}

	void CBuffer::write(void* data, size_t datalength)
	{
		while(_size + datalength >= _maxsize)
			realloc();
		memcpy(_current,data,datalength);
		_current+=datalength;
		_size+=datalength;
	}

	void CBuffer::read(void* data,size_t datalength)
	{
		memcpy(data,_current,datalength);
		_current+=datalength;
	}

	void CBuffer::realloc()
	{
		byte* newbuffer = new byte[_maxsize+_delta];
		memcpy(newbuffer,_begin, _size);
		delete[] _begin;
		_begin = newbuffer;
		_current = _begin + _size;
		_maxsize+=_delta;	
	}

} // namespace Net