/**
@file GraphicPlane.h

@see Logic::CGraphicPlane
@see Logic::IComponent

@author Paquito el chocolatero
@date Enero, 350 A.C.
*/

#ifndef __Logic_GraphicPlane_H
#define __Logic_GraphicPlane_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics {
	class CEntity;
}

//declaración de la clase
namespace Logic {

	class CGraphicPlane : public IComponent {
		DEC_FACTORY(CGraphicPlane);
	public:

		CGraphicPlane();

		virtual ~CGraphicPlane();

	protected:

		virtual void onStart();


	}; // class CGraphicPlane

	REG_FACTORY(CGraphicPlane);

} // namespace Logic

#endif // __Logic_GraphicPlane_H
