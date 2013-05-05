/**
@file Math.h

Este fichero contiene la definición de ciertos tipos de datos
utilizados por la aplicación y relacionados con la base
matemática; en particular, define distintos tipos de matriz,
vector, etc.
<p>
En la práctica, los tipos son simplemente tipos sinónimos
a los de Ogre, para evitar pérdidas de tiempo en conversiones
superfluas entre la aplicación y Ogre (sí habrá que hacerlas
entre la aplicación y el motor de física, por ejemplo).
<p>
Se ofrecen también una serie de funciones auxiliares.
 
@author David Llansó
@date Julio, 2010
*/

#ifndef __BaseSubsystems_Math_H
#define __BaseSubsystems_Math_H

// Includes de Ogre donde se definen los tipos
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include <OgreRay.h>
#include <OgreLight.h>
#include <string>

/**
Definicion de matriz de 4x4. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef Ogre::Matrix4 Matrix4;

/**
Definición de matriz de 3x3 de rotación. La definición del tipo 
de datos es la misma que la utilizada por el motor gráfico, por 
lo tanto es dependiente del motor usado.
 */
typedef Ogre::Matrix3 Matrix3;

/**
Vector (o punto) 2d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef Ogre::Vector2 Vector2;

/**
Vector (o punto) 3d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef	Ogre::Vector3 Vector3;

/**
Vector (o punto) 4d utilizado. La definición del tipo de datos
es la misma que la utilizada por el motor gráfico, por lo tanto
es dependiente del motor usado.
 */
typedef	Ogre::Vector4 Vector4;

/**
Quaternion, usado para cálculos de rotaciones tridimensionales. 
La definición del tipo de datos es la misma que la utilizada por 
el motor gráfico, por lo tanto es dependiente del motor usado.
 */
typedef Ogre::Quaternion Quaternion;

/**
Rayo. La definición del tipo de datos es la misma que la utilizada 
por el motor gráfico, por lo tanto es dependiente del motor usado.
 */
typedef Ogre::Ray Ray;

/**
Namespace en el que ofrecemos alguna definición de constante
matamática y métodos para convertir grados en radianes, etc.
*/
namespace Math
{
	/**
	Definición de la constante PI.
	*/
	static const float PI = float( 4.0f * atan( 1.0f ) );

	/**
	Definición de la constante PI.
	*/
	static const float HALF_PI = PI * 0.5f;
	/**
	Constante para pasar de grados a radianes.
	*/
	static const float _deg2Rad = PI / 180.0f;

	/**
	Constante para pasar de radianes a grados.
	*/
	static const float _rad2Deg = 180.0f / PI;

	/**
	Transforma grados en radianes.

	@param degree Ángulo en grados.
	@return Ángulo en radianes.
	*/
	static float fromDegreesToRadians(float degrees) {return degrees*_deg2Rad;}

	/**
	Transforma radianes en grados.

	@param radian Ángulo en radianes.
	@return Ángulo en grados.
	*/
	static float fromRadiansToDegrees(float radians) {return radians*_rad2Deg;}
	
	/**
	Crea un vector unitario de dirección a partir de un angulo de
	orientación en radianes.

	@param orientation Orientación en radianes.
	@return Vector unitario en el plano ZX.
	*/
	static Vector3 getDirection(float orientation) 
	{
		return Vector3(-sin(orientation), 0, -cos(orientation));

	} // getDirection

	/**
	Crea un vector unitario de dirección vertical a partir de un angulo de
	orientación en radianes.

	@param orientation Orientación en radianes.
	@return Vector unitario en el plano ZX.
	*/
	static Vector3 getDirectionPitch(float orientation) 
	{
		
		return Vector3(0, orientation, 0);

	} // getDirection
	
	/**
	Aplica un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere aplicar.
	@param transform Matriz de transformación a modificar.
	*/
	static void yaw(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(newYaw, pitch, roll);
		transform = rotation;

	} // yaw
	
	/**
	Extrae el estado del viraje de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Viraje de la entidad.
	*/
	static float getYaw(const Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		return yaw.valueRadians();

	} // getYaw
	
	/**
	Establece un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	static void setYaw(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::yaw(turn,transform);

	} // setYaw

	/**
	Aplica un subviraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere aplicar.
	@param transform Matriz de transformación a modificar.
	*/
	static void pitch(float turn, Matrix4& transform) 
	{

		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(yaw, newPitch, roll);

		/*
<<<<<<< HEAD
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(yaw, newPitch, roll);
=======
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);

		Ogre::Radian newPitch = pitch + Ogre::Radian(turn);

		rotation.FromEulerAnglesYXZ(yaw, newPitch, roll);
>>>>>>> movimiento
		*/

		transform = rotation;
	} // pitch
	
