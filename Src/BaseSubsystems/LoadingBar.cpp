#include "LoadingBar.h"
#include "Server.h"
#include "Graphics\Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include <OgreRenderWindow.h>

namespace BaseSubsystems{

	void LoadingBar::loadMapLoadingBar(){

		Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
		_window = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();
		_loadingBar = BaseSubsystems::CServer::getSingletonPtr()->getHikari()->createFlashOverlay("loadingbar",Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport(),
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport()->getWidth(), 
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getViewport()->getWidth(),
			Hikari::Center, 
			15);

		_loadingBar->load("LoadingMenu.swf");
		_loadingBar->show();
	}

	void LoadingBar::stopBar(){
		Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
		_loadingBar->hide();
	}
	void LoadingBar::loadinitLoadingBar(){
	}


// ResourceGroupListener callbacks
	void LoadingBar::resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::scriptParseEnded(const Ogre::String& scriptName, bool skipped)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::resourceGroupScriptingEnded(const Ogre::String& groupName)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::resourceLoadStarted(const Ogre::ResourcePtr& resource)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::resourceLoadEnded(void)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::worldGeometryStageStarted(const Ogre::String& description)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::worldGeometryStageEnded(void)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
	void LoadingBar::resourceGroupLoadEnded(const Ogre::String& groupName)
	{
		BaseSubsystems::CServer::getSingletonPtr()->getHikari()->update();
		_window->update();
	}
}