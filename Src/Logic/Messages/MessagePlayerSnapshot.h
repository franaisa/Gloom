#ifndef __Logic_MessagePlayerSnapshot_H
#define __Logic_MessagePlayerSnapshot_H

#include "Message.h"

namespace Logic {

	struct AnimInfo {
		unsigned int tick;
		std::string animName;
		bool loop;
		bool stop;
		bool exclude;
	};

	struct AudioInfo {
		unsigned int tick;
		std::string audioName;
		bool loopSound;
		bool play3d;
		bool streamSound;
	};

	// No heredo de CMessageTransformSnapshot porque me
	// da problemillas con el constructor (por querer hacer un bypass
	// y llamar a CMessage directamente)
	class CMessagePlayerSnapshot: public CMessage {
	DEC_FACTORYMESSAGE(CMessagePlayerSnapshot);
	public:
		CMessagePlayerSnapshot();
		virtual ~CMessagePlayerSnapshot();

		void setTransformBuffer(const std::vector<Matrix4>& buffer);
		std::vector<Matrix4> getTransformBuffer();
		
		void setAnimationBuffer(const std::vector<AnimInfo>& buffer);
		std::vector<AnimInfo> getAnimationBuffer();

		void setAudioBuffer(const std::vector<AudioInfo>& buffer);
		std::vector<AudioInfo> getAudioBuffer();

		virtual Net::CBuffer serialize();
		virtual void deserialize(Net::CBuffer& buffer);
	private:
		 std::vector<Matrix4> _transformBuffer;
		 std::vector<AnimInfo> _animationBuffer;
		 std::vector<AudioInfo> _audioBuffer;
	};
	REG_FACTORYMESSAGE(CMessagePlayerSnapshot);
};

#endif