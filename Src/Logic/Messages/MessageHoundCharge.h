#ifndef __Logic_MessageHoundCharge_H
#define __Logic_MessageHoundCharge_H

#include "Message.h"

namespace Logic {

	class CMessageHoundCharge : public CMessage{
	DEC_FACTORYMESSAGE(CMessageHoundCharge);
	public:
		CMessageHoundCharge();
		virtual ~CMessageHoundCharge();
		
		void isActive(bool active) { _isActive = active; }
		void setFilterMask(unsigned int mask) { _filterMask = mask; }
		void setForce(float force) { this->_force = force; }

		bool isActive() { return _isActive; }
		unsigned int getFilterMask() { return _filterMask; }
		float getForce() { return _force; }

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	
	private:
		bool _isActive;
		unsigned int _filterMask;
		float _force;
	};
	REG_FACTORYMESSAGE(CMessageHoundCharge);

};

#endif