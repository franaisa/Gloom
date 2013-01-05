#include "GUIDescriptor.h"
#include "GUIEventArgs.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

using namespace std;

namespace GUI {

	GUIDescriptor::GUIDescriptor(void) : _window(NULL) {
	} // default constructor

	//____________________________________________________________________________________________

	GUIDescriptor::GUIDescriptor(const string& layoutName) {
		this->_layoutName = layoutName;

		// Create the window on CEGUI
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(_layoutName + ".layout");
			_window = CEGUI::WindowManager::getSingleton().getWindow(_layoutName);
	}

	//____________________________________________________________________________________________

	GUIDescriptor::~GUIDescriptor(void) {
	} // destructor

	//____________________________________________________________________________________________

	CEGUI::Window* GUIDescriptor::activate() {
		// Activate the window that contains the layout especified in "_layoutName.layout"
		CEGUI::System::getSingletonPtr()->setGUISheet(_window);
		_window->setVisible(true);
		_window->activate();

		return _window;
	}
};