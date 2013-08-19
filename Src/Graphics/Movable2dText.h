//---------------------------------------------------------------------------
// Movable2dText.h
//---------------------------------------------------------------------------

/**
@file Movable2dText.h


@see Graphics::CMovable2dText

@author Francisco Aisa García
@date Agosto, 2013
*/

#ifndef __Graphics_Movable2dText_H
#define __Graphics_Movable2dText_H

#include "BaseSubsystems/Math.h"
#include "Billboard.h"

#include <OgreString.h>
#include <OgreTexture.h>
#include <OgreImage.h>
#include <OgreFont.h>
#include <OgreColourValue.h>

// Predeclaración de clases para ahorrar tiempo de compilación

namespace Ogre {
	class BillboardSet;
	class Billboard;
}

namespace Graphics {
	class CScene;
	class CEntity;
}

namespace Graphics {

	/**
	
	*/

	class CMovable2dText : private CBillboard {
	public:
		CMovable2dText();
		~CMovable2dText();

		void load(CScene* scene, CEntity* parent, const Vector3& position, const std::string& text, const std::string& textFont, const Vector4& color);
		void setVisible(bool isVisible);
		bool isVisible();

	protected:

		void writeToTexture(const Ogre::String &str, Ogre::TexturePtr destTexture, Ogre::Image::Box destRectangle, Ogre::FontPtr font, 
							const Ogre::ColourValue &color, char justify = 'c',  bool wordwrap = true);

	};

}

#endif