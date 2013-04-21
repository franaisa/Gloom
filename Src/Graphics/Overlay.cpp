//---------------------------------------------------------------------------
// Overlay.cpp
//---------------------------------------------------------------------------

/**
@file Overlay.cpp

Contiene la implementación de la clase que representa un Overlay.

@see Graphics::COverlay

@author Antonio Jesus Narvaez
@date Febrero, 2013
*/

#include "Overlay.h"
#include "Scene.h"
#include "Entity.h"

#include "Particle.h"

#include "Graphics/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreOverlay.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreEntity.h>

namespace Graphics 
{
	COverlay::COverlay(Ogre::Overlay* overlay){
		_overlay = overlay;
	}

	COverlay::COverlay(Ogre::OverlayElement* overlay){
		if(std::string::npos != overlay->getName().find("Panel")){
			_overlayContainer = static_cast<Ogre::OverlayContainer*>(overlay);
		}else{
			_overlayText = static_cast<Ogre::TextAreaOverlayElement*>(overlay);
		}
	} // COverlay

	COverlay::COverlay(const std::string &name, CScene* scene, const std::string &type){

		_overlay = NULL;
		_overlayContainer = NULL;
		_overlayText = NULL;
		_scene = scene;

		if(type.empty()){
			if(0 == (_overlay = Graphics::CServer::getSingletonPtr()->getOverlayManager()->getByName(name)))
				_overlay = Graphics::CServer::getSingletonPtr()->getOverlayManager()->create(name);
		}else{
			if(type == "Panel"){
				if(!Graphics::CServer::getSingletonPtr()->getOverlayManager()->hasOverlayElement(name)){
					_overlayContainer = static_cast<Ogre::OverlayContainer*> (Graphics::CServer::getSingletonPtr()->getOverlayManager()->createOverlayElement(type, name));
				}else{
					_overlayContainer = static_cast<Ogre::OverlayContainer*> (Graphics::CServer::getSingletonPtr()->getOverlayManager()->getOverlayElement(name));
				}
			}
			if(type == "TextArea"){
				if(!Graphics::CServer::getSingletonPtr()->getOverlayManager()->hasOverlayElement(name)){
					_overlayText = static_cast<Ogre::TextAreaOverlayElement*> (Graphics::CServer::getSingletonPtr()->getOverlayManager()->createOverlayElement(type, name));
				}else{
					_overlayText = static_cast<Ogre::TextAreaOverlayElement*> (Graphics::CServer::getSingletonPtr()->getOverlayManager()->getOverlayElement(name));
				}
			}
		}
		_type = type;
	} // COverlay

	//------------------------------------------------------------

	COverlay::~COverlay(){
		/*
		if(_overlay){Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroy(_overlay->getName());}
		if(_overlayContainer){Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyOverlayElement(_overlayContainer->getName());}
		if(_overlayText){Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyOverlayElement(_overlayContainer->getName());}
		*/
		//std::cout << "destruyendo overlay" << std::endl;
		
		/*
		Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyAllOverlayElements();
		Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyAll();
		*/
		if(_overlay){ 
			Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroy(_overlay);
		}
		
		if(_overlayContainer){ 
			Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyOverlayElement(_overlayContainer);
		}
		if(_overlayText){
			Graphics::CServer::getSingletonPtr()->getOverlayManager()->destroyOverlayElement(_overlayText);
		}

		_overlay = NULL;
		_overlayContainer = NULL;
		_overlayText = NULL;

	} // ~COverlay

	//------------------------------------------------------------

	void COverlay::setVisible(bool visible){
		if(visible){
			if(_overlay){ _overlay->show();}
			if(_overlayContainer){ _overlayContainer->show();}
			if(_overlayText){ _overlayText->show();}
		}else{
			if(_overlay){ _overlay->hide();}
			if(_overlayContainer){ _overlayContainer->hide();}
			if(_overlayText){ _overlayText->hide();}
		}
	} // setVisible

	//------------------------------------------------------------

	bool COverlay::isVisible(){

		if(_overlay){return _overlay->isVisible();}
		if(_overlayContainer){return _overlayContainer->isVisible();}
		if(_overlayText){return _overlayText->isVisible();}
		return false;
	} // isVisible

	//------------------------------------------------------------

	void COverlay::add2D(COverlay* overlayContainer){
		if(_overlay){ _overlay->add2D(overlayContainer->getOverlayContainer()); 	
		}
	} // add2D

