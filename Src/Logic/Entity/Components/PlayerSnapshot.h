/**
@file PlayerSnapshot.h


@see Logic::CPlayerSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_PlayerSnapshot_H
#define __Logic_PlayerSnapshot_H

#include "SnapshotGenerator.h"

#include "Basesubsystems/Math.h"

#include "Logic/Messages/MessagePlayerSnapshot.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CPlayerSnapshot : public CSnapshotGenerator {
		DEC_FACTORY(CPlayerSnapshot);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CPlayerSnapshot();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CPlayerSnapshot();

		
		// =======================================================================
		//                     METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================


		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<CMessage>& message);

		virtual void process(const std::shared_ptr<CMessage>& message);

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		virtual void sendSnapshot();

		//__________________________________________________________________

		virtual void takeSnapshot();

	protected:

		virtual void onFixedTick(unsigned int msecs);

	private:

		unsigned int _ticksPerBuffer;
		unsigned int _tickCounter;

		std::vector<Matrix4>	_transformBuffer;
		std::vector<AnimInfo>	_animationBuffer;
		std::vector<AudioInfo>	_audioBuffer;
		std::vector<WeaponInfo> _weaponBuffer;
	}; // class CPlayerSnapshot

	REG_FACTORY(CPlayerSnapshot);

} // namespace Logic

#endif // __Logic_Life_H
