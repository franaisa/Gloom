/**
@file ShootRaycast.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootRaycast
@see Logic::IComponent

*/

#include "ShootRaycast.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/Shoot.h"
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"

#include "Graphics/Camera.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	//IMP_FACTORY(CShootRaycast);
	
	bool CShootRaycast::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShoot::spawn(entity,map,entityInfo)) return false;

		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();

		_dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
		_distance = entityInfo->getFloatAttribute(weapon+"Distance");

		if(entityInfo->hasAttribute("audioNoAmmo"))
			_noAmmo = entityInfo->getStringAttribute("audioNoAmmo");

		if(entityInfo->hasAttribute(weapon+"Audio"))
			_audioShoot = entityInfo->getStringAttribute(weapon+"Audio");

		return true;
	}

	//__________________________________________________________________

	// Disparo, usa el patrón template
	void CShootRaycast::shoot() {
		if(_canShoot && _currentAmmo > 0){
			_canShoot = false;
			_cooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			decrementAmmo();

			for(int i = 0; i < _numberShots; ++i) {
				std::pair<CEntity*, Ray> entityHit = fireWeapon();
				
				if(entityHit.first != NULL) {
					triggerHitMessages(entityHit);
				}
			}
			//Sonido de disparo
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();
			maudio->setRuta(_audioShoot);
			maudio->setId("audioShoot");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(_entity->isPlayer());
			_entity->emitMessage(maudio);
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			Logic::CMessageAudio *maudio=new Logic::CMessageAudio();
			maudio->setRuta(_noAmmo);
			maudio->setId("noAmmo");
			maudio->setPosition(_entity->getPosition());
			maudio->setNotIfPlay(true);
			maudio->setIsPlayer(_entity->isPlayer());
			_entity->emitMessage(maudio);
		}
	}// shoot
	
	//__________________________________________________________________

	// Dispara rayos mediante raycast dependiendo de los parametros del arquetipo del arma
	std::pair<CEntity*, Ray> CShootRaycast::fireWeapon() {
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0,_heightShoot,0);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, dispersionDirection);
			
		// Dibujamos el rayo en ogre para poder depurar
		drawRaycast(ray);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance,_entity->getEntityID());

		return std::pair<CEntity*, Ray>(entity, ray);
	}// fireWeapon
	
	//__________________________________________________________________

	// Implementación por defecto de triggerHitMessages
	void CShootRaycast::triggerHitMessages(std::pair<CEntity*, Ray> entityHit) {
		Logic::CMessageDamaged* m = new Logic::CMessageDamaged();
		m->setDamage(_damage);
		m->setEnemy(_entity);
		entityHit.first->emitMessage(m);



	}// triggerHitMessages

	//__________________________________________________________________

	// Dibujado de raycast para depurar
	void CShootRaycast::drawRaycast(const Ray& raycast) {
		Graphics::CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
		Ogre::SceneManager *mSceneMgr = scene->getSceneMgr();

		std::stringstream aux;
		aux << "laser" << _nameWeapon << _temporal;
		++_temporal;
		std::string laser = aux.str();

		Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject(laser); 
		Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(laser+"_node"); 
 
		myManualObject->begin("laser", Ogre::RenderOperation::OT_LINE_STRIP);
		Vector3 v = raycast.getOrigin();
		myManualObject->position(v.x,v.y,v.z);

		for(int i=0; i < _distance;++i){
			Vector3 v = raycast.getPoint(i);
			myManualObject->position(v.x,v.y,v.z);
			// etc 
		}

		myManualObject->end(); 
		myManualObjectNode->attachObject(myManualObject);
	}// drawRaycast

} // namespace Logic

