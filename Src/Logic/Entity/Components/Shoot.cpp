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
				_weapons[i].damage= (unsigned char) entityInfo->getIntAttribute(weapon+"Damage");
				_weapons[i].dispersion = (unsigned char) entityInfo->getIntAttribute(weapon+"Dispersion");
				_weapons[i].distance = entityInfo->getFloatAttribute(weapon+"Distance");
				_weapons[i].coolDown = (unsigned char) entityInfo->getIntAttribute(weapon+"CoolDown");
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
				if(((CMessageControl*)message)->getType()==Control::LEFT_CLICK){
					shoot();
				}
				break;
		}

	} // process
	//---------------------------------------------------------


	void CShoot::shoot(){
		
		//Generación del rayo habiendo obtenido antes el origen y la dirección
		Graphics::CCamera* camera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		Vector3 direction = camera->getTargetCameraPosition() - camera->getCameraPosition();
		direction.normalise();
		//El origen debe ser mínimo la capsula y por si miramos al suelo la separación mínima debe ser 1.5f ( en un futuro es probable que sea recomendable cambiar por no chocar con el grupo de uno mismo )
		Vector3 origin = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera()->getCameraPosition() 
			 + ((_capsuleRadius + 1.5f) * direction);

		Ray ray(origin, direction);

		//Dibujo del rayo
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
			Vector3 v = ray.getPoint(i);
			myManualObject->position(v.x,v.y,v.z);
			// etc 
		}

			myManualObject->end(); 
			myManualObjectNode->attachObject(myManualObject);


		//Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, _weapons[_actualWeapon].distance);
		
		//Si hay colisión envíamos a dicha entidad un mensaje de daño
		if(entity)
		{
			printf("\nimpacto con %s", entity->getName().c_str());
			Logic::CMessageDamaged *m=new Logic::CMessageDamaged(Logic::Message::DAMAGED);
			m->setDamage(_weapons[_actualWeapon].damage);
			entity->emitMessage(m);
		}
	} // shoot

} // namespace Logic

