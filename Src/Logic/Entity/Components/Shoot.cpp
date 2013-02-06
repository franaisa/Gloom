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

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"

#include "Logic/Messages/MessageAddAmmo.h"
#include "Logic/Messages/MessageAddWeapon.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic 
{
	//IMP_FACTORY(CShoot);
	
	//---------------------------------------------------------

	bool CShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("physic_radius")){
			_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		}
						
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;		////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();
												
		_name = entityInfo->getStringAttribute(weapon+"Name");
		_damage= (unsigned char) entityInfo->getIntAttribute(weapon+"Damage");
		_dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
		_distance = entityInfo->getFloatAttribute(weapon+"Distance");
		_numberShoots = (unsigned char) entityInfo->getIntAttribute(weapon+"NumberShoots");
		_coolDown = (unsigned char) entityInfo->getIntAttribute(weapon+"CoolDown");
		_ammo = entityInfo->getIntAttribute(weapon+"Ammo");
		
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
			break;
		}
		
	} // process
	//---------------------------------------------------------

	void CShoot::shoot(){
		
		if(_ammo > 0){

			//Generación del rayo habiendo obtenido antes el origen y la dirección
			Graphics::CCamera* camera = Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera();
		
		

			int currentNumberShoots = _numberShoots;
			// Para generalizar las armas, todas tendras tantas balas como la variable numberShoots
			if(_numberShoots > _ammo)
				currentNumberShoots = _ammo;
			
			for(int i = 0; i < currentNumberShoots; ++i)
			{

				// Se corrige la posicion de la camara
				Vector3 direction = camera->getTargetCameraPosition() - camera->getCameraPosition();
				direction.normalise();
				//El origen debe ser mínimo la capsula y por si miramos al suelo la separación mínima debe ser 1.5f ( en un futuro es probable que sea recomendable cambiar por no chocar con el grupo de uno mismo )
				Vector3 origin = camera->getCameraPosition() + (_capsuleRadius * direction);


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
				CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance,3);
			

				//resto las balas que tiene, luego enviare las que le quedan actualizadas, asi no envio un mensaje por balas (en la escopeta envairia 8 mensajes, asi solo uno)
				if(_name != "Hammer"){
					--_ammo;
				}
				//Si hay colisión envíamos a dicha entidad un mensaje de daño
				if(entity)
				{

					printf("impacto con %s\n", entity->getName().c_str());

					// LLamar al componente que corresponda con el daño hecho
					//entity->


					Logic::CMessageDamaged *m=new Logic::CMessageDamaged();
					m->setDamage(_damage);
					entity->emitMessage(m);
				}
			
				//Para el rebote, si hay colision con la entidad mundo pues reboto en la dirección opuesta a la que miro
				
			}// fin del bucle para multiples disparos
			if(_name != "Hammer"){				
				Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
				message->setAmmo(_ammo);

				//Cambio sobre uno, hay q cambiarlo ;-)
				message->setWeapon(1);
				_entity->emitMessage(message);
			}
			
		}
	} // shoot

	// Cuando activo una entidad de disparo, actulio la hub indicandole cuanta municion tengo
	
	void CShoot::activate() {
		IComponent::activate();

		Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
		message->setAmmo(_ammo);
		_entity->emitMessage(message);

	}

} // namespace Logic

