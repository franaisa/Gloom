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

#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageCreateParticle.h"


namespace Logic {

	IMP_FACTORY(CShootSniper);
	
	CShootSniper::~CShootSniper() {
		// Nada que hacer
	}//~CShootSniper
	//-------------------------------------------------------

	void CShootSniper::primaryShoot(){
		if(_primaryCanShoot && _currentAmmo > 0){
			_primaryCanShoot = false;
			_primaryCooldownTimer = 0;

			decrementAmmo();

			for(int i = 0; i < _numberShots; ++i) {
				primaryFireWeapon();				
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

	}//primaryShoot
	//-------------------------------------------------------

	void CShootSniper::secondaryShoot(){
		int secondaryConsumeAmmo=2;
		if(_secondaryCanShoot && _currentAmmo >= secondaryConsumeAmmo){
			_secondaryCanShoot = false;
			_secondaryCooldownTimer = 0;

			//Gasta varias balas(parametro a configurar por arquetipos)
			for(int i=0;i<secondaryConsumeAmmo;++i)
				decrementAmmo();

			for(int i = 0; i < _numberShots; ++i) {
				secondaryFireWeapon();				
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


	}//secondaryShoot
	//-------------------------------------------------------

	void CShootSniper::primaryFireWeapon(){
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, dispersionDirection);
			
		// Dibujamos el rayo en ogre para poder depurar
		//drawRaycast(ray);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _distance,hits,true);
		//Aplicamos daño si no somos nosotros mismos(se podria modificar la fisica para que no nos devuelva a nosotros)
		//Y ademas no hemos tocado ya pared
		
		for(int i=0;i<hits.size();++i){
			//Si tocamos el mundo no continuamos viendo hits y llamamos al pintado del rayo
			if(hits[i].entity->getType().compare("World")==0){
				float distanceWorld=hits[i].distance;
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
				cGraphics->changeScale(Vector3(0.2,0.2,10));//Alargamos el cilindro
				return;
			}
			//Sino mientras que no seamos nosotros mismos
			if(hits[i].entity->getEntityID()!=_entity->getEntityID()){
				std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
				m->setDamage(_damage);
				m->setEnemy(_entity);
				hits[i].entity->emitMessage(m);
			}
		}

	}//primaryFireWeapon
	//-------------------------------------------------------

	void CShootSniper::secondaryFireWeapon(){
		//Direccion
		Vector3 direction = Math::getDirection(_entity->getOrientation()); 
		//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
		Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);
		//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
		Vector3 dispersionDirection = direction.randomDeviant(angle);
		dispersionDirection.normalise();

		//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
		//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
		Vector3 origin = _entity->getPosition()+Vector3(0.0f,_heightShoot,0.0f);
		// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
		Ray ray(origin, dispersionDirection);
			
		// Dibujamos el rayo en ogre para poder depurar
		//drawRaycast(ray);

		// Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
		std::vector<Physics::CRaycastHit> hits;
		Physics::CServer::getSingletonPtr()->raycastMultiple(ray, _distance,hits,true);

		//Cogemos lo primero tocado que no seamos nosotros mismos y vemos si a un rango X hay enemigos (no nosotros)
		//Ojo en cooperativo tendremos que hacer distincion entre otros players aliados
		CEntity* entityHit=NULL;
		std::string type;
		for(int i=0;i<hits.size();++i){
			//Si tocamos mundo terminamos
			if(hits[i].entity->getType().compare("World")==0)
				break;
			//Entidades validas (Player que no seamos nosotros mismos)
			//OJO añadi remote player para probar en singlePlayer supongo que en MP no hay de este tipo de jugadores
			type = hits[i].entity->getType();
			if((type == "Screamer" || type == "Shadow" || type == "Hound" || type == "Archangel" ||
				type == "LocalScreamer" || type == "LocalShadow" || type == "LocalHound" || type == "LocalArchangel" || type == "RemotePlayer") && hits[i].entity!=_entity){
				entityHit=hits[i].entity;
				break;
			}
		}
		//Si tocamos una entidad valida aplicamos daño a dicha entidad (al final)
		//Ademas lanzamos un overlap esferico con X radio(distancia máxima) desde el centro del jugador dado
		//Si en dicho overlap tocamos a alguna entidad vemos si un rayo desde nosotros mismos llega a darle directamente (sin obstaculos)
		//Nos quedaremos con la entidad que este a menor distancia para aplicar el daño
		//Lo suyo seria que el overlap pueda ordenar por distancia las entidades que te devuelve para ir mirando por orden:
		// primero si la entidad es valida para aplicarle daño, segundo el rayo para los obstaculos (el orden ya estaria bien)
		if(entityHit!=NULL){
			std::vector<CEntity*> entitiesHit;
			
			Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(100);//Radio(dist maxima expansion)
			Vector3 explotionPos = entityHit->getPosition();
			Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, explotionPos, entitiesHit);
			
			int nbHits = entitiesHit.size();
			//Calculamos la entidad válida con menor distancia
			CEntity* expandToEntity=NULL;
			for(int i=0;i<nbHits;++i){
				std::cout << "bucle general " << i << std::endl;
				type = entitiesHit[i]->getType();
				float distMinima=120;
				//Si es válida (player que no sea el ya dañado, ni nosotros mismos) (nosotros mismos sin poner para comprobar que hay expansion de daño)
				if((type == "Screamer" || type == "Shadow" || type == "Hound" || type == "Archangel" ||
					type == "LocalScreamer" || type == "LocalShadow" || type == "LocalHound" || type == "LocalArchangel") && entitiesHit[i]!=entityHit){// && hits[i].entity->getEntityID()!=_entity->getEntityID() ){
					//Si mejora la distancia
					float distance=entitiesHit[i]->getPosition().distance(entityHit->getPosition());
					if( distance < distMinima){
						Vector3 direccion=entitiesHit[i]->getPosition()-entityHit->getPosition();
						direccion.normalise();
						Ray ray(entitiesHit[i]->getPosition()+Vector3(0,_heightShoot,0), direccion);
						//Si hay una linea recta de un jugador a otro despejada
						std::vector<Physics::CRaycastHit> hits;
						Physics::CServer::getSingletonPtr()->raycastMultiple(ray, distance,hits,false);
						for(int j=0;j<hits.size();++j){
							std::cout << "bucle interno " << j << std::endl;
							//Si no se trata del dado y el candidato a expandir es que hay algo mas por lo que no damos
							if(hits[j].entity!= entityHit && hits[j].entity != entitiesHit[i] && hits[j].entity->getType().compare("SpawnPoint")!=0)
								break;
						}
						expandToEntity=entitiesHit[i];
					}//if(distance<distMinima)
				}
			}

			//Aplicamos daño a la entidad dada y a la más próxima
			std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
			m->setDamage(_damage);
			m->setEnemy(_entity);
			entityHit->emitMessage(m);
			if(expandToEntity!=NULL){
				std::shared_ptr<CMessageDamaged> m2 = std::make_shared<CMessageDamaged>();
				m2->setDamage(_damage);
				m2->setEnemy(_entity);
				expandToEntity->emitMessage(m2);
			}

		
		}//if(entityHit!=NULL)

	}//secondaryFireWeapon
	//-------------------------------------------------------

} // namespace Logic

