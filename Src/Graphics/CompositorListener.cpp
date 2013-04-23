//---------------------------------------------------------------------------
// CompositorListener.cpp
//---------------------------------------------------------------------------

/**
@file CompositorListener.cpp

Contiene la implementacion del compositor listener, este se usa para pasar parametros a los compositor (y los shader aplicados a estos)

@author Antonio Jesus Narvaez
@date Abril, 2013
*/

#include "CompositorListener.h"

#include <map>
#ifndef __Graphics_COMPOSITORLISTENER_H
#define __Graphics_COMPOSITORLISTENER_H

#include <OgreCompositorInstance.h>
#include <OgreGpuProgramParams.h>
#include <OgreCompositorManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>

#include "Graphics\Server.h"
#include "Graphics\Scene.h"
#include "Graphics\Camera.h"

namespace Graphics 
{
		CCompositorListener::CCompositorListener ()
		{
			_compositorManager = Ogre::CompositorManager::getSingletonPtr();
			_cameraViewport = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera()->getOgreCamera()->getViewport();
		}

		CCompositorListener::~CCompositorListener(){};
	
		void CCompositorListener::addCompositor(const std::string &nameCompositor, int id){
			_compositorManager->addCompositor(_cameraViewport,nameCompositor)->getCompositor();
			_nameCompositorAndId.insert(std::pair<std::string, int>(nameCompositor, id));
		}

		void CCompositorListener::deleteCompositor(const std::string &nameCompositor){

			_compositorManager->removeCompositor(_cameraViewport,nameCompositor);
			_nameCompositorAndId.erase(nameCompositor);
			_compositorsAndVariables.erase(_nameCompositorAndId[nameCompositor]);
		
		}
		
		bool CCompositorListener::inputCompositor(const std::string &nameCompositor, const std::string &nameVar, int value){
			
			if (_nameCompositorAndId.find(nameCompositor) == _nameCompositorAndId.end()){
				int id = _nameCompositorAndId[nameCompositor];
				_compositorsAndVariables.insert(std::pair<int, TkeyValue>(id, (TkeyValue(nameVar, value))));
				return true;
			}
			return false;
		}


		void CCompositorListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
		{

			if(_compositorsAndVariables.count(pass_id))
				necesaryOperations(pass_id, mat);
		}

		void CCompositorListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
		{
			if(_compositorsAndVariables.count(pass_id))
				necesaryOperations(pass_id, mat);
		}


		void CCompositorListener::necesaryOperations(int pass_id, Ogre::MaterialPtr &mat){
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			//const Ogre::String& progName = mat->getBestTechnique()->getPass(0)->getFragmentProgramName();
			std::pair<std::map<int, TkeyValue>::iterator,std::map<int, TkeyValue>::iterator> it= _compositorsAndVariables.equal_range(pass_id);

			for (std::map<int, TkeyValue>::iterator it2 = it.first; it2 != it.second; ++it2)
			{
				fpParams->setNamedConstant((*it2).second.first,(*it2).second.second);
			}
		}
}

#endif // __Graphics_CompositorListener_H