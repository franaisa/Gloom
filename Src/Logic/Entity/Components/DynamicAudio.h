/**
@file DynamicAudio.h

Contiene la declaración del componente que 
controla el sonido de la entidad. Es igual
que CAudio, con la diferencia de que en el 
tick actualiza la posicion de la fuente de
sonido.

@see Logic::CAudio
@see Logic::CDynamicAudio
@see Logic::IComponent

@author Francisco Aisa García
@date Septiembre, 2013
*/
#ifndef __Logic_DynamicAudio_H
#define __Logic_DynamicAudio_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/AudioListener.h"

namespace Audio {
	class CServer;
}

namespace Logic {

	/**
	@date Marzo, 2013
	*/

	class CDynamicAudio : public IComponent, public IAudioListener {
		DEC_FACTORY(CDynamicAudio);
	public:

		CDynamicAudio();
		virtual ~CDynamicAudio();

		virtual void trackEnd(int channelIndex);

	protected:

		/**
		Este componente sólo acepta mensajes de tipo AUDIO.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo AUDIO se encarga de comunicarselo al server de audio.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);

		virtual void onActivate();

		virtual void onDeactivate();

		virtual void onStart();

		virtual void onTick(unsigned int msecs);

	private:

		Vector3 _speedVector;
		Vector3 _previousPosition;
		
		std::map<std::string, int> _tracksBeingPlayed;
		Audio::CServer* _audioServer;

	}; // class CDynamicAudio

	REG_FACTORY(CDynamicAudio);

} // namespace Logic

#endif // __Logic_Audio_H
