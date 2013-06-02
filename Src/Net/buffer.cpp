//---------------------------------------------------------------------------
// buffer.cpp
//---------------------------------------------------------------------------

/**
 * @file buffer.cpp
 *
 * Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 *
 * @author Francisco Aisa García
 * @author Rubén Mulero Guerrero
 * @date Febrero, 2013
 */

#include "buffer.h"
#include "memory.h"
#include "Map/MapEntity.h"

#define DEBUG_BUFFER 1

namespace Net {

	CBuffer::CBuffer(size_t initsize, size_t delta) {
		// Creamos un nuevo wrapper para alojar los datos del buffer
		_wrapperPtr = new BufferWrapper;

		// Inicializamos los datos propios del buffer
		_wrapperPtr->_current = _wrapperPtr->_begin = new byte[initsize];
		_wrapperPtr->_maxsize = initsize;
		_wrapperPtr->_size = 0;
		_wrapperPtr->_delta = delta;

		// Tenemos una referencia de este buffer
		_wrapperPtr->_refCount = 1;
	}

	//__________________________________________________________________

	CBuffer::CBuffer(const CBuffer& source) {
		// Para evitar la copia incrementamos el contador de referencias
		++(source._wrapperPtr->_refCount);
		// Apuntamos al recurso compartido
		_wrapperPtr = source._wrapperPtr;
	}

	//__________________________________________________________________

	CBuffer::CBuffer(byte* adoptBuffer, size_t bufferSize) {
		// Creamos un nuevo wrapper para alojar los datos del buffer
		_wrapperPtr = new BufferWrapper;

		// Inicializamos los datos propios del buffer
		_wrapperPtr->_current = _wrapperPtr->_begin = adoptBuffer;
		_wrapperPtr->_maxsize = bufferSize;
		_wrapperPtr->_size = bufferSize;
		_wrapperPtr->_delta = 100;

		// Tenemos una referencia de este buffer
		_wrapperPtr->_refCount = 1;
	}

	//__________________________________________________________________

	CBuffer& CBuffer::operator=(const CBuffer& source) {
		if(this == &source) {
			return *this;
		}

		// Incrementamos el número de referencias de source
		++(source._wrapperPtr->_refCount);
		// Eliminamos una referencia de nuestro buffer, ya que
		// nosotros dejamos de apuntar a él.
		// Comprobar si se trata de la última referencia para
		// destruir el objeto
		if(--(_wrapperPtr->_refCount) == 0) {
			delete [] _wrapperPtr->_begin;
			delete _wrapperPtr;
		}
		
		// Apuntamos al objeto que nos pasan
		_wrapperPtr = source._wrapperPtr;

		return *this;
	}

	//__________________________________________________________________

	CBuffer::~CBuffer() {
		if(--(_wrapperPtr->_refCount) == 0) {
			delete [] _wrapperPtr->_begin;
			delete _wrapperPtr;
		}
	}

	//__________________________________________________________________

	byte* CBuffer::getbuffer() {
		return _wrapperPtr->_begin;
	}

	//__________________________________________________________________

	size_t CBuffer::getSize() {
		return _wrapperPtr->_size;
	}

	//__________________________________________________________________

	void CBuffer::createOwnInstance() {
		// Decrementamos el contador de referencias
		// ya que vamos a crear nuestra propia copia
		// para ser modificada al gusto
		--(_wrapperPtr->_refCount);

		// Creamos un wrapper temporal para ir volcando los datos
		// a nuestro nuevo buffer
		BufferWrapper* tempBufferData = _wrapperPtr;
		
		// Creamos un nuevo wrapper y copiamos los datos del buffer
		_wrapperPtr = new BufferWrapper;
		_wrapperPtr->_begin = new byte [tempBufferData->_maxsize];
		//cambiado el int i a unsigned porque es sin signo (ERA WARNING)
		for(unsigned int i = 0; i < tempBufferData->_size; ++i) {
			_wrapperPtr->_begin[i] = tempBufferData->_begin[i];
		}

		// Seteamos el offset de nuestro puntero current
		_wrapperPtr->_current = _wrapperPtr->_begin + (tempBufferData->_current - tempBufferData->_begin);
		
		// Copiamos el resto de datos
		_wrapperPtr->_maxsize = tempBufferData->_maxsize;
		_wrapperPtr->_size = tempBufferData->_size;
		_wrapperPtr->_delta = tempBufferData->_delta;

		// Seteamos el contador de referencias a uno
		_wrapperPtr->_refCount = 1;
	}

	//__________________________________________________________________

	void CBuffer::reset() {
		if(_wrapperPtr->_refCount > 1) {
			createOwnInstance();
		}

		_wrapperPtr->_current = _wrapperPtr->_begin;
	}

	//__________________________________________________________________

	void CBuffer::write(void* data, size_t datalength) {
		if(_wrapperPtr->_refCount > 1) {
			createOwnInstance();
		}

		while(_wrapperPtr->_size + datalength >= _wrapperPtr->_maxsize)
			realloc();
		
		memcpy(_wrapperPtr->_current,data,datalength);
		_wrapperPtr->_current+=datalength;
		_wrapperPtr->_size+=datalength;
	}

