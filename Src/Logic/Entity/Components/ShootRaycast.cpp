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
#include "Logic/Messages/MessageHudDispersion.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"

#include "Graphics/Camera.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

#include "Graphics/OgreDecal.h"

#include "Logic/Entity/Component.h"

#include "Graphics/Entity.h"
#include "Graphics.h"

namespace Logic {
	//IMP_FACTORY(CShootRaycast);

	CShootRaycast::~CShootRaycast() {
		// Nada que hacer
	}
	
	//__________________________________________________________________

	bool CShootRaycast::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShoot::spawn(entity,map,entityInfo)) return false;

		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();

		//Dispersión
		_dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
		_dispersionOriginal = _dispersion;

		_distance = entityInfo->getFloatAttribute(weapon+"Distance");

		if(entityInfo->hasAttribute("audioNoAmmo"))
			_noAmmo = entityInfo->getStringAttribute("audioNoAmmo");

		if(entityInfo->hasAttribute(weapon+"Audio"))
			_audioShoot = entityInfo->getStringAttribute(weapon+"Audio");

		return true;
	}

	//__________________________________________________________________

	// Disparo, usa el patrón template
	void CShootRaycast::primaryShoot() {
		if(_primaryCanShoot && _currentAmmo > 0){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;
				
			drawParticle("shootParticle");

			decrementAmmo();

			for(int i = 0; i < _numberShots; ++i) {
				CEntity* entityHit = fireWeapon();

				if(entityHit != NULL) {
					triggerHitMessages(entityHit);
				}
			}
			//Sonido de disparo
			emitSound(_audioShoot, "audioShoot");

			//Mensaje de dispersion en mira
			/*std::shared_ptr<CMessageHudDispersion> dispersionMsg = std::make_shared<CMessageHudDispersion>();
			_entity->emitMessage(dispersionMsg);*/
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			emitSound(_noAmmo, "noAmmo", true);
		}
	}// shoot
	//__________________________________________________________________

	void CShootRaycast::secondaryShoot() {

	} // secondaryShoot
	//__________________________________________________________________

	void CShootRaycast::secondaryShoot(int iRafagas) {

		decrementAmmo(iRafagas);

		//Creación de sweephit para 
		Physics::SphereGeometry sphere  = Physics::CGeometryFactory::getSingletonPtr()->createSphere(3.5);
		std::vector<Physics::CSweepHit> hits;
		//Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),_directionShoot,_screamerScreamMaxDistance,hitSpots, true);
		Vector3 vDirectionShoot = Math::getDirection(_entity->getOrientation());
		Physics::CServer::getSingletonPtr()->sweepMultiple(sphere, (_entity->getPosition() + Vector3(0,_heightShoot,0)),vDirectionShoot, _distance,hits, false, Physics::CollisionGroup::ePLAYER );	

		for(auto it = hits.begin(); it < hits.end(); ++it){
			std::string typeEntity = (*it).entity->getType();
			if((*it).entity->getName() != _entity->getName())
			{
				int danyoTotal = _damage * iRafagas;
				std::cout << "Le he dado!!! Danyo = " << danyoTotal << std::endl;

				std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
				m->setDamage(danyoTotal);
				m->setEnemy(_entity);
				(*it).entity->emitMessage(m);

				//Le he dado
				/*
				Vector3 direct = -(_directionShoot.reflect(-(*it).normal));
				auto m = std::make_shared<CMessageAddForcePlayer>();
				m->setForce(_directionShoot * (_screamerScreamForce*(1.0f- (*it).distance/_screamerScreamMaxDistance)));
				(*it).entity->emitMessage(m);

				auto m2 = std::make_shared<CMessageSetAnimation>();
				m2->setString("Damage");
				m2->setBool(false);
				(*it).entity->emitMessage(m2);*/
			}
		}
	} // secondaryShoot
	//__________________________________________________________________


	void CShootRaycast::stopSecondaryShoot() {

	} // secondaryShoot
	//__________________________________________________________________

	// Dispara rayos mediante raycast dependiendo de los parametros del arquetipo del arma
	CEntity* CShootRaycast::fireWeapon() {
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		std::cout << "Angulo: " << angle << std::endl;

		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, dispersionDirection);
			
		// Dibujamos el rayo en ogre para poder depurar
		drawRaycast(ray);

		//Comprobación de si da al mundo
		Physics::CRaycastHit hits2;
		bool disp = Physics::CServer::getSingletonPtr()->raycastSingle(ray, _distance,hits2, Physics::CollisionGroup::eWORLD);
		if (disp)
		{
			Vector3 pos = hits2.impact;
			std::cout << "-------He dado " << pos << std::endl;
			decals(hits2.entity, hits2.impact);

			// Añado aqui las particulas de dado en la pared.
			auto m = std::make_shared<CMessageCreateParticle>();
			m->setPosition(hits2.impact);
			m->setParticle("impactParticle");
			m->setDirectionWithForce(hits2.normal);
			hits2.entity->emitMessage(m);

		}

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _distance,hits, true,Physics::CollisionGroup::ePLAYER);


		//Devolvemos lo primero tocado que no seamos nosotros mismos
		CEntity* touched=NULL;
		for(int i=0;i<hits.size();++i)
			if(hits[i].entity!=_entity)
				touched=hits[i].entity;
		return touched;
		
	}// fireWeapon
	
	//__________________________________________________________________

	// Implementación por defecto de triggerHitMessages
	void CShootRaycast::triggerHitMessages(CEntity* entityHit) {
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		m->setDamage(_damage);
		m->setEnemy(_entity);
		entityHit->emitMessage(m);

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

	//__________________________________________________________________


} // namespace Logic

