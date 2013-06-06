#ifndef __Logic_MessagePlayerSpawn_H
#define __Logic_MessagePlayerSpawn_H

#include "Message.h"

namespace Logic {

	class CMessagePlayerSpawn: public CMessage{
	DEC_FACTORYMESSAGE(CMessagePlayerSpawn);
	public:
		CMessagePlayerSpawn();

		Vector3 getSpawnPosition();
		void setSpawnPosition(const Vector3& position);
		Quaternion getSpawnOrientation();
		void setSpawnOrientation(const Quaternion& orientation);

		virtual ~CMessagePlayerSpawn(){};

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _position;
		Quaternion _orientation;
	};
	REG_FACTORYMESSAGE(CMessagePlayerSpawn);
};

#endif