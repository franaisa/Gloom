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
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"

#include "Graphics/Camera.h"

#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Messages/MessageChangeWeapon.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageControl.h"

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
			_numWeapons = entityInfo->getIntAttribute("numWeapons");
			_weapons = new TWeapon[_numWeapons];
			_timeSinceLastShoot = new float[_numWeapons];

			for(int i = 0; i < _numWeapons; ++i){
				
				std::stringstream aux;
				aux << "weapon" << i+1;
				std::string weapon = aux.str();
												
				_weapons[i].name = entityInfo->getStringAttribute(weapon+"Name");
				_weapons[i].damage= (unsigned char) entityInfo->getIntAttribute(weapon+"Damage");
				_weapons[i].dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
				_weapons[i].distance = entityInfo->getFloatAttribute(weapon+"Distance");
				_weapons[i].numberShoots = (unsigned char) entityInfo->getIntAttribute(weapon+"NumberShoots");
				_weapons[i].coolDown = (unsigned char) entityInfo->getIntAttribute(weapon+"CoolDown");
				if(entityInfo->hasAttribute(weapon+"ammo")){
					_weapons[i].ammo = entityInfo->getIntAttribute(weapon+"ammo");
				}else{
					_weapons[i].ammo = -1;
				}
			}
		}
		return true;

	} // spawn
	//---------------------------------------------------------


	bool CShoot::accept(CMessage *message)
	{
		return message->getMessageType() == Message::CONTROL
			|| message->getMessageType() == Message::CHANGE_WEAPON;
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
			case Message::CHANGE_WEAPON:
					changeWeapon( ((CMessageChangeWeapon*)message)->getWeapon() );
			break;
		}

	} // process
	//---------------------------------------------------------

	void CShoot::changeWeapon(unsigned char newWeapon){
		
		
		if(newWeapon >= _numWeapons){
			return;
		}

		if(newWeapon != _actualWeapon && _weapons[newWeapon].ammo != -1)
		{
			_actualWeapon = newWeapon;
			Logic::CMessageChangeWeaponGraphics *m=new Logic::CMessageChangeWeaponGraphics(Logic::Message::CHANGE_WEAPON_GRAPHICS);
			m->setWeapon(_actualWeapon);
			_entity->emitMessage(m);
			
		}
		
	}

	void CShoot::shoot(){
		
		//Generación del rayo habiendo obtenido antes el origen y la dirección
		Graphics::CCamera* camera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		

		// Para generalizar las armas, todas tendras tantas balas como la variable numberShoots
		for(int i = 0; i < _weapons[_actualWeapon].numberShoots; ++i)
		{

			// Se corrige la posicion de la camara
			Vector3 direction = camera->getTargetCameraPosition() - camera->getCameraPosition();
			direction.normalise();
			//El origen debe ser mínimo la capsula y por si miramos al suelo la separación mínima debe ser 1.5f ( en un futuro es probable que sea recomendable cambiar por no chocar con el grupo de uno mismo )
			Vector3 origin = camera->getCameraPosition() + ((_capsuleRadius + 1.5f) * direction);


			//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
			Ogre::Radian angle = Ogre::Radian(_weapons[_actualWeapon].dispersion/100);

			//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
			Vector3 dispersionDirection = direction.randomDeviant(angle);
			dispersionDirection.normalise();

			// Creamos el ray desde el origen en la direccion del raton (desvido ya aplicado)
			Ray ray(origin, dispersionDirection);
			


			////////////////////////////////////////////////Dibujo del rayo


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


			for(int i=0; i < _weapons[_actualWeapon].distance;++i){
				Vector3 v = ray.getPoint(i);
				myManualObject->position(v.x,v.y,v.z);
				// etc 
			}

				myManualObject->end(); 
				myManualObjectNode->attachObject(myManualObject);


			//////////////////////////////////fin del dibujado del rayo

			//Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
			CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosest(ray, _weapons[_actualWeapon].distance);
		
			//Si hay colisión envíamos a dicha entidad un mensaje de daño
			if(entity)
			{
				printf("\nimpacto con %s", entity->getName().c_str());

				// LLamar al componente que corresponda con el daño hecho
				//entity->

				Logic::CMessageDamaged *m=new Logic::CMessageDamaged(Logic::Message::DAMAGED);
				m->setDamage(_weapons[_actualWeapon].damage);
				entity->emitMessage(m);
			}
		
		
		}// fin del bucle para multiples disparos
	} // shoot

} // namespace Logic

