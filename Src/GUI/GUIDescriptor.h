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

typedef std::map<std::string, bool (*)(const CEGUI::EventArgs&)> 
	ButtonFunctionTable;

// Namespace for anything related with the GUI
namespace GUI {
	
	/**
	 * This class defines a GUI descriptor.
	 *
	 * The purpose of this class is to store any relevant information needed to 
	 * render a GUI window. The point really is to abstract the details of the
	 * GUI engine we are using to render the window.
	 *
	 * Details like pair Button-Function, layout names and window pointers should
	 * be stored here.
	 *
	 * GUI::Server keeps descriptors to all the layouts that have been
	 * registered.
	 *
	 * Please note that in this early version copy constructor, operator= and 
	 * operator== have NOT BEEN IMPLEMENTED due to the lack of neccesity.
	 *
	 * @author Francisco Aisa García
	 * @version 0.1
	*/
	class GUIDescriptor {
	public:


		// =======================================================================
        //                     CONSTRUCTORS AND DESTRUCTORS
        // =======================================================================


		/**
		 * Default constructor
		 *
		 * Default class constructor has been omitted for the sake of safety. We 
		 * should NEVER create a descriptor without any parameters, because that 
		 * would mean that is not usable and it could create havoc if we try to use 
		 * it. Also, it doesn't make much sense, and in case we did it, we should 
		 * take good note of it when deallocating memory in GUI::CServer's  
		 * destructor.
		 */
		//GUIDescriptor();

		//________________________________________________________________________

		/**
		 * Custom constructor
		 *
		 * It creates a new descriptor for a new layout. It also takes care of
		 * window initialization.
		 * @param layoutName Layout Name for this GUI descriptor.
		 */
		GUIDescriptor(const std::string& layoutName);

		//________________________________________________________________________

		/** Destructor. */
		~GUIDescriptor(void);


		// =======================================================================
        //                              MODIFIERS
        // =======================================================================


		/**
		 * Activate the GUI window assigned to this descriptor.
		 *
		 * @return A pointer to the CEGUI window that is made active.
		 */
		CEGUI::Window* activate();

		//________________________________________________________________________

		/**
		 * Add a pair button function to the GUI descriptor.
		 *
		 * Buttons are defined in external files. Once they are used they need to
		 * fire some kind of functionality; that's why we need a pointer to a 
		 * function, so we can tell CEGUI which function to fire when the button
		 * is clicked.
		 *
		 * The look of the button is defined in the aforementioned external file,
		 * kind of like what we would do in CSS.
		 *
		 * @param layoutName Name of the layout to which this button is going to
		 * be assigned
		 * @param buttonFunction Function that is going to be fired when the button
		 * is pushed.
		 */
		void addButton( const std::string& layoutName, 
						bool (*buttonFunction)(const GUIEventArgs&) );

	private:
		// =======================================================================
        //                            PRIVATE FIELDS
        // =======================================================================

		/** Pointer to the CEGUI window that is going to be used */
		CEGUI::Window* _window;

		/** Layout name of GUI that this descriptor points to */
		std::string _layoutName;

		/** Map table that hold pairs < button name - button function > */
		ButtonFunctionTable _buttonFunctionTable;
	};

};

#endif
