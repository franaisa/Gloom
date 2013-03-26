/**
@file Message.h

Contiene el tipo de datos de un mensaje.

@see Logic::CMessage

@author José Antonio García Yáñez
@author Francisco Aisa García
@author Rubén Mulero Guerrero
*/

#ifndef __Logic_Message_H
#define __Logic_Message_H

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
			UNASSIGNED				= 0xFFFFFFFF,
			SET_TRANSFORM			= 0x00000000,
			SET_ANIMATION			= 0x00000001,
			STOP_ANIMATION			= 0x00000002,
			CONTROL					= 0x00000003,
			AVATAR_WALK				= 0x00000004,
			KINEMATIC_MOVE			= 0x00000005,
			TOUCHED					= 0x00000006,
			UNTOUCHED				= 0x00000007,
			SWITCH					= 0x00000008,
			DAMAGED					= 0x00000009,
			CHANGE_WEAPON			= 0x0000000A,//10
			CHANGE_WEAPON_GRAPHICS	= 0x0000000B,//11
			COLLISION_DOWN			= 0x0000000C,//12
			REBOUND					= 0x0000000D,//13
			HUD_LIFE				= 0x0000000E,//14
			HUD_SHIELD				= 0x0000000F,//15
			HUD_AMMO				= 0x00000010,//16
			HUD_WEAPON				= 0x00000011,//17
			ADD_LIFE				= 0x00000012,//18
			ADD_SHIELD				= 0x00000013,//19
			ADD_AMMO				= 0x00000014,//20
			ADD_WEAPON				= 0x00000015,//21
			PLAYER_DEAD				= 0x00000016,//22
			SET_PHYSIC_POSITION		= 0x00000017,//23
			HUD_SPAWN				= 0x00000018,//24
			CAMERA_TO_ENEMY			= 0x00000019,//25
			SYNC_POSITION			= 0x0000001A,//26
			WAKEUP					= 0x0000001B,//27
			SLEEP					= 0x0000001C,//28
			ACTIVATE				= 0x0000001D,//29
			DEACTIVATE				= 0x0000001E,//30
			CEALING					= 0x0000001F,//31
			ADDFORCEPLAYER			= 0x00000020,//32
			SIDE					= 0x00000021,//33
			ELEVATOR_INITIAL		= 0x00000022,//34
			ELEVATOR_FINAL			= 0x00000023,//35
			ADD_FORCE_PHYSICS		= 0x00000024,//36
			CONTACT_ENTER			= 0x00000025,//37
			PLAYER_SPAWN			= 0x00000026,//38
			HUD_DEBBUG				= 0x00000027,//39
			CONTACT_EXIT			= 0x00000028,//40
			AUDIO					= 0x00000029,//41
			CHANGE_PLAYER_CLASS		= 0x00000030,//42
			CHANGE_MATERIAL			= 0x00000031 //43
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
			UNLEFT_CLICK,
			UNRIGHT_CLICK,
			UNMIDDLE_CLICK,
			UNBUTTON3_CLICK,
			SIDEJUMP_LEFT,
			SIDEJUMP_RIGHT,
			CONCATSIDEJUMP_LEFT,
			CONCATSIDEJUMP_RIGHT,
			USE_PRIMARY_SKILL,
			USE_SECONDARY_SKILL
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
	@author Francisco Aisa García
	@author Ruben Mulero Guerrero
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
		* Método virtual puro que serializa los datos internos de cada mensaje.
		* El puntero de escritura/lectura NO SE RESETEA en ningún caso. Si el
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
	// Macros para la adición de los componentes a la factoría //
	// de componentes. //
	/////////////////////////////////////////////////////////////

	/**
	Macro para la declaración de los métodos necesarios para que
	la factoria cree nuevas instancias del tipo de componentes y
	para que el componente se registre en la factoría.
	*/
	#define DEC_FACTORYMESSAGE(Class) \
	public: \
		/** \
		Crea un componente de la clase en la que se declara. \
		*/ \
		static CMessage* create(); \
		/** \
		Registra el componente de la clase en la factoría. \
		*/ \
		static bool regist(); \

	/**
	Macro para la implementación de los métodos necesarios para que
	la factoria cree nuevas instancias del tipo de componentes y
	para que el componente se registre en la factoría.
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
	Macro que invoca al método que registra la clase en la factoría.
	*/
	#define REG_FACTORYMESSAGE(Class) \
	static bool RegisteredFACTORYMESSAGE_##Class = Class::regist();

} // namespace Logic

#endif // __Logic_Message_H