	//__________________________________________________________________

	void CBuffer::read(void* data,size_t datalength) {
		if(_wrapperPtr->_refCount > 1) {
			createOwnInstance();
		}

		memcpy(data, _wrapperPtr->_current, datalength);
		_wrapperPtr->_current += datalength;
	}

	//__________________________________________________________________

	void CBuffer::realloc() {
		byte* newbuffer = new byte[_wrapperPtr->_maxsize + _wrapperPtr->_delta];
		memcpy(newbuffer, _wrapperPtr->_begin, _wrapperPtr->_size);
		delete[] _wrapperPtr->_begin;
		
		_wrapperPtr->_begin = newbuffer;
		_wrapperPtr->_current = _wrapperPtr->_begin + _wrapperPtr->_size;
		_wrapperPtr->_maxsize += _wrapperPtr->_delta;	
	}

	//__________________________________________________________________

	void CBuffer::serialize(int data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(int& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(const Vector3& data) {
		Vector3 temp = data;
		write(&(temp.x), sizeof(temp.x));
		write(&(temp.y), sizeof(temp.y));
		write(&(temp.z), sizeof(temp.z));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(Vector3& data) {
		read(&data.x, sizeof(data.x));
		read(&data.y, sizeof(data.y));
		read(&data.z, sizeof(data.z));
	}

	//__________________________________________________________________

	void CBuffer::serialize(const Quaternion& data) {	
		serialize(data.getYaw().valueAngleUnits());
		serialize(data.getPitch().valueAngleUnits());
		serialize(data.getRoll().valueAngleUnits());
	}

	//__________________________________________________________________

	void CBuffer::deserialize(Quaternion& data) {
		float yaw, pitch,roll;

		// Obtenemos Yaw, Pitch y Roll
		read(&yaw, sizeof(yaw));
		read(&pitch, sizeof(pitch));
		read(&roll, sizeof(roll));
		
		Quaternion y(Ogre::Radian(Math::fromDegreesToRadians(yaw)),Vector3::UNIT_Y);
		Quaternion p(Ogre::Radian(Math::fromDegreesToRadians(pitch)),Vector3::UNIT_X);
		Quaternion r(Ogre::Radian(Math::fromDegreesToRadians(roll)),Vector3::UNIT_Z);

		data=yaw*pitch*roll;
	}

	//__________________________________________________________________

	void CBuffer::serialize(const std::string& data, bool crc) {
		if(crc){
			int crc = Math::CRC(data);

			write(&crc, sizeof(crc));
		}else{
			unsigned int stringSize = data.size();
			write(&stringSize, sizeof(unsigned int));
			write(const_cast <char*>(data.c_str()), data.size());
		}
	}

	//__________________________________________________________________

	void CBuffer::deserialize(std::string& data) {
		int size;
		read(&size, sizeof(unsigned int));

		char * aux = (char*)malloc(size);
		read(aux, size);
		data.assign(aux, size);
	}

	//__________________________________________________________________

	void CBuffer::serialize(unsigned int data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(unsigned int& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(float data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(float& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(unsigned char data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(unsigned char& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(char data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(char& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(bool data) {
		write(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(bool& data) {
		read(&data, sizeof(data));
	}

	//__________________________________________________________________

	void CBuffer::serialize(const Matrix4& data) {
		Vector3 position = data.getTrans();
		serialize(position);
		serialize(Math::getYaw(data));
		serialize(Math::getPitch(data));
	}

	//__________________________________________________________________

	void CBuffer::deserialize(Matrix4& data) {
		Vector3 position;
		float yaw, pitch;

		// Obtenemos los datos del vector
		read(&position.x, sizeof(position.x));
		read(&position.y, sizeof(position.y));
		read(&position.z, sizeof(position.z));

		// Obtenemos el Yaw y el Pitch
		read(&yaw, sizeof(yaw));
		read(&pitch, sizeof(pitch));
		
		Math::setPitchYaw(pitch,yaw, data);
		data.setTrans(position);
	}

	//__________________________________________________________________

	void CBuffer::serialize(const Map::CEntity* entityInfo) {
		auto it = entityInfo->_attributes.begin();
		auto end = entityInfo->_attributes.end();

		// Serializamos el número de atributos que hay
		unsigned int attributesListSize = entityInfo->_attributes.size();
		write(&attributesListSize, sizeof(attributesListSize));
		for(; it != end; ++it) {
			serialize(std::string(it->first), false);
			serialize(std::string(it->second), false);
		}
	}

	//__________________________________________________________________

	void CBuffer::deserialize(Map::CEntity* entityInfo) {
		unsigned int attributesListSize;
		read(&attributesListSize, sizeof(attributesListSize));

		std::string key, value;
		for(int i = 0; i < attributesListSize; ++i) {
			deserialize(key);
			deserialize(value);

			entityInfo->_attributes[key] = value;
		}
	}

} // namespace Net