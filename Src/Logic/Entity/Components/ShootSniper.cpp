/**
@file ShootSniper.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShootSniper
@see Logic::CShootRaycast

@author Jose Antonio García Yáñez
@date Mayo, 2013
*/

#include "ShootSniper.h"

#include "Physics/Server.h"
#include "Physics/RaycastHit.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Map/MapEntity.h"
#include "HudWeapons.h"

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageCreateParticle.h"


namespace Logic {

	IMP_FACTORY(CShootSniper);
	
	CShootSniper::~CShootSniper() {
		// Nada que hacer
	}//~CShootSniper
	//-------------------------------------------------------

	bool CShootSniper::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IWeapon::spawn(entity,map,entityInfo)) return false;

		if(entityInfo->hasAttribute(_weaponName+"MaxExpansiveDistance"))
			_maxExpansiveDistance = entityInfo->getFloatAttribute(_weaponName+"MaxExpansiveDistance");

		if(entityInfo->hasAttribute(_weaponName+"AmmoSpentPerSecondaryShot"))
			_secondaryConsumeAmmo = entityInfo->getIntAttribute(_weaponName+"AmmoSpentPerSecondaryShot");
		
		return true;
	} // spawn
	//__________________________________________________________________

	void CShootSniper::primaryFire(){
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 

		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, direction);
			
		// Dibujamos el rayo en ogre para poder depurar
		//drawRaycast(ray);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _shotsDistance, hits,true,Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eWORLD);

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
				return;
			}
			//Sino mientras que no seamos nosotros mismos
			if(hits[i].entity->getEntityID()!=_entity->getEntityID()){
				triggerHitMessages(hits[i].entity, _primaryFireDamage);
			}
		}

	}//primaryFireWeapon
	//-------------------------------------------------------

	void CShootSniper::secondaryFire(){
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, direction);
			
		// Dibujamos el rayo en ogre para poder depurar
		//drawRaycast(ray);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _shotsDistance, hits,true,Physics::CollisionGroup::ePLAYER | Physics::CollisionGroup::eWORLD);

		//Cogemos lo primero tocado que no seamos nosotros mismos y vemos si a un rango X hay enemigos (no nosotros)
		//Ojo en cooperativo tendremos que hacer distincion entre otros players aliados
		CEntity* entityHit=NULL;
		std::string type;
		for(int i=0;i<hits.size();++i){
			//Si tocamos mundo terminamos
			if(hits[i].entity->getType().compare("World")==0)
				break;
			//Entidades validas (Player que no seamos nosotros mismos)
			type = hits[i].entity->getType();
			if(hits[i].entity!=_entity){
				entityHit=hits[i].entity;
				break;
			}
		}
		//Si hemos tocado una entidad, vemos si hay daño de expansion a otra entidad
		//Por último aplicamos el daño correspondiente a las entidades pertinentes
		CEntity *enemyToExpand=NULL;
		if(entityHit!=NULL){
			enemyToExpand=findEnemyToExpand(entityHit);
			//Aplicamos daño a la entidad dada y a la más próxima (si la hay)
			triggerHitMessages(entityHit, _primaryFireDamage);
			if(enemyToExpand!=NULL){
				triggerHitMessages(enemyToExpand, _secondaryFireDamage);
			}
		}//if(entityHit!=NULL)

	}//secondaryFireWeapon
	//-------------------------------------------------------

	CEntity* CShootSniper::findEnemyToExpand(CEntity* entityHit){
	
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

	// Implementación por defecto de triggerHitMessages
	void CShootSniper::triggerHitMessages(CEntity* entityHit, float damageFire) {
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		m->setDamage(damageFire);
		m->setEnemy(_entity);
		entityHit->emitMessage(m);

	}// triggerHitMessager
	//__________________________________________________________________

} // namespace Logic

