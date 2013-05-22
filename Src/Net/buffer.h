//---------------------------------------------------------------------------
// buffer.h
//---------------------------------------------------------------------------

/**
@file buffer.h
 
Implementa un buffer dinámico que los objetos utilizan para serializarse o deserializarse
 
@author Francisco Aisa García
@author Rubén Mulero Guerrero
@date Enero, 2013
*/

#ifndef __BUFFER_H
#define __BUFFER_H

#include "BaseSubsystems\Math.h"
#include "Logic/PlayerInfo.h"

namespace Map {
	class CEntity;
}

namespace Net {

	typedef unsigned char byte;

	/**
	@ingroup logicGroup

	Buffer para almacenar los datos que van a ser transmitidos por la red.
	Reajusta su tamaño de forma dinámica e implementa copy-on-write.

	@author Francisco Aisa García
	@author Rubén Mulero Guerrero
	@date Febrero, 2013
	*/

	class CBuffer {
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/**
		Crea un buffer dinámico.

		@param initsize Tamaño inicial del buffer.
		@param delta Indica cuanto crecerá el buffer cada vez que se necesite ampliarlo.
		*/
		CBuffer(size_t initsize = 100, size_t delta = 100);

		//________________________________________________________________________

		/** 
		Constructor de copias. Usa Copy-on-write. 
		
		@param source Buffer a partir del cual se va a construir nuestro buffer.
		*/
		CBuffer(const CBuffer& source);

		//________________________________________________________________________

		/**
		Constructor por parámetro. Construye un mensaje a partir de una ristra de bytes.
		Tras la construccion NO SE RECOLOCA EL PUNTERO DE LECTURA/ESCRITURA, hay que hacerlo
		manualmente.
		El propio buffer se encargará de elminar los datos reservados en la cadena de bytes pasada.
		OJO!! se asume que el tamaño pasado es el tamaño lógico y físico! por lo que la ristra de bytes
		que se pasa se espera que sea del tamaño exacto de los datos que contiene.

		@param adoptBuffer Ristra de bytes a partir de las cuales se va a construir el buffer.
		@param bufferSize Tamaño de la ristra de bytes.
		*/
		CBuffer::CBuffer(byte* adoptBuffer, size_t bufferSize);

		//________________________________________________________________________

		/** Destructor */
		~CBuffer();


		// =======================================================================
		//                              OPERADORES
		// =======================================================================


		/**
		Operador de asignacion.

		@param source Buffer que se va a asignar a nuestro buffer.
		@return Referencia a nuestro buffer.
		*/
		CBuffer& operator=(const CBuffer& source);


		// =======================================================================
		//                               GETTERS
		// =======================================================================
		
		
		/** Devuelve el buffer como una secuencia de bytes. */
		byte* getbuffer();

		//________________________________________________________________________

		/**
		Devuelve el tamaño del buffer. 
		Se utilizará normalmente para saber cuantos bytes devuelve getbuffer()
		*/
		size_t getSize();


		// =======================================================================
		//                              MODIFICADORES
		// =======================================================================


		/**
		Reinicia el buffer. No modifica su tamaño actual, tan solo resetea el puntero
		de lectura/escritura.
		*/
		void reset();

		//________________________________________________________________________

		/**
		Escribe datos en el buffer y modifica el puntero de lectura/escritura.
		Si alguien quiere hacer una lectura desde el principio tiene que llamar
		primero a reset().

		@param data Datos a escribir.
		@param datalenght Tamaño de los datos a escribir (número de bytes).
		*/
		void write(void* data, size_t datalength);

		//________________________________________________________________________

		/**
		Lee datos del buffer y modifica el puntero de lectura/escritura.
		
		@param data Puntero que indica donde se deben copiar los datos desde el buffer.
		@param datalength Número de datos (en bytes) a leer.
		*/
		void read (void* data, size_t datalength);


		// =======================================================================
		//                        METODOS DE SERIALIZACION
		// =======================================================================


