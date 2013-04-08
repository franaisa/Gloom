#ifndef __Logic_MessageCreateParticle_H
#define __Logic_MessageCreateParticle_H

#include "Message.h"

namespace Logic {

	class CMessageCreateParticle: public CMessage{
	DEC_FACTORYMESSAGE(CMessageCreateParticle);
	public:
		CMessageCreateParticle();
		std::string getParticle();
		Vector3 getPosition();
		void setParticle(const std::string &name);
		void setPosition(const Vector3 &pos);

		Vector3 getDirectionWithForce();
		void setDirectionWithForce(const Vector3 &dirWithForce);
		~CMessageCreateParticle(){};
		
		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 std::string _name;
		 Vector3 _position;
		 Vector3 _directionWithForce;
	};
	REG_FACTORYMESSAGE(CMessageCreateParticle);
};

#endif