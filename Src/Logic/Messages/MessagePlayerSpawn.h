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

		~CMessagePlayerSpawn(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Vector3 _spawnPosition;
	};
	REG_FACTORYMESSAGE(CMessagePlayerSpawn);
};

#endif