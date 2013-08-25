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
	class CPlane;
}

//declaración de la clase
namespace Logic {

	class CGraphicPlane : public IComponent {
		DEC_FACTORY(CGraphicPlane);
	public:

		CGraphicPlane();

		virtual ~CGraphicPlane();

		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

	protected:

		virtual void onStart();

		unsigned int readLightMask(const Map::CEntity *entityInfo);

	private:

		Graphics::CPlane* _plane;

		/** Mascara de luces */
		unsigned int _lightMask;

		/** Orientacion del plano */
		Vector3 _orientation;

		/** Anchura del plano */
		float _width;

		/** Altura del plano */
		float _height;

		/** Numero de segmentos horizontales */
		unsigned int _xSegments;

		/** Numero de segmentos verticales */
		unsigned int _ySegments;

		/** Repeticion del tiling en la u */
		float _uTiling;

		/** Repeticion el tiling en la v */
		float _vTiling;

		/** Vector up del plano */
		Vector3 _upVector;

		/** Nombre del material que usa el plano */
		std::string _materialName;

		/** True si el plano genera sombras */
		bool _castShadows;

	}; // class CGraphicPlane

	REG_FACTORY(CGraphicPlane);

} // namespace Logic

#endif // __Logic_GraphicPlane_H
