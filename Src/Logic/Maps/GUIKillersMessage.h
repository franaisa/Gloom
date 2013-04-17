#include "GUIManager.h"

namespace Logic{
	class CGUIManager;
}

namespace Logic{
	class GUIKillersMessage{
	public:
		GUIKillersMessage();

		~GUIKillersMessage();

		static GUIKillersMessage* getSingletonPtr(){return _instance;}

		void addKiller(const std::string &killer, const std::string &killed);

	private:
		static GUIKillersMessage* _instance;
		CGUIManager* _guiManager;
	};

}