		/**
		Escribe un vector3 en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(const Vector3& data);

		//________________________________________________________________________

		/**
		Lee un vector3 del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(Vector3& data);

		//________________________________________________________________________

		/**
		Escribe un float en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(float data);

		//________________________________________________________________________

		/**
		Lee un float del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(float& data);

		//________________________________________________________________________

		/**
		Escribe un entero en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(int data);

		//________________________________________________________________________

		/**
		Lee un entero del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(int& data);

		//________________________________________________________________________
		
		/**
		Escribe un entero sin signo en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(unsigned int data);

		//________________________________________________________________________

		/**
		Lee un entero sin signo del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(unsigned int& data);

		//________________________________________________________________________
		
		/**
		Escribe un string en el buffer dando la opcion de usar el CRC32.
		
		@param data Datos a serializar.
		@param crc true si queremos que se le aplique el CRC32 al string, en
		caso contrario serializa el tamaño del string y la ristra de caracteres
		de dicho string.
		*/
		void serialize(const std::string& data, bool crc);

		//________________________________________________________________________

		/**
		Lee un string del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(std::string& data);
		
		//________________________________________________________________________

		/**
		Escribe un caracter sin signo en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(unsigned char data);

		//________________________________________________________________________

		/**
		Lee un caracter sin signo del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(unsigned char& data);

		//________________________________________________________________________

		/**
		Escribe un booleano en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(bool data);

		//________________________________________________________________________

		/**
		Lee un booleano del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(bool& data);
		
		//________________________________________________________________________

		/**
		Escribe un caracter en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(char data);

		//________________________________________________________________________

		/**
		Lee un caracter del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(char& data);
		
		//________________________________________________________________________

		/**
		Escribe una matriz de transformacion en el buffer.
		
		@param data Datos a serializar.
		*/
		void serialize(const Matrix4& data);

		//________________________________________________________________________

		/**
		Lee una matriz de transformacion del buffer.
		
		@param data Datos a deserializar.
		*/
		void deserialize(Matrix4& data);

		//________________________________________________________________________

		/**
		Lee una entidad del mapa (en formato string para la factoria) y la serializa
		en el buffer.

		IMPORTANTE: entityInfo no puede ser null, de lo contrario explota!!

		@param entityInfo Información sobre la entidad que queremos serializar.
		*/
		void serialize(const Map::CEntity* entityInfo);

		//________________________________________________________________________

		/**
		Deserializa la información de entidad contenida en el buffer en el parámetro
		dado.

		IMPORTANTE: Se asume que el cliente reserva la memoria del puntero, de lo
		contrario explota.

		@param entityInfo Información sobre la entidad.
		*/
		void deserialize(Map::CEntity* entityInfo);


	private:

		// =======================================================================
		//                          METODOS PRIVADOS
		// =======================================================================


		/**
		Se encarga de ampliar la memoria reservada para el buffer y rellenar dicha
		memoria.
		*/
		void realloc();

		//________________________________________________________________________

		/**
		Crea una instancia propia del buffer. Util para copy-on-write.
		*/
		void createOwnInstance();


		// =======================================================================
		//                          MIEMBROS PRIVADOS
		// =======================================================================

		/** Wrapper para facilitar el uso de copy-on-write. */
		struct BufferWrapper {
			/** Puntero al comienzo del buffer. */
			byte* _begin;
			
			/** Puntero de lectura/escritura. */
			byte* _current;
			
			/** Tamaño de la memoria fisica reservada. */
			size_t _maxsize;
			
			/** Tamaño de la memoria logica ocupada. */
			size_t _size;
			
			/** Tamaño delta. */
			size_t _delta;
		
			/** Contador de referencias para copy-on-write. */
			unsigned int _refCount;
		};

		/** Wrapper de los campos privados para usar copy-on-write. */
		BufferWrapper* _wrapperPtr;
	};

} // namespace Net

#endif // _CBUFFER_H