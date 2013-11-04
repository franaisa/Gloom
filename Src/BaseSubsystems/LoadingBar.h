#include <OgreResourceGroupManager.h>
#include <Hikari.h>

#ifndef __LoadingBar_H
#define __LoadingBar_H

namespace Ogre{
	class RenderWindow;
}

namespace BaseSubsystems{
	class LoadingBar : public Ogre::ResourceGroupListener{
	public:
	LoadingBar(){}
	virtual ~LoadingBar(){}

	void loadinitLoadingBar();
	void loadMapLoadingBar();
	void stopBar();
	// ResourceGroupListener callbacks
	virtual void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount);
	virtual void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript) ;
	virtual void scriptParseEnded(const Ogre::String& scriptName, bool skipped) ;
	virtual void resourceGroupScriptingEnded(const Ogre::String& groupName);
	virtual void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount) ;
	virtual void resourceLoadStarted(const Ogre::ResourcePtr& resource);
	virtual void resourceLoadEnded(void) ;
	virtual void worldGeometryStageStarted(const Ogre::String& description);
	virtual void worldGeometryStageEnded(void) ;
	virtual void resourceGroupLoadEnded(const Ogre::String& groupName);
	private:
		Hikari::FlashControl * _loadingBar;
		Ogre::RenderWindow* _window;
	};
}
#endif