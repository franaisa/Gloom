/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "Shoot.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "Graphics/Camera.h"



#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic 
{
	IMP_FACTORY(CShoot);
	
	//---------------------------------------------------------

	bool CShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("physic_radius")){
			_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		}
		
		if(entityInfo->hasAttribute("numWeapons")){
			int numWeapons = entityInfo->getIntAttribute("numWeapons");
			_weapons = new TWeapon[numWeapons];
			_timeSinceLastShoot = new float[numWeapons];

			for(int i = 0; i < numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << i+1;
				std::string weapon = aux.str();
												
				_weapons[i].name = entityInfo->getStringAttribute(weapon+"Name");
				_weapons[i].damage= entityInfo->getFloatAttribute(weapon+"Damage");
				_weapons[i].dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
				_weapons[i].distance = entityInfo->getFloatAttribute(weapon+"Distance");
				_weapons[i].coolDown = entityInfo->getFloatAttribute(weapon+"CoolDown");
			}
		}
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CShoot::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CONTROL;
	} // accept
	
	//---------------------------------------------------------

	void CShoot::process(CMessage *message)
	{
		switch(message->getMessageType())
		{
			case Message::CONTROL:
				if(!((CMessageControl*)message)->getType()==Control::LEFT_CLICK){
					shoot();
				}
				break;
		}

	} // process

	void CShoot::shoot(){
		


		
		Vector3 direction = Math::getDirection(_entity->getTransform());
		

		direction.normalise();

		//Vector3 origin = _entity->getPosition() + ((_capsuleRadius + 0.1f) * direction);
		Vector3 origin = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera()->getCameraPosition() 
			 + ((_capsuleRadius + 0.1f) * direction);

		Ray ray(origin, direction);


		////////////////////////////////////////////////////////////////////////////////////
			Graphics::CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
			Ogre::SceneManager *mSceneMgr = scene->getSceneMgr();


			std::stringstream aux;
			aux << "laser" << _temporal;
			++_temporal;
			std::string laser = aux.str();


			Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject(laser); 
			Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(laser+"_node"); 
 

			myManualObject->begin("laser", Ogre::RenderOperation::OT_LINE_STRIP); 
			Vector3 v = ray.getOrigin();
			myManualObject->position(v.x,v.y,v.z);


		for(int i=0; (i) < _weapons[_actualWeapon].distance;++i){
			printf("\n%d",i);
			
			Vector3 v = ray.getPoint(i);
			myManualObject->position(v.x,v.y,v.z);
			// etc 
		}

			myManualObject->end(); 
 
			myManualObjectNode->attachObject(myManualObject);

			//////////////////////////////////////////////////////////////////////////////////
		
		CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, _weapons[_actualWeapon].distance);
		
		if(entity)
		{
			printf("\nimpacto con %s", entity->getName().c_str());
			Logic::CMessageDamaged *m=new Logic::CMessageDamaged(Logic::Message::DAMAGED);
			m->setDamage(_weapons[_actualWeapon].damage);
			entity->emitMessage(m);
		}
	} // shoot

} // namespace Logic

