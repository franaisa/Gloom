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
 * Copyright © 2011-2012 Francisco Aisa Garcia
 */

#ifndef __GUI_GUIDESCRIPTOR_H__
#define __GUI_GUIDESCRIPTOR_H__

//******************************************************************************
//                                 INCLUDE
//******************************************************************************

#include <string>
#include <map>

//******************************************************************************
//                           FORWARD DECLARATIONS
//******************************************************************************

namespace CEGUI {
	class Window;
	class EventArgs;
};

namespace GUI {
	class GUIEventArgs;
};

typedef std::map<std::string, bool (*)(const CEGUI::EventArgs&)> ButtonFunctionTable;

// Namespace for anything related with the GUI
namespace GUI {
	
	/**
	 * This class defines a GUI descriptor.
	 *
	 * The purpose of this class is to store any relevant information needed to 
	 * render a GUI window. The point really is to abstract the details of the
	 * GUI engine we are using to render the window.
	 *
	 * GUI::Server keeps descriptors to all the layouts that have been
	 * registered.
	 *
	 * @author Francisco Aisa García
	 * @version 0.1
	*/
	class GUIDescriptor {
	public:
		GUIDescriptor();
		GUIDescriptor(const std::string& layoutName);
		~GUIDescriptor(void);

		CEGUI::Window* activate();
		void addButton( const std::string& layoutName, bool (*buttonFunction)(const GUIEventArgs&) );

	private:
		CEGUI::Window* _window;
		std::string _layoutName;
		// No es necesario una lista de funciones wrapper ya que al hacer un constructor de conversion
		// podemos llamar directamente a la funcion subscriptevent con nuestros parametros y que se
		// casteen automaticamente a los de CEGUI
		ButtonFunctionTable _buttonFunctionTable;
	};

};

#endif
