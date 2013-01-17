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
			UNASSIGNED = 0xFFFFFFFF,
			SET_TRANSFORM,
			SET_ANIMATION,
			STOP_ANIMATION,
			CONTROL,
			AVATAR_WALK,
			KINEMATIC_MOVE,
			TOUCHED,
			UNTOUCHED,
			SWITCH,
			DAMAGED
		};
	}

	/**
	Tipo copia para los mensajes. Por simplicidad.
	*/
	typedef Message::TMessageType TMessageType;

	/**
	Contiene el tipo de datos de un mensaje. Tiene una serie de
	atributos genéricos que se interpretarán en función del tipo 
	de mensaje.
	<p>
	@remarks <b>¡¡ESTO NO ES ESCALABLE!!</b> En tu proyecto esto 
	debería ser cambiado.
	Lo suyo sería tener una clase CMesage base y luego clases que
	especializasen a ésta con los atributos necesarios para el 
	mensaje concreto en vez de tener una serie de atributos
	genéricos como es esta estructura que deben de ser interpretados
	externamente en función del tipo de mensaje.
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó García
	@date Julio, 2010
    @ingroup grupoEntidad
	*/
	class CMessage{
	public:
		 TMessageType getMessageType();
		 virtual ~CMessage(){};
	protected:
		TMessageType _type;
	};

	class CMessageControl : public CMessage{
	public:
		CMessageControl(TMessageType t);
		std::string getString();
		void setString(std::string);
		void setMouse(float mouse[]);
		float* getMouse();
		~CMessageControl(){};

	private:
		std::string _string;
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
		int getChange();
		void setChange(int change);
		~CMessageSwitch(){};
		
	private:
		int _change;
	};


	class CMessageDamaged: public CMessage{
	public:
		CMessageDamaged(TMessageType t);
		float getDamage();
		void setDamage(float damage);
		~CMessageDamaged(){};
		
	private:
		int _damage;
	};

} // namespace Logic

#endif // __Logic_Message_H
