//---------------------------------------------------------------------------
// PreloadResourceManager.cpp
//---------------------------------------------------------------------------

/**
@file PreloadResourceManager.cpp

Contiene la implementación del gestor de precarga de recursos de cada nivel.

@see Logic::CPreloadResourceManager

@author Jose Antonio García Yáñez
@date Julio, 2013
*/

#include "PreloadResourceManager.h"

#include <OgreMeshManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>

#include <cassert>

using namespace std;

namespace Logic {

	CPreloadResourceManager* CPreloadResourceManager::_instance = 0;

	//--------------------------------------------------------

	CPreloadResourceManager::CPreloadResourceManager()
	{
		_instance = this;

	} // CServer
	//--------------------------------------------------------

	CPreloadResourceManager::~CPreloadResourceManager()
	{
		_instance = 0;

	} // ~CServer
	//--------------------------------------------------------

	bool CPreloadResourceManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CPreloadResourceManager no permitida!");

		new CPreloadResourceManager();

		//Declaracion, inicializacion y precarga de los recursos comunes
		_instance->preloadCommon();

		return true;

	} // Init
	//--------------------------------------------------------

	void CPreloadResourceManager::Release()
	{
		assert(_instance && "Logic::CPreloadResourceManager no está inicializado!");

		if(_instance)
			delete _instance;

	} // Release
	//--------------------------------------------------------

	void CPreloadResourceManager::preloadCommon() 
	{
		//Declaracion de los recursos que son texturas png de forma selectiva (especificando grupo)
		Ogre::StringVectorPtr texturePngNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.png");
		for(Ogre::StringVector::iterator itName = texturePngNames->begin(); itName!=texturePngNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture","Prueba");
		}
		//Declaracion de los recursos que son texturas png de forma selectiva (especificando grupo)
		Ogre::StringVectorPtr textureJpgNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.jpg");
		for(Ogre::StringVector::iterator itName = textureJpgNames->begin(); itName!=textureJpgNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture","Prueba");
		}
		//Declaracion de los recursos que son mallas de forma selectiva 
		Ogre::StringVectorPtr meshNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.mesh");
		for(Ogre::StringVector::iterator itName = meshNames->begin(); itName!=meshNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Mesh","Prueba");
		}
		//Inicializacion y precarga de las texturas y mallas previamente declaradas (especificando el grupo "Prueba")
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Prueba");
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Prueba");
		
	} // preloadCommon
	//--------------------------------------------------------

	void CPreloadResourceManager::preloadResources(const string &mapName)
	{	
		string section;
		
	} // preloadResources
	//---------------------------------------------------------


	void CPreloadResourceManager::unloadResources(const string &mapName)
	{	
		string section;

	} // unloadResources
	//---------------------------------------------------------

} // namespace Logic
