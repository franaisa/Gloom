#include "Logic/Entity/Component.h"


namespace Hikari{
	class FlashControl;
}

namespace Logic{


	class CSpectatorHud: public IComponent{

		DEC_FACTORY(CSpectatorHud);

	public:

		CSpectatorHud();

		~CSpectatorHud();

		// =======================================================================
		//                     METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		

		virtual void onDeactivate();

		virtual void onActivate();

		void updateMatchTime(int minutes, int seconds);

		void freelook();

		void lookAt(const std::string &name);

	private:

		Hikari::FlashControl * _hud;
	};
	REG_FACTORY(CSpectatorHud);
}