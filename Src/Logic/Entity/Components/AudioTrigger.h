/**
@file AudioTrigger.h

Contiene la declaración del componente que controla el sonido de la entidad.

@see Logic::CAudioTrigger
@see Logic::IComponent

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Logic_AudioTrigger_H
#define __Logic_AudioTrigger_H

#include "Logic/Entity/Component.h"

// Declaración de la clase
namespace Logic {
	/**

    @ingroup logicGroup

	@author Francisco Aisa García
	@date Agosto, 2013
	*/
	class CAudioTrigger : public IComponent {
		DEC_FACTORY(CAudioTrigger);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAudioTrigger();

		virtual bool spawn(CEntity* entity, CMap* map, const Map::CEntity* entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo AUDIO.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo AUDIO se encarga de comunicarselo al server de audio.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

	private:

		bool _stream;
		std::string _name;

	}; // class CAudioTrigger

	REG_FACTORY(CAudioTrigger);

} // namespace Logic

#endif // __Logic_Audio_H
