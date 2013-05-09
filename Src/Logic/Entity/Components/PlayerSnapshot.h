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
		//                            METODOS PROPIOS
		// =======================================================================


		virtual void sendSnapshot();

		//__________________________________________________________________

		virtual void takeSnapshot();

	private:

		std::vector<Matrix4> _transformBuffer;

	}; // class CPlayerSnapshot

	REG_FACTORY(CPlayerSnapshot);

} // namespace Logic

#endif // __Logic_Life_H
