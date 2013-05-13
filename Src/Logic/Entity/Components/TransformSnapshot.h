/**
@file TransformSnapshot.h


@see Logic::CTransformSnapshot
@see Logic::IComponent

@author Francisco Aisa García
@date Mayo, 2013
*/

#ifndef __Logic_TransformSnapshot_H
#define __Logic_TransformSnapshot_H

#include "SnapshotGenerator.h"
#include "Basesubsystems/Math.h"

namespace Logic {
	
	/**
	
    @ingroup logicGroup

	@author Francisco Aisa García
	@date Mayo, 2013
	*/
	
	class CTransformSnapshot : public CSnapshotGenerator {
		DEC_FACTORY(CTransformSnapshot);
	public:


		// =======================================================================
		//                      CONSTRUCTORES Y DESTRUCTOR
		// =======================================================================


		/** Constructor por defecto. */
		CTransformSnapshot();

		//__________________________________________________________________

		/** Destructor. */
		virtual ~CTransformSnapshot();

		
		// =======================================================================
		//                            METODOS PROPIOS
		// =======================================================================


		virtual void sendSnapshot();

		//__________________________________________________________________

		virtual void takeSnapshot();

	private:

		std::vector<Matrix4> _transformBuffer;

	}; // class CTransformSnapshot

	REG_FACTORY(CTransformSnapshot);

} // namespace Logic

#endif // __Logic_Life_H
