#ifndef __Graphics_ObjectTextDisplay_H
#define __Graphics_ObjectTextDisplay_H

#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreImage.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreCamera.h>

class CObjectTextDisplay {
 
public:
    CObjectTextDisplay(const Ogre::MovableObject* p, const Ogre::Camera* c) {

		/*Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("HammerHead", "General");
		mFont->setType(Ogre::FT_TRUETYPE);
		mFont->setSource("HammerheadRegular");
		mFont->setTrueTypeSize(16);
		mFont->setTrueTypeResolution(96);*/

        m_p = p;
        m_c = c;
        m_enabled = false;
        m_text = "";
 
        // create an overlay that we can use for later
        m_pOverlay = Ogre::OverlayManager::getSingleton().create("shapeName");
        m_pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                  "Panel", "container1"));
 
        m_pOverlay->add2D(m_pContainer);
 
        m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "shapeNameText");
        m_pText->setDimensions(1.0, 1.0);
        m_pText->setMetricsMode(Ogre::GMM_PIXELS);
        m_pText->setPosition(0, 0);
 
        m_pText->setParameter("font_name", "bluehighway-8");
        m_pText->setParameter("char_height", "26");
        m_pText->setParameter("horz_align", "center");
        m_pText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));
 
        m_pContainer->addChild(m_pText);
        m_pOverlay->show();
    }
 
    virtual ~CObjectTextDisplay() { 
        // overlay cleanup -- Ogre would clean this up at app exit but if your app 
        // tends to create and delete these objects often it's a good idea to do it here.
 
        m_pOverlay->hide();
        Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
        m_pContainer->removeChild("shapeNameText");
        m_pOverlay->remove2D(m_pContainer);
        overlayManager->destroyOverlayElement(m_pText);
        overlayManager->destroyOverlayElement(m_pContainer);
        overlayManager->destroy(m_pOverlay);
    }
 
    void enable(bool enable) {
        m_enabled = enable;
        if (enable)
            m_pOverlay->show();
        else
            m_pOverlay->hide();
    }
 
    void setText(const Ogre::String& text) {
        m_text = text;
        m_pText->setCaption(m_text);
    }
 
    void update();
 
protected:
    const Ogre::MovableObject* m_p;
    const Ogre::Camera* m_c;
    bool m_enabled;
    Ogre::Overlay* m_pOverlay;
    Ogre::OverlayElement* m_pText;
    Ogre::OverlayContainer* m_pContainer;
    Ogre::String m_text;
};
 
void CObjectTextDisplay::update()  {
    if (!m_enabled)
        return;
 
    // get the projection of the object's AABB into screen space
    const Ogre::AxisAlignedBox& bbox = m_p->getWorldBoundingBox(true);
    Ogre::Matrix4 mat = m_c->getViewMatrix();
 
    const Ogre::Vector3* corners = bbox.getAllCorners();
 
    float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;
 
    // expand the screen-space bounding-box so that it completely encloses 
    // the object's AABB
    for (int i=0; i<8; i++) {
        Ogre::Vector3 corner = corners[i];
 
        // multiply the AABB corner vertex by the view matrix to 
        // get a camera-space vertex
        corner = mat * corner;
 
        // make 2D relative/normalized coords from the view-space vertex
        // by dividing out the Z (depth) factor -- this is an approximation
        float x = corner.x / corner.z + 0.5;
        float y = corner.y / corner.z + 0.5;
 
        if (x < min_x) 
            min_x = x;
 
        if (x > max_x) 
            max_x = x;
 
        if (y < min_y) 
            min_y = y;
 
        if (y > max_y) 
            max_y = y;
    }
 
    // we now have relative screen-space coords for the object's bounding box; here
    // we need to center the text above the BB on the top edge. The line that defines
    // this top edge is (min_x, min_y) to (max_x, min_y)
 
    //m_pContainer->setPosition(min_x, min_y);
    m_pContainer->setPosition(1-max_x, min_y);  // Edited by alberts: This code works for me
    m_pContainer->setDimensions(max_x - min_x, 0.1); // 0.1, just "because"
}

#endif