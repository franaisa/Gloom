/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::CMessage

@author David Llansó García
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "BaseSubsystems/Math.h"
#include "Net/Buffer.h"

// Predeclaraciones
namespace Logic {
	class CEntity;
};

namespace Net {
	typedef unsigned char byte;
};

namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace Message
	{
		enum TMessageType
		{
			UNASSIGNED          = 0xFFFFFFFF,
			SET_TRANSFORM       = 0x00000000,
			SET_ANIMATION       = 0x00000001,
			STOP_ANIMATION      = 0x00000002,
			CONTROL	            = 0x00000003,
			AVATAR_WALK         = 0x00000004,
			TOUCHED             = 0x00000005,
			UNTOUCHED           = 0x00000006,
			SWITCH              = 0x00000007,
			DAMAGED             = 0x00000008,
			MOVE_TO             = 0x00000009,
			ROUTE_TO            = 0x0000000A,
			FINISHED_ROUTE      = 0x0000000B,
			FAILED_ROUTE        = 0x0000000C,
			KINEMATIC_MOVE		= 0x0000000D
		};
	}
	/**
	Namespace para los tipos de mensajes de control posibles.
	*/
	namespace Control
	{
			enum ControlType
		{
			UNASSIGNED			= 0xFFFFFFFF,
			WALK				= 0x00000000,
			WALKBACK			= 0x00000001,
			STOP_WALK			= 0x00000002,
			STOP_WALKBACK		= 0x00000003,
			STRAFE_LEFT			= 0x00000004,
			STRAFE_RIGHT		= 0x00000005,
			STOP_STRAFE_LEFT	= 0x00000006,
			STOP_STRAFE_RIGHT	= 0x00000007,
			MOUSE				= 0x00000008,
			LEFT_CLICK			= 0x00000009,
			RIGHT_CLICK			= 0x0000000A,
			MIDDLE_CLICK		= 0x0000000B,
			BUTTON3_CLICK		= 0x0000000C,
			JUMP				= 0x0000000D
		};

	}
	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Message::TMessageType TMessageType;

	/**
	Tipo copia para los mensajes de control. Por simplicidad.
	*/
	typedef Control::ControlType ControlType;


	/**
		Contiene la jerarquia de mensajes que implementaremos.
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author Jose Antonio García Yáñez
	@date Enero, 2013
    @ingroup grupoEntidad
	*/
	class CMessage { // Abstracta
	public:
		 TMessageType getMessageType();
		 // Inicializa los mensajes a los valores por defecto
		 CMessage(TMessageType t);
		 // Inicializa los valores de sus campos a partir de una ristra de bytes
		 // con datos serializados
		 CMessage(Net::byte* serializedData, size_t dataSize = 500);
		 virtual ~CMessage(){ /* Nada que hacer, no hay memoria dinámica */ };

		 // Control de referencias
		 void addSmartP();
		 void subSmartP();
		 
		 // Serializa los datos internos de cada mensaje concreto
		 // Por defecto, devuelve un buffer con tan solo el tipo del mensaje
		 // (que es la implementación mínima).
		 // Aquellos mensajes que tengan más parámetros deberán sobreescribir
		 // este método.
		 // Igualmente deberían apoyarse en la implementación del padre
		 // OJO!! NO RESETEA EL PUNTERO DE ESCRITURA/LECTURA POR DEFECTO
		 // el motivo principial es para que las clases derivadas solo tengan
		 // que llamar a write para escribir sus datos reutilizando la implentacion
		 // del padre.
		 // Estamos presuponiendo que nadie va a instanciar a CMessage.
		 virtual Net::CBuffer serialize() = 0;

	protected:
		TMessageType _type;
		unsigned char _smartP;
		/* Se usa en la construccion del objeto y en el método serialize */
		Net::CBuffer _tempBuffer;
	};

	// _________________________________________________

	class CMessageControl : public CMessage{
	public:
		CMessageControl(TMessageType t);
		ControlType getType();
		void setType(ControlType controltype);
		~CMessageControl(){};

		virtual Net::CBuffer serialize();

	protected:
		ControlType _controlType;
	};

	// _________________________________________________

	class CMessageMouse : public CMessageControl{
	public:
		CMessageMouse(TMessageType t);
		void setMouse(float mouse[]);
		float* getMouse();
		~CMessageMouse(){};

		virtual Net::CBuffer serialize();

	private:
		float _mouse[2];
	};

	// _________________________________________________

	class CMessageTransform : public CMessage{
	public:
		CMessageTransform(TMessageType t);
		Matrix4 getTransform();
		void setTransform(Matrix4 transform);
		~CMessageTransform(){};
		
		virtual Net::CBuffer serialize();

	private:
		Matrix4 _transform;
	};

	// _________________________________________________

	class CMessageTouched : public CMessage{
	public:
		CMessageTouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageTouched(){};
		
		virtual Net::CBuffer serialize();

	private:
		CEntity *_entity;
	};

	// _________________________________________________

	class CMessageUntouched : public CMessage{
	public:
		CMessageUntouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageUntouched(){};
		
		virtual Net::CBuffer serialize();

	private:
		CEntity *_entity;
	};

	// _________________________________________________

	class CMessageSetAnimation : public CMessage{
	public:
		CMessageSetAnimation(TMessageType t);
		std::string getString();
		void setString(std::string);
		bool getBool();
		void setBool(bool boolean);
		~CMessageSetAnimation(){};

		virtual Net::CBuffer serialize();

	private:
		std::string _string;
		bool _bool;
	};

	// _________________________________________________

	class CMessageStopAnimation : public CMessage{
	public:
		CMessageStopAnimation(TMessageType t);
		std::string getString();
		void setString(std::string);
		bool getBool();
		void setBool(bool boolean);
		~CMessageStopAnimation(){};

		virtual Net::CBuffer serialize();
		
	private:
		std::string _string;
		bool _bool;
	};

	// _________________________________________________

	class CMessageSwitch: public CMessage{
	public:
		CMessageSwitch(TMessageType t);
		unsigned char getChange();
		void setChange(unsigned char change);
		~CMessageSwitch(){};
		
		virtual Net::CBuffer serialize();

	private:
		unsigned char _change;
	};

	// _________________________________________________

	class CMessageDamaged: public CMessage{
	public:
		CMessageDamaged(TMessageType t);
		unsigned char getDamage();
		void setDamage(unsigned char damage);
		~CMessageDamaged(){};
		
		virtual Net::CBuffer serialize();

	private:
		unsigned char _damage;
	};

	// _________________________________________________

	class CMessageAvatarWalk: public CMessage{
	public:
		CMessageAvatarWalk(TMessageType t);
		Vector3 getDirection();
		void setDirection(Vector3 direction);
		~CMessageAvatarWalk(){};

		virtual Net::CBuffer serialize();
		
	private:
		Vector3 _direction;
	};

	// _________________________________________________

	class CMessageKinematicMove: public CMessage{
	public:
		CMessageKinematicMove(TMessageType t);
		Vector3 getMovement();
		void setMovement(Vector3 movement);
		~CMessageKinematicMove(){};

		virtual Net::CBuffer serialize();
		
	private:
		Vector3 _movement;
	};


} // namespace Logic

#endif // __Logic_Message_H
