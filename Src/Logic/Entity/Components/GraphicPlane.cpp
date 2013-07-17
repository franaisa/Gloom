/**
@file GraphicPlane.cpp


@see Logic::CGraphicPlane
@see Logic::IComponent

*/

#include "GraphicPlane.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include <Ogre/OgreSceneManager.h>
#include <Ogre/OgreEntity.h>
#include <Ogre/OgreMeshManager.h>

namespace Logic {

	IMP_FACTORY(CGraphicPlane);

	CGraphicPlane::CGraphicPlane() {

	}
	
	//---------------------------------------------------------

	CGraphicPlane::~CGraphicPlane() {

	} // ~CGraphics
	
	//---------------------------------------------------------

	void CGraphicPlane::onStart() {
		Ogre::SceneManager* sceneMgr = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();

		// Construimos un mesh gráfico para el plano
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
													  plane, 1500, 1500, 20, 20, true, 1, 40, 40, Ogre::Vector3::UNIT_Z);

		// Le pedimos a Ogre que nos calcule las tangentes con mucho cariño si puede ser
		unsigned short src, dest;
		if (!planeMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest)) {
			planeMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		// Creamos el plano a partir del mesh generado
		Ogre::Entity* entGround = sceneMgr->createEntity(planeMesh);
		Ogre::SceneNode* groundNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
		groundNode->attachObject(entGround);
		groundNode->setPosition(0.0f, 0.0f, 0.0f);

		// Asignamos manualmente el material de pruebas
		entGround->setMaterialName("bumpLightingTest");
		entGround->setCastShadows(false);

		/*Ogre::Light* l = sceneMgr->createLight("TestLight");
		l->setPosition(0, 20, 0);
		l->setDirection(1, -1, 1);
		//l->setSpotlightInnerAngle( Ogre::Radian(0.31756043f) );
		//l->setSpotlightOuterAngle( Ogre::Radian(0.55481103f) );
		l->setDiffuseColour(0.0f, 0.0f, 0.9f);
		l->setAttenuation(70, 0.01, 0.01, 0.0001);*/
	} // spawn

} // namespace Logic

