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
		//NOTA: ACTUALMENTE SE PRECARGA LO QUE HAY EN PRUEBA PARA VER COMO YA NO DA TIRONES EL DISPARAR CON LA IRONHELL
		// EN UN FUTURO DEBERA IR EL NOMBRE COMMON A MACHETE, Y TENER NOSOTROS EN RESOURCES UNA SECCION COMMON

		//Declaracion de los recursos que son texturas png de forma selectiva
		Ogre::StringVectorPtr texturePngNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.png");
		for(Ogre::StringVector::iterator itName = texturePngNames->begin(); itName!=texturePngNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture","Prueba");
		}
		//Declaracion de los recursos que son texturas png de forma selectiva
		Ogre::StringVectorPtr textureJpgNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.jpg");
		for(Ogre::StringVector::iterator itName = textureJpgNames->begin(); itName!=textureJpgNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture","Prueba");
		}
		//Declaracion de los recursos que son mallas de forma selectiva
		Ogre::StringVectorPtr meshNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames("Prueba","*.mesh");
		for(Ogre::StringVector::iterator itName = meshNames->begin(); itName!=meshNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Mesh","Prueba");
		}
		//Inicializacion y precarga de las texturas y mallas previamente declaradas
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Prueba");
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Prueba");
		
	} // preloadCommon
	//--------------------------------------------------------

	void CPreloadResourceManager::preloadResources(const string &mapName)
	{	
		//El string section contendrá el nombre del mapa sin tener en cuenta SP o MP
		//Nota: Actualmente se toman como diferenciantes el . para SP y la _ para MP.
		string section;
		for (int i=0; i<mapName.length(); ++i){
			if(mapName[i]!='_' && mapName[i]!='.')
				section+=mapName[i];
			else
				break;
		}

		//Declaracion de los recursos que son texturas png de forma selectiva
		Ogre::StringVectorPtr texturePngNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames(section,"*.png");
		for(Ogre::StringVector::iterator itName = texturePngNames->begin(); itName!=texturePngNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture",section);
		}
		//Declaracion de los recursos que son texturas png de forma selectiva
		Ogre::StringVectorPtr textureJpgNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames(section,"*.jpg");
		for(Ogre::StringVector::iterator itName = textureJpgNames->begin(); itName!=textureJpgNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Texture",section);
		}
		//Declaracion de los recursos que son mallas de forma selectiva
		Ogre::StringVectorPtr meshNames = Ogre::ResourceGroupManager::getSingleton().findResourceNames(section,"*.mesh");
		for(Ogre::StringVector::iterator itName = meshNames->begin(); itName!=meshNames->end(); ++itName){
			Ogre::ResourceGroupManager::getSingleton().declareResource(*itName,"Mesh",section);
		}
		//Inicializacion y precarga de las texturas y mallas previamente declaradas
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(section);
		Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(section);
		


	} // preloadResources
	//---------------------------------------------------------


	void CPreloadResourceManager::unloadResources(const string &mapName)
	{	
		//El string section contendrá el nombre del mapa sin tener en cuenta SP o MP
		//Nota: Actualmente se toman como diferenciantes el . para SP y la _ para MP.
		string section;
		for (int i=0; i<mapName.length(); ++i){
			if(mapName[i]!='_' && mapName[i]!='.')
				section+=mapName[i];
			else
				break;
		}

		Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup(section);
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(section);

	} // unloadResources
	//---------------------------------------------------------

} // namespace Logic
