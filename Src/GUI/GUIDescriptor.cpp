/*
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version. This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. You should have received a copy of the GNU
 * General Public License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 * USA.
 *
 * Copyright © 2011-2012 
 * @author Francisco Aisa Garcia
 */

// GUI HEADERS
#include "GUIDescriptor.h"
#include "GUIEventArgs.h"

// CEGUI HEADERS
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

// NAMESPACES IN USE
using namespace std;

namespace GUI {

	// SHOULD NEVER BE USED!!
	/*GUIDescriptor::GUIDescriptor(void) : _window(NULL) {
	} // default constructor*/

	//________________________________________________________________________

	GUIDescriptor::GUIDescriptor(const string& layoutName) {
		this->_layoutName = layoutName;

		// Create the window on CEGUI
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(_layoutName+ ".layout" );
		_window = CEGUI::WindowManager::getSingleton().getWindow(_layoutName);
	} // custom constructor

	//________________________________________________________________________

	GUIDescriptor::~GUIDescriptor(void) {
	} // destructor

	//________________________________________________________________________

	CEGUI::Window* GUIDescriptor::activate() {
		// Activate the window that contains the layout especified in 
		// "_layoutName.layout"
		CEGUI::System::getSingletonPtr()->setGUISheet(_window);
		_window->setVisible(true);
		_window->activate();

		return _window;
	} // activate
};