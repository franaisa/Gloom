#ifndef __Logic_MessageParticleVisibility_H
#define __Logic_MessageParticleVisibility_H

#include "Message.h"

namespace Logic {

	class CMessageParticleVisibility : public CMessage{
	DEC_FACTORYMESSAGE(CMessageParticleVisibility);
	public:
		CMessageParticleVisibility();
		virtual ~CMessageParticleVisibility(){};
		
		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);

		std::string getNameParticle(){return _nameParticle;};
		void setNameParticle(std::string nameParticle){ _nameParticle = nameParticle;};

		bool getVisibility(){ return _visibility;}
		void setVisibility (bool visibility){ _visibility = visibility; }

	private:
		std::string _nameParticle;
		bool _visibility;

	};
	REG_FACTORYMESSAGE(CMessageParticleVisibility);
};

#endif