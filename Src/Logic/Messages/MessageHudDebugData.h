#ifndef __Logic_MessageHudDebugData_H
#define __Logic_MessageHudDebugData_H

#include "Message.h"

namespace Logic {

	class CMessageHudDebugData: public CMessage{
	DEC_FACTORYMESSAGE(CMessageHudDebugData);
	public:
		CMessageHudDebugData();
		~CMessageHudDebugData(){};

		void setKey(std::string key);
		void setValue(std::string value);

		void setValue(float value);
		void setValue(Vector3 value);

		std::string getKey();
		std::string getValue();
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:

		std::string _key;
		std::string _value;
	};
	REG_FACTORYMESSAGE(CMessageHudDebugData);
};

#endif