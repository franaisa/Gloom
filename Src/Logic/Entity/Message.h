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

// Predeclaraciones
namespace Logic {
	class CEntity;
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
			FAILED_ROUTE        = 0x0000000C
		};
	}
	/**
	Namespace para los tipos de mensajes de control posibles.
	*/
	namespace Control
	{
			enum ControlType
		{
			UNASSIGNED = 0xFFFFFFFF,
			WALK,
			WALKBACK,
			STOP_WALK,
			STOP_WALKBACK,
			STRAFE_LEFT,
			STRAFE_RIGHT,
			STOP_STRAFE_LEFT,
			STOP_STRAFE_RIGHT,
			MOUSE,
			LEFT_CLICK,
			RIGHT_CLICK,
			MIDDLE_CLICK,
			BUTTON3_CLICK,
			JUMP
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
	class CMessage{
	public:
		 TMessageType getMessageType();
		 void addSmartP();
		 void subSmartP();
		 virtual ~CMessage(){};

	public:
		TMessageType _type;
		unsigned char _smartP;
	};



	class CMessageControl : public CMessage{
	public:
		CMessageControl(TMessageType t);
		ControlType getType();
		void setType(ControlType controltype);
		~CMessageControl(){};

	protected:
		ControlType _controlType;
	};


	class CMessageMouse : public CMessageControl{
	public:
		CMessageMouse(TMessageType t);
		void setMouse(float mouse[]);
		float* getMouse();
		~CMessageMouse(){};

	private:
		float _mouse[2];
	};


	class CMessageTransform : public CMessage{
	public:
		CMessageTransform(TMessageType t);
		Matrix4 getTransform();
		void setTransform(Matrix4 transform);
		~CMessageTransform(){};
		
	private:
		Matrix4 _transform;
	};


	class CMessageTouched : public CMessage{
	public:
		CMessageTouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageTouched(){};
		
	private:
		CEntity *_entity;
	};

	class CMessageUntouched : public CMessage{
	public:
		CMessageUntouched(TMessageType t);
		CEntity* getEntity();
		void setEntity(CEntity *c);
		~CMessageUntouched(){};
		
	private:
		CEntity *_entity;
	};

	class CMessageSetAnimation : public CMessage{
	public:
		CMessageSetAnimation(TMessageType t);
		std::string getString();
		void setString(std::string);
		bool getBool();
		void setBool(bool boolean);
		~CMessageSetAnimation(){};

	private:
		std::string _string;
		bool _bool;
	};

	class CMessageStopAnimation : public CMessage{
	public:
		CMessageStopAnimation(TMessageType t);
		std::string getString();
		void setString(std::string);
		bool getBool();
		void setBool(bool boolean);
		~CMessageStopAnimation(){};
		
	private:
		std::string _string;
		bool _bool;
	};

	class CMessageSwitch: public CMessage{
	public:
		CMessageSwitch(TMessageType t);
		unsigned char getChange();
		void setChange(unsigned char change);
		~CMessageSwitch(){};
		
	private:
		unsigned char _change;
	};


	class CMessageDamaged: public CMessage{
	public:
		CMessageDamaged(TMessageType t);
		unsigned char getDamage();
		void setDamage(unsigned char damage);
		~CMessageDamaged(){};
		
	private:
		unsigned char _damage;
	};

	class CMessageAvatarWalk: public CMessage{
	public:
		CMessageAvatarWalk(TMessageType t);
		Vector3 getDirection();
		void setDirection(Vector3 direction);
		~CMessageAvatarWalk(){};
		
	private:
		Vector3 _direction;
	};

	class CMessageKinematicMove: public CMessage{
	public:
		CMessageKinematicMove(TMessageType t);
		Vector3 getMovement();
		void setMovement(Vector3 movement);
		~CMessageKinematicMove(){};
		
	private:
		Vector3 _movement;
	};

} // namespace Logic

#endif // __Logic_Message_H
