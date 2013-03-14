#ifndef __Logic_MessagePlayerSpawn_H
#define __Logic_MessagePlayerSpawn_H

#include "Message.h"

namespace Logic {

	class CMessagePlayerSpawn: public CMessage{
	DEC_FACTORYMESSAGE(CMessagePlayerSpawn);
	public:
		CMessagePlayerSpawn();

		Matrix4 getSpawnTransform();
		void setSpawnTransform(const Matrix4& transform);

		~CMessagePlayerSpawn(){};

		virtual Net::CBuffer* serialize();
		virtual void deserialize(Net::CBuffer& buffer);
		
	private:
		Matrix4 _spawnTransform;
	};
	REG_FACTORYMESSAGE(CMessagePlayerSpawn);
};

#endif