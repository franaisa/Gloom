/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "ShootHammer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/Shoot.h"

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"
#include "Logic/Messages/MessageRebound.h"

#include "Graphics/Camera.h"



#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic 
{
	IMP_FACTORY(CShootHammer);
	
	bool CShootHammer::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!CShoot::spawn(entity,map,entityInfo))
			return false;
		
		_currentAmmo = 1;

		_damageReflect = entityInfo->getIntAttribute("weaponHammerDamageReflect");
		
		return true;
	}


	void CShootHammer::shoot(){
		
		if(_canShoot){

			_canShoot = false;
			_coldDownTime = 0;
		
			// Para generalizar las armas, todas tendras tantas balas como la variable numberShoots
			for(int i = 0; i < _numberShoots; ++i)
			{
				//Direccion
					Vector3 direction = Math::getDirection(_entity->getOrientation()); 
					//El origen debe ser mínimo la capsula (si chocamos el disparo en la capsula al mirar en diferentes direcciones ya esta tratado en la funcion de colision)
					//Posicion de la entidad + altura de disparo(coincidente con la altura de la camara)
					Vector3 origin = _entity->getPosition()+Vector3(0,_heightShoot,0); 

				//Me dispongo a calcular la desviacion del arma, en el map.txt se pondra en grados de dispersion (0 => sin dispersion)
				Ogre::Radian angle = Ogre::Radian( (  (((float)(rand() % 100))/100.0f) * (_dispersion)) /100);

			

				//Esto hace un random total, lo que significa, por ejemplo, que puede que todas las balas vayan hacia la derecha 
				Vector3 dispersionDirection = direction.randomDeviant(angle);
				dispersionDirection.normalise();

				// Creamos el ray desde el origen en la direccion del raton (desvido ya aplicado)
				Ray ray(origin, dispersionDirection);
			


				////////////////////////////////////////////////Dibujo del rayo


					Graphics::CScene *scene = Graphics::CServer::getSingletonPtr()->getActiveScene();
					Ogre::SceneManager *mSceneMgr = scene->getSceneMgr();

				
					std::stringstream aux;
					aux << "laser" << _nameWeapon << _temporal;
					++_temporal;
					std::string laser = aux.str();


					Ogre::ManualObject* myManualObject =  mSceneMgr->createManualObject(laser); 
					Ogre::SceneNode* myManualObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(laser+"_node"); 
 

					myManualObject->begin("laser", Ogre::RenderOperation::OT_LINE_STRIP);
					Vector3 v = ray.getOrigin();
					myManualObject->position(v.x,v.y,v.z);


				for(int i=0; i < _distance;++i){
					Vector3 v = ray.getPoint(i);
					myManualObject->position(v.x,v.y,v.z);
					// etc 
				}

					myManualObject->end(); 
					myManualObjectNode->attachObject(myManualObject);


				//////////////////////////////////fin del dibujado del rayo

				//Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
				CEntity *entityCollision = Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance,3);
		
				//Si hay colisión envíamos a dicha entidad un mensaje de daño
				if(entityCollision)
				{
					if(entityCollision->getName().compare("World")==0){

						Vector3 direccionOpuestaRay= ray.getDirection()*-1;
						Logic::CMessageRebound *m=new Logic::CMessageRebound();
						m->setDir(direccionOpuestaRay);
						_entity->emitMessage(m);

						Logic::CMessageDamaged *damage=new Logic::CMessageDamaged();
						damage->setDamage(_damageReflect);
						damage->setEnemy(_entity);
						_entity->emitMessage(damage);

					}
					// LLamar al componente que corresponda con el daño hecho (solamente si no fuera el mundo el del choque)
					else{
						Logic::CMessageDamaged *m=new Logic::CMessageDamaged();
						m->setDamage(_damage);
						m->setEnemy(entityCollision);
						entityCollision->emitMessage(m);
					}
				}
			
				//Para el rebote, si hay colision con la entidad mundo pues reboto en la dirección opuesta a la que miro
			
				
		
			}// fin del bucle para multiples disparos

		}// fin if _canShoot
	} // shoot
	//---------------------------------------------------------

	void CShootHammer::decrementAmmo() {
		// Redefinimos el metodo para que no se haga nada ya que el hammer
		// realmente no tiene municion
	}// decrementAmmo
	//---------------------------------------------------------

	/*void CShootHammer::triggerHitMessages(CEntity* entityHit) {
		if(entityHit->getName().compare("World")==0){
			Vector3 direccionOpuestaRay= ray.getDirection()*-1;
			Logic::CMessageRebound *m=new Logic::CMessageRebound();
			m->setDir(direccionOpuestaRay);
			_entity->emitMessage(m);

			Logic::CMessageDamaged *damage=new Logic::CMessageDamaged();
			damage->setDamage(_damageReflect);
			damage->setEnemy(_entity);
			_entity->emitMessage(damage);

		}
		// LLamar al componente que corresponda con el daño hecho (solamente si no fuera el mundo el del choque)
		else{
			Logic::CMessageDamaged *m=new Logic::CMessageDamaged();
			m->setDamage(_damage);
			m->setEnemy(entityHit);
			entityHit->emitMessage(m);
		}
	}// triggerHitMessages
	//---------------------------------------------------------*/

} // namespace Logic

