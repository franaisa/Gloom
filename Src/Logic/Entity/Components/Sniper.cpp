/**
@file Sniper.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CSniper
@see Logic::CShootRaycast

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#include "Sniper.h"
#include "ScreamerShieldDamageNotifier.h"

#include "Physics/Server.h"
#include "Physics/RaycastHit.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Map/MapEntity.h"
#include "HudWeapons.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageCreateParticle.h"

#include <OgreManualObject.h>

namespace Logic {

	IMP_FACTORY(CSniper);
	
	CSniper::~CSniper() {
		// Nada que hacer
	}//~CSniper
	//-------------------------------------------------------

	bool CSniper::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IWeapon::spawn(entity,map,entityInfo)) return false;

		if(entityInfo->hasAttribute(_weaponName+"MaxExpansiveDistance"))
			_maxExpansiveDistance = entityInfo->getFloatAttribute(_weaponName+"MaxExpansiveDistance");

		if(entityInfo->hasAttribute(_weaponName+"AmmoSpentPerSecondaryShot"))
			_secondaryConsumeAmmo = entityInfo->getIntAttribute(_weaponName+"AmmoSpentPerSecondaryShot");
		
		if( entityInfo->hasAttribute(_weaponName + "ShotsDistance") )
			_shotsDistance = entityInfo->getFloatAttribute(_weaponName + "ShotsDistance");

		_defaultPrimaryFireCooldown = _primaryFireCooldown = entityInfo->getFloatAttribute(_weaponName+"PrimaryFireCooldown") * 1000;

		_defaultPrimaryFireDamage = _primaryFireDamage = entityInfo->getFloatAttribute(_weaponName + "PrimaryFireDamage");

		_defaultSecondaryFireCooldown = _secondaryFireCooldown = entityInfo->getFloatAttribute(_weaponName+"SecondaryFireCooldown") * 1000;

		_defaultSecondaryFireDamage = _secondaryFireDamage = entityInfo->getFloatAttribute(_weaponName + "SecondaryFireDamage");

		_burnedIncrementPercentageDamage = entityInfo->getFloatAttribute(_weaponName + "BurnedIncrementPercentageDamage") / 100;

		return true;
	} // spawn
	//__________________________________________________________________

	void CSniper::onActivate() {

		//Reiniciamos el cooldown
		reduceCooldown(0);
		//y los daños
		amplifyDamage(0);
	}

	//__________________________________________________________________

	void CSniper::primaryFire(){

		//Direccion
		Vector3 direction = _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;
	
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, direction);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _shotsDistance, hits,true, Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eWORLD | Physics::CollisionGroup::eFIREBALL |
																							 Physics::CollisionGroup::eSCREAMER_SHIELD);

		decrementAmmo();

		//Aplicamos daño si no somos nosotros mismos(se podria modificar la fisica para que no nos devuelva a nosotros)
		//Y ademas no hemos tocado ya pared
		for(int i=0;i<hits.size();++i){
			//Si tocamos el mundo no continuamos viendo hits y llamamos al pintado del rayo (si se considera necesario)
			if(hits[i].entity->getType().compare("World")==0){
				//Si hacemos el rayo extendiendo un mesh sacar a un metodo de pintado
				/*float distanceWorld=hits[i].distance;
				//Atendiendo a la distancia y sabiendo que el gráfico de la entidad mide X metros
				//La escala que debera tener es
				float expansion=distanceWorld/10;
				Vector3 newScale(expansion,1,1);
				//La posicion por tanto sera	
				Vector3 newPosition=_entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f) + ( Math::getDirection( _entity->getOrientation() ) 
					* (_capsuleRadius + 0.2f +10.0f));//10.0f es el changeScale, no sale bien pk el mesh no tiene la z con 1Unidad
				//Cogemos un laser y lo seteamos (pull de laseres?)
				CEntity *laser=Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("LaserBeam1");
				laser->setPosition(newPosition);
				laser->setOrientation(_entity->getOrientation());
				CGraphics *cGraphics=laser->getComponent<CGraphics>("CGraphics");
				cGraphics->changeScale(Vector3(0.2,0.2,10));//Alargamos el cilindro*/

				//Antes de salir desactivamos el quemado para el siguiente disparo
				_burned=false;

				return;
			}
			//Si tocamos una bola de fuego, activamos el quemado
			else if(hits[i].entity->getType().compare("FireBall")==0){
				_burned=true;
			}
			else if(hits[i].entity->getType() == "ScreamerShield") {
				// Tras mandar los mensajes de daño al escudo del screamer
				// abortamos la ejecución del bucle ya que el escudo evita
				// que el rayo de la sniper llegue más lejos
				CEntity* screamerShieldOwner = hits[i].entity->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier")->getOwner();
				
				// Si no se trata de nuestro propio escudo mandamos los
				// mensajes de daño
				if(screamerShieldOwner != _entity) {
					if(_burned)
						triggerHitMessages(hits[i].entity, _primaryFireDamage + _primaryFireDamage * _burnedIncrementPercentageDamage);
					else
						triggerHitMessages(hits[i].entity, _primaryFireDamage);
				}

				break;
			}
			//Sino mientras que no seamos nosotros mismos
			else if(hits[i].entity!=_entity){
				if(_burned)
					triggerHitMessages(hits[i].entity, _primaryFireDamage + _primaryFireDamage * _burnedIncrementPercentageDamage);
				else
					triggerHitMessages(hits[i].entity, _primaryFireDamage);

				
			}
		}
	}//primaryFireWeapon
	//-------------------------------------------------------

	void CSniper::secondaryFire(){

		//Direccion
		Vector3 direction = _entity->getOrientation()*Vector3::NEGATIVE_UNIT_Z;

		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, direction);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _shotsDistance, hits, true, Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eWORLD | Physics::CollisionGroup::eFIREBALL | Physics::CollisionGroup::eSCREAMER_SHIELD);

		//Cogemos lo primero tocado que no seamos nosotros mismos y vemos si a un rango X hay enemigos (no nosotros)
		//Ojo en cooperativo tendremos que hacer distincion entre otros players aliados
		CEntity* entityHit=NULL;
		for(int i=0;i<hits.size();++i){
			//Si tocamos mundo terminamos
			if(hits[i].entity->getType().compare("World")==0)
				break;
			//Si es una bola de fuego activamos el quemado
			if(hits[i].entity->getType().compare("FireBall")==0){
				_burned=true;
			}
			//Entidades validas (Player que no seamos nosotros mismos)
			if(hits[i].entity!=_entity){
				if(hits[i].entity->getType() == "ScreamerShield") {
					CEntity* screamerShieldOwner = hits[i].entity->getComponent<CScreamerShieldDamageNotifier>("CScreamerShieldDamageNotifier")->getOwner();

					if(screamerShieldOwner == _entity)
						break;
				}

				entityHit=hits[i].entity;
				break;
			}
		}
		//Si hemos tocado una entidad, vemos si hay daño de expansion a otra entidad
		//Por último aplicamos el daño correspondiente a las entidades pertinentes
		CEntity *enemyToExpand=NULL;
		if(entityHit!=NULL && entityHit->getType() != "ScreamerShield"){
			enemyToExpand=findEnemyToExpand(entityHit);
			//Aplicamos daño a la entidad dada y a la más próxima (si la hay)
			if(_burned)
				triggerHitMessages(entityHit, _secondaryFireDamage + _secondaryFireDamage * _burnedIncrementPercentageDamage);
			else
				triggerHitMessages(entityHit, _secondaryFireDamage);
			if(enemyToExpand!=NULL){
				if(_burned)
					triggerHitMessages(enemyToExpand, _secondaryFireDamage + _secondaryFireDamage * _burnedIncrementPercentageDamage);
				else
					triggerHitMessages(enemyToExpand, _secondaryFireDamage);
			}
		}//if(entityHit!=NULL)

		//Desactivamos el daño por quemado
		_burned=false;

		decrementAmmo(_secondaryConsumeAmmo);
	}//secondaryFireWeapon
	//-------------------------------------------------------

	CEntity* CSniper::findEnemyToExpand(CEntity* entityHit){
	
		std::vector<CEntity*> entitiesHit;
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_maxExpansiveDistance);
		Vector3 explotionPos = entityHit->getPosition();
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit,Physics::CollisionGroup::ePLAYER);
			
		int nbHits = entitiesHit.size();
		
		//Calculamos la entidad válida con menor distancia
		CEntity* expandToEntity=NULL;
		float minDist=_maxExpansiveDistance+10;
		for(int i=0;i<nbHits;++i){
			std::string type = entitiesHit[i]->getType();
			//Si es válida (player que no sea el ya dañado, ni nosotros mismos)
			if(entitiesHit[i]!=entityHit && entitiesHit[i]!=_entity){
				//Si mejora la distancia
				float distance=entitiesHit[i]->getPosition().distance(entityHit->getPosition());
				if( distance < minDist){
					Vector3 direccion=entitiesHit[i]->getPosition()-entityHit->getPosition();
					direccion.normalise();
					Ray ray(entitiesHit[i]->getPosition()+Vector3(0,_heightShoot,0), direccion);
					//Si hay una linea recta de un jugador a otro despejada
					std::vector<Physics::CRaycastHit> hits;
					Physics::CServer::getSingletonPtr()->raycastMultiple(ray, distance,hits,false,Physics::CollisionGroup::ePLAYER);
					for(int j=0;j<hits.size();++j){
						//Si no se trata del dado y el candidato a expandir es que hay algo mas por lo que no damos
						if(hits[j].entity!= entityHit && hits[j].entity != entitiesHit[i])
							break;
					}
					expandToEntity=entitiesHit[i];
					minDist=distance;
				}//if(distance<distMinima)
			}
		}
		return expandToEntity;

	}//findEnemyToExpand
	//-------------------------------------------------------

	void CSniper::triggerHitMessages(CEntity* entityHit, float damageFire) {
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		m->setDamage(damageFire);
		m->setEnemy(_entity);
		entityHit->emitMessage(m);
	}// triggerHitMessages
	//__________________________________________________________________

	void CSniper::amplifyDamage(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto
		if(percentage == 0) {
			_primaryFireDamage = _defaultPrimaryFireDamage;
			_secondaryFireDamage = _defaultSecondaryFireDamage;
		}
		// Sino aplicamos el porcentaje pasado por parámetro
		else {
			_primaryFireDamage += percentage * _primaryFireDamage * 0.01f;
			_secondaryFireDamage += percentage * _secondaryFireDamage * 0.01f;
		}
	} // amplifyDamage
	//__________________________________________________________________

	void CSniper::onTick(unsigned int msecs) {
		/*
		// Controlamos el cooldown del disparo primario y secundario
		if(_primaryFireTimer > 0) {
			_primaryFireTimer -= msecs;
			
			if(_primaryFireTimer < 0){
				_primaryFireTimer = 0;
				if(_primaryFireIsActive)
					primaryFire();
			}
		}

		if(_secondaryFireTimer > 0) {
			_secondaryFireTimer -= msecs;

			if(_secondaryFireTimer < 0){
				_secondaryFireTimer = 0;
				if(_secondaryFireIsActive)
					secondaryFire();
			}
		}
		*/
	} // onTick
	//__________________________________________________________________



} // namespace Logic

