/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::CMessage

@author David Llans� Garc�a
*/
#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>

#include "BaseSubsystems/Math.h"
#include "Net/Buffer.h"
#include "Logic/Entity/MessageFactory.h"

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
			DAMAGED,
			CHANGE_WEAPON,
			CHANGE_WEAPON_GRAPHICS,
			COLLISION_DOWN,
			REBOUND
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
			JUMP,
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

	@author Jose Antonio Garc�a Y��ez
	@date Enero, 2013
    @ingroup grupoEntidad
	*/
	class CMessage { // Abstracta
	public:
		 TMessageType getMessageType();
		 // Inicializa los mensajes a los valores por defecto
		 CMessage(TMessageType t);
		 virtual ~CMessage();

		 // Control de referencias
		 void addSmartP();
		 void subSmartP();

		 /**
		  * M�todo virtual puro que serializa los datos internos de cada mensaje.
		  * El puntero de escritura/lectura NO SE RESETEA en ning�n caso. Si el 
		  * cliente quiere realizar lecturas debe realizar un reset sobre el buffer
		  * devuelto.
		  * OJO!!! La memoria reservada para el buffer devuelto se libera en el propio
		  * mensaje. El cliente NUNCA debe intentar efectuar un delete sobre el buffer
		  * devuelto (de lo contrario se lia muy parda).
		  */
		 virtual Net::CBuffer* serialize() = 0;

		 virtual void deserialize(Net::CBuffer& buffer) = 0;

	protected:
		TMessageType _type;
		unsigned char _smartP;
		/* Se utiliza para serializar mensajes */
		Net::CBuffer* _tempBuffer;
	};

/////////////////////////////////////////////////////////////
// Macros para la adici�n de los componentes a la factor�a // 
// de componentes.                                         //
/////////////////////////////////////////////////////////////
	
/** 
Macro para la declaraci�n de los m�todos necesarios para que 
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define DEC_FACTORYMESSAGE(Class) \
public: \
	/** \
	Crea un componente de la clase en la que se declara. \
	*/ \
    static CMessage* create(); \
	/** \
	Registra el componente de la clase en la factor�a. \
	*/ \
	static bool regist(); \

/** 
Macro para la implementaci�n de los m�todos necesarios para que
la factoria cree nuevas instancias del tipo de componentes y 
para que el componente se registre en la factor�a.
*/
#define IMP_FACTORYMESSAGE(Class) \
	CMessage* Class::create() \
	{ \
		CMessage* res = new Class(); \
		return res; \
	} \
	bool Class::regist() \
	{ \
		if (!CMessageFactory::getSingletonPtr()->has(#Class)) \
		{ \
			CMessageFactory::getSingletonPtr()->add(Class::create, #Class); \
		} \
		return true; \
	}

/** 
Macro que invoca al m�todo que registra la clase en la factor�a.
*/
#define REG_FACTORYMESSAGE(Class) \
	static bool RegisteredFACTORYMESSAGE_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Message_H