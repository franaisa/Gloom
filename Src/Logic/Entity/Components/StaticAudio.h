/**
@file StaticAudio.h

Contiene la declaración del componente que controla el sonido de la entidad.

@see Logic::CStaticAudio
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Logic_StaticAudio_H
#define __Logic_StaticAudio_H

#include "Logic/Entity/Component.h"

// Declaración de la clase
namespace Logic {
	/**

    @ingroup logicGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/
	class CStaticAudio : public IComponent {
		DEC_FACTORY(CStaticAudio);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CStaticAudio();

		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

	protected:

		virtual void onActivate();

		virtual void onDeactivate();

	private:

		bool _streamAudio;
		std::string _audioName;
		Vector3 _position;
		unsigned int _channelIndex;
	}; // class CStaticAudio

	REG_FACTORY(CStaticAudio);

} // namespace Logic

#endif // __Logic_Audio_H
