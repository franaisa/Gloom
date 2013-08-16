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
	
	//________________________________________________________________________

	CGraphicPlane::~CGraphicPlane() {

	} // ~CGraphics
	
	//________________________________________________________________________

	void CGraphicPlane::onStart() {
		Ogre::SceneManager* sceneMgr = Graphics::CServer::getSingletonPtr()->getActiveScene()->getSceneMgr();

		// Construimos un mesh gráfico para el plano
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
													  plane, 1500, 1500, 20, 20, true, 1, 15, 15, Ogre::Vector3::UNIT_Z);

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
		entGround->setLightMask(1 << 0 | 1 << 2);

		// Asignamos manualmente el material de pruebas
		entGround->setMaterialName("bumpLightingTest");
		entGround->setCastShadows(false);
	} // spawn

} // namespace Logic

