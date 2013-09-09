#ifndef __Logic_AudioListener_H
#define __Logic_AudioListener_H

namespace Logic {

	class IAudioListener {
	public:

		virtual void trackEnd(int channelIndex) = 0;

	}; // class IAudioListener

} // namespace Logic

#endif
