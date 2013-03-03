#ifndef __Graphics_ObjectTextDisplay_H
#define __Graphics_ObjectTextDisplay_H

#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreImage.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreCamera.h>

namespace Graphics {

// From the Ogre forums.  This class displays text above or next to on screen entities.
class CObjectTextDisplay {
 
public:
    static Ogre::Overlay* g_pOverlay;
 
    CObjectTextDisplay(const Ogre::MovableObject* p, const Ogre::Camera* c) {
        m_p = p;
        m_c = c;
        m_enabled = false;
        m_text = "";
 
        // create an overlay that we can use for later
		if(g_pOverlay == NULL)
		{
			g_pOverlay = Ogre::OverlayManager::getSingleton().create("floatingTextOverlay");
			g_pOverlay->show();
		}
 
		char buf[50];
		sprintf(buf, "c_%s", p->getName().c_str());
		m_elementName = buf;
        m_pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                  "Panel", "container1" +  p->getName()));
 
        g_pOverlay->add2D(m_pContainer);
 
		sprintf(buf, "ct_%s", p->getName().c_str());
		m_elementTextName = buf;
        m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", buf + p->getName());
		assert(m_pText != NULL);
        m_pText->setDimensions(1.0, 1.0);
        m_pText->setMetricsMode(Ogre::GMM_PIXELS);
        m_pText->setPosition(0, 0);
 
        m_pText->setParameter("font_name", "fuenteSimple");
        m_pText->setParameter("char_height", "16");
        m_pText->setParameter("horz_align", "center");
        m_pText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));
 
        m_pContainer->addChild(m_pText);
		m_pContainer->setEnabled(false);
    }
 
    virtual ~CObjectTextDisplay() {
 
        // overlay cleanup -- Ogre would clean this up at app exit but if your app 
        // tends to create and delete these objects often it's a good idea to do it here.
 
		Ogre::OverlayManager *overlayManager = Ogre::OverlayManager::getSingletonPtr();
		m_pContainer->removeChild(m_elementTextName);
		g_pOverlay->remove2D(m_pContainer);
		overlayManager->destroyOverlayElement(m_pText);
		overlayManager->destroyOverlayElement(m_pContainer);
    }
 
    void enable(bool enable) {
        m_enabled = enable;
        if (enable)
		{
            m_pContainer->show();
		}
        else
		{
            m_pContainer->hide();
		}
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
    Ogre::OverlayElement* m_pText;
    Ogre::String m_text;
	Ogre::String m_elementName;
	Ogre::String m_elementTextName;
    Ogre::OverlayContainer* m_pContainer;
};
 
Ogre::Overlay * CObjectTextDisplay::g_pOverlay = NULL;
 
void CObjectTextDisplay::update()  {
    if (!m_enabled)
        return;
 
    const Ogre::AxisAlignedBox& bbox = m_p->getWorldBoundingBox(true);
	Ogre::Matrix4 mat = m_c->getViewMatrix();
 
	bool behind = false;
 
	// We want to put the text point in the center of the top of the AABB Box.
	Ogre::Vector3 topcenter = bbox.getCenter();
	// Y is up.
	topcenter.y += bbox.getHalfSize().y;
	topcenter = mat * topcenter;
	// We are now in screen pixel coords and depth is +Z away from the viewer.
	behind = (topcenter.z > 0.0);
 
	if(behind)
	{
		// Don't show text for objects behind the camera.
		m_pContainer->setPosition(-1000, -1000);
	}
	else
	{
		// Not in pixel coordinates, in screen coordinates as described above.
		// We convert to screen relative coord by knowing the window size.
		// Top left screen corner is 0, 0 and the bottom right is 1,1
		// The 0.45's here offset alittle up and right for better "text above head" positioning.
		// The 2.2 and 1.7 compensate for some strangeness in Ogre projection?
		// Tested in wide screen and normal aspect ratio.
		m_pContainer->setPosition(0.45f - topcenter.x / (2.2f * topcenter.z), 
			topcenter.y / (1.7f * topcenter.z) + 0.45f);
		// Sizse is relative to screen size being 1.0 by 1.0 (not pixels size)
		m_pContainer->setDimensions(0.1f, 0.1f);
	}
}

};

#endif