	/**
	Extrae el estado del subviraje de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Viraje de la entidad.
	*/
	static float getPitch(const Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		return pitch.valueRadians();

	} // getPitch
	
	/**
	Establece un subviraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	static void setPitch(float turn, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::pitch(turn,transform);

	} // setPitch

	

	/**
	Aplica un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere aplicar.
	@param transform Matriz de transformación a modificar.
	*/
	static void roll(float turn, Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newRoll = roll + Ogre::Radian(turn);
		rotation.FromEulerAnglesYXZ(yaw, pitch, newRoll);
		transform = rotation;

	} // roll
	
	/**
	Extrae el estado del viraje de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Viraje de la entidad.
	*/
	static float getRoll(const Matrix4& transform) 
	{
		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		return roll.valueRadians();

	} // getRoll
	
	/**
	Establece un viraje a una matriz de transformación.

	@param turn Giro en radianes que se quiere etablecer.
	@param transform Matriz de transformación a modificar.
	*/
	static void setRoll(float turn, Matrix4& transform)
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::roll(turn,transform);

	} // setRoll

	/**
	Aplica un viraje a una matriz de transformación.

	@param turnP Giro en radianes que se quiere aplicar al pitch.
	@param turnY Giro en radianes que se quiere aplicar al yaw.
	@param transform Matriz de transformación a modificar.
	*/
	static void pitchYaw(float turnP, float turnY, Matrix4& transform) 
	{

		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turnP);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turnY);

		rotation.FromEulerAnglesYXZ(newYaw, newPitch, roll);
		transform = rotation;

	} // pitchYaw


	/**
	Establece un subviraje a una matriz de transformación.

	@param turnP Giro en radianes que se quiere etablecer en el eje Y.
	@param turnY Giro en radianes que se quiere etablecer en el eje X.
	@param transform Matriz de transformación a modificar.
	*/
	static void setPitchYaw(float turnP, float turnY, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::pitchYaw(turnP,turnY,transform);
	} // setPitchYaw

	/**
	Aplica un viraje a una matriz de transformación.

	@param turnP Giro en radianes que se quiere aplicar al pitch.
	@param turnY Giro en radianes que se quiere aplicar al yaw.
	@param turnR Giro en radianes que se quiere aplicar al roll.
	@param transform Matriz de transformación a modificar.
	*/
	static void pitchYawRoll(float turnP, float turnY,float turnR, Matrix4& transform) 
	{

		Matrix3 rotation;
		transform.extract3x3Matrix(rotation);
		Ogre::Radian yaw, pitch, roll;
		rotation.ToEulerAnglesYXZ(yaw, pitch, roll);
		Ogre::Radian newPitch = pitch + Ogre::Radian(turnP);
		Ogre::Radian newYaw = yaw + Ogre::Radian(turnY);
		Ogre::Radian newRoll = roll + Ogre::Radian(turnR);

		rotation.FromEulerAnglesYXZ(newYaw, newPitch, newRoll);
		transform = rotation;

	} // pitch


	/**
	Establece un subviraje a una matriz de transformación.

	@param turnP Giro en radianes que se quiere etablecer en el eje Y.
	@param turnY Giro en radianes que se quiere etablecer en el eje X.
	@param turnR Giro en radianes que se quiere etablecer en el eje Z.
	@param transform Matriz de transformación a modificar.
	*/
	static void setPitchYawRoll(float turnP, float turnY, float turnR, Matrix4& transform) 
	{
		// Reiniciamos la matriz de rotación
		transform = Matrix3::IDENTITY;
		// Sobre esta rotamos.
		Math::pitchYawRoll(turnP,turnY,turnR,transform);

	} // setPitchYawRoll
	
	/**
	Crea un vector unitario de dirección en el plano ZX a partir 
	de una matriz de transformación.

	@param transform Matriz de transformación.
	@return Vector unitario en el plano ZX.
	*/
	static Vector3 getDirection(const Matrix4& transform) 
	{
		Vector3 yaw = getDirection(getYaw(transform));
		Vector3 pitch = getDirectionPitch(getPitch(transform));
		return yaw + pitch;

	} // getDirection

	/**
	Calcula el valor CRC de un string pasado como parametro.

	@param nombre string del que se desea calcular el CRC.
	@return el número entero resultado de calcular el CRC.
	*/
	static int CRC(const std::string nombre) 
	{
		
		int count = nombre.size();
		unsigned int  crc;
		unsigned int i;
		const char* ptr = nombre.c_str();
		crc = 0;

		while (--count >= 0)
		{
			crc = crc ^ (unsigned short)*ptr++ << 8;
			i = 8;
			do
			{
				if (crc & 0x8000)
					crc = crc << 1 ^ 0x1021;
				else
					crc = crc << 1;
			} while(--i);
		}
		return (crc);

	} // CRC



} // namespace Math

#endif // __BaseSubsystems_Math_H