	//------------------------------------------------------------

	
	CEntity* COverlay::add3D(const std::string &name, const std::string &mesh, const Vector3 *position){
		
		if(_overlay){

			//CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
			
			int counter=0;
			char num[5];
			sprintf(num, "%d", counter++);
			
			std::string nameSceneNode = "SceneNode_"+name;// + num;
			std::string nameEntity =  "hud3D_"+name;

			std::vector<CEntity *>::const_iterator it = _entities3D.begin() ;
			bool findIt = false;
			while(it != _entities3D.end() && !findIt){
				if((*it)->getName() == nameEntity){
					findIt = true;
				}else{
					++it;
				}
			}
			if(findIt)
			{
				//fran, esta te la dedico :D
				return (*it);
			}

			
			//scene->getSceneMgr()->getRootSceneNode()->addChild(sceneNode);
			Ogre::SceneNode* sceneNode;
			sceneNode = new Ogre::SceneNode(_scene->getSceneMgr(), nameSceneNode);
				
			Ogre::Entity *entity;
			entity = _scene->getSceneMgr()->createEntity(nameEntity, mesh);

			
			Ogre::MaterialPtr aux= Ogre::MaterialManager::getSingleton().getByName(name);
			//Ogre::MaterialPtr material = static_cast<Ogre::Material *>(Ogre::MaterialManager::getSingleton().getByName(name).get())->clone(name+"_3D");
			
			if(!aux.isNull()){
				Ogre::MaterialPtr material = aux.get()->clone(nameEntity);

				material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
				material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
				material->getTechnique(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			
				entity->setMaterial(material);
			}
			
			//scene->getSceneMgr()->getRootSceneNode()->addChild(sceneNode);

			sceneNode->attachObject((Ogre::MovableObject *)entity);
			
			_overlay->add3D(sceneNode);

			sceneNode->setPosition(*position);

			// Esto es una pequeña ñapa, me creo un entidad grafica pero sin inicializar, y le añado una escena ahierro
			// Hago esto para que se pueda desplazar desde la logica sin ningun problema.
			Graphics::CEntity *entityTemp = new CEntity(nameEntity, mesh);
			entityTemp->setOgreEntity(entity);
			entityTemp->setSceneNode(sceneNode);
			entityTemp->setScene(_scene);

			_entities3D.push_back(entityTemp);

			return entityTemp;
		}
		return 0;

	} // add3D

	//------------------------------------------------------------

	void COverlay::add3D(Graphics::CParticle *particle, const Vector3 *position){
			
		
		if(_overlay){
			_overlay->add3D(particle->getSceneNode());

			// Esto es una pequeña ñapa, me creo un entidad grafica pero sin inicializar, y le añado una escena ahierro
			// Hago esto para que se pueda desplazar desde la logica sin ningun problema.
		}

	} // add3D

	//------------------------------------------------------------

	void COverlay::setPosition(float left, float top){
		if(_overlayContainer){ _overlayContainer->setPosition(left, top);}
		if(_overlayText){ _overlayText->setPosition(left, top);}
	} // setPosition

	//------------------------------------------------------------

	
	void COverlay::setDimensions(float width, float height){
		if(_overlayContainer){ _overlayContainer->setDimensions(width, height);}
		if(_overlayText){ _overlayText->setDimensions(width, height);}
	} // setDimensions

	//------------------------------------------------------------

	
	void COverlay::setMetricsMode(std::string unit){
		Ogre::GuiMetricsMode metricsMode = Ogre::GMM_RELATIVE;
		if(unit == "pixel")
			metricsMode = Ogre::GMM_PIXELS;

		if(_overlayContainer){ _overlayContainer->setMetricsMode(metricsMode);}
		if(_overlayText){ _overlayText->setMetricsMode(metricsMode);}
	} // setMetricsMode

	//------------------------------------------------------------

	
	void COverlay::setMaterial(const std::string& material){
		if(_overlayContainer){ _overlayContainer->setMaterialName(material);}
		if(_overlayText){ _overlayText->setMaterialName(material);}
	} // setMaterial

	//------------------------------------------------------------

	
	void COverlay::setText(const std::string& text){
		if(_overlayContainer){ _overlayContainer->setCaption(text);}
		if(_overlayText){ _overlayText->setCaption(text);}
	} // setText

	//------------------------------------------------------------

	void COverlay::setZBuffer(const int zBuffer){
		if(_overlay){ _overlay->setZOrder(zBuffer);}
	} // setText

	//------------------------------------------------------------


	void COverlay::setTextSize(const int textSize){
		if(_overlayText){ _overlayText->setCharHeight((Ogre::Real)textSize);}
	} // setTextSize

	//------------------------------------------------------------


	void COverlay::setFont(const std::string& font){
		if(_overlayText){ _overlayText->setFontName(font);}
	} // setFont

	//------------------------------------------------------------


	void COverlay::addChild(COverlay* child){
		if(_overlayContainer){ _overlayContainer->addChild(child->getOverlayText());}
	} // addChild

	//------------------------------------------------------------


} // namespace Graphics
