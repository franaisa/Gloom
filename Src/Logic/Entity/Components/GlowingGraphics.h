/**
@file GlowingGraphics.h

@see Logic::CGlowingGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_GlowingGraphics_H
#define __Logic_GlowingGraphics_H

#include "Logic/Entity/Component.h"
#include "AnimatedGraphics.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics {
	class CEntity;
	class CGlowingEntity;
}

//declaración de la clase
namespace Logic {

	/**

    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
	*/
	
	class CGlowingGraphics : public CAnimatedGraphics {
		DEC_FACTORY(CGlowingGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CGlowingGraphics();

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CGlowingGraphics();

	protected:

		/**
		Método virtual que construye la entidad gráfica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gráficas (animadas, etc.).
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);
		
		/**
		Entidad gráfica.
		*/
		Graphics::CGlowingEntity* _glowingGraphicsEntity;

	}; // class CGraphics

	REG_FACTORY(CGlowingGraphics);

} // namespace Logic

#endif // __Logic_GlowingGraphics_H
