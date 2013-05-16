/**
@file PositionSnapshot.h


@see Logic::CPositionSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_PositionSnapshot_H
#define __Logic_PositionSnapshot_H

#include "SnapshotGenerator.h"
#include "Basesubsystems/Math.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CPositionSnapshot : public CSnapshotGenerator {
		DEC_FACTORY(CPositionSnapshot);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CPositionSnapshot();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CPositionSnapshot();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		virtual void sendSnapshot();

		//__________________________________________________________________

		virtual void takeSnapshot();

	private:

		std::vector<Vector3> _positionBuffer;

	}; // class CPositionSnapshot

	REG_FACTORY(CPositionSnapshot);

} // namespace Logic

#endif // __Logic_Life_H
