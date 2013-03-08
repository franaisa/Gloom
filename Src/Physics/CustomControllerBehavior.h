#ifndef __Physics_CustomControllerBehavior_H
#define __Physics_CustomControllerBehavior_H

#include <PxPhysicsAPI.h>
#include <vector>

namespace Physics {
	class CCustomControllerBehavior : public physx::PxControllerBehaviorCallback {
	public:
		CCustomControllerBehavior(const std::vector<int>& shapesToIgnore) {
			filterMask = 0;
			for(int i = 0; i < shapesToIgnore.size(); ++i) {
				filterMask |= (1 << shapesToIgnore[i]);
			}
		}

		virtual physx::PxU32 getBehaviorFlags(const physx::PxShape &shape) {
			physx::PxFilterData filterData = shape.getSimulationFilterData();
			
			return filterData.word0 & filterMask ? 0 : physx::PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
		}
		
		virtual physx::PxU32 getBehaviorFlags(const physx::PxController& controller) {
			return 0;
		}

		virtual physx::PxU32 getBehaviorFlags(const physx::PxObstacle& obstacle) {
			return 0;
		}

	private:
		physx::PxU32 filterMask;
	};
};

#endif