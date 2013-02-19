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
	IMP_FACTORY(CShoot);
	
	//---------------------------------------------------------

	bool CShoot::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("physic_radius")){
			_capsuleRadius = entityInfo->getFloatAttribute("physic_radius");
		}

		if(entityInfo->hasAttribute("heightShoot")){
			_heightShoot = entityInfo->getIntAttribute("heightShoot");
		}
		
						
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;		////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();
												
		_name = entityInfo->getStringAttribute(weapon+"Name");
		_damage= (unsigned char) entityInfo->getIntAttribute(weapon+"Damage");
		_dispersion = entityInfo->getFloatAttribute(weapon+"Dispersion");
		_distance = entityInfo->getFloatAttribute(weapon+"Distance");
		_numberShoots = (unsigned char) entityInfo->getIntAttribute(weapon+"NumberShoots");
		_coldDown = (unsigned char) entityInfo->getIntAttribute(weapon+"ColdDown") * 1000;
		_maxAmmo = entityInfo->getIntAttribute(weapon+"MaxAmmo");
		_id = entityInfo->getIntAttribute(weapon+"Id");
		
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


	void CShoot::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
		if(_coldDownTime < _coldDown)
			_coldDownTime += msecs;
		else
			_canShoot = true;

	} // tick
	//----------------------------------------------------------

	void CShoot::shoot(){

		if(_canShoot && _currentAmmo > 0){

				_canShoot = false;
				_coldDownTime = 0;
		
				int currentNumberShoots = _numberShoots;
				// Para generalizar las armas, todas tendras tantas balas como la variable numberShoots
				if(_numberShoots > _currentAmmo)
					currentNumberShoots = _currentAmmo;
			
				for(int i = 0; i < currentNumberShoots; ++i)
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

					// Creamos el ray desde el origen en la direccion del raton (desvio ya aplicado)
					Ray ray(origin, dispersionDirection);
			
					// Dibujado del rayo en Ogre
					drawRaycast(ray);

			

					//Rayo lanzado por el servidor de físicas de acuerdo a la distancia de potencia del arma
					CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance,3);
			

					//resto las balas que tiene, luego enviare las que le quedan actualizadas, asi no envio un mensaje por balas (en la escopeta envairia 8 mensajes, asi solo uno)
					if(_name != "Hammer"){
						--_currentAmmo;
					}
					//Si hay colisión envíamos a dicha entidad un mensaje de daño
					if(entity)
					{

						printf("impacto con %s\n", entity->getName().c_str());

						// LLamar al componente que corresponda con el daño hecho
						//entity->


						Logic::CMessageDamaged *m=new Logic::CMessageDamaged();
						m->setDamage(_damage);
						m->setEnemy(_entity);
						entity->emitMessage(m);
					}
			
					//Para el rebote, si hay colision con la entidad mundo pues reboto en la dirección opuesta a la que miro
				
				}// fin del bucle para multiples disparos
				if(_name != "Hammer"){				
					Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
					message->setAmmo(_currentAmmo);

					//Cambio sobre uno, hay q cambiarlo ;-)
					message->setWeapon(_id);
					_entity->emitMessage(message);
				}
			} // fin if de _canShoot && _ammo > 0
	} // shoot

	// Cuando activo una entidad de disparo, actulio la hub indicandole cuanta municion tengo
	
	void CShoot::activate() {
		IComponent::activate();

		Logic::CMessageHudWeapon *message = new Logic::CMessageHudWeapon();
		message->setWeapon(_id);
		message->setAmmo(_currentAmmo);
		_entity->emitMessage(message);
	} // activate
	//----------------------------------------------------------

	void CShoot::addAmmo(int weapon, int ammo, bool iAmCatch)
	{
		if(_currentAmmo + ammo>_maxAmmo)
			_currentAmmo = _maxAmmo;
		else
			_currentAmmo += ammo;

		if(iAmCatch){
			Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
			message->setWeapon(_id);
			message->setAmmo(_currentAmmo);
			_entity->emitMessage(message);
		}
	} // addAmmo
	//----------------------------------------------------------

	void CShoot::decrementAmmo() {
		--_currentAmmo;

		// Notificamos al hud para que cambie la cantidad de municion
		// que tenemos
		Logic::CMessageHudAmmo *message = new Logic::CMessageHudAmmo();
		message->setAmmo(_currentAmmo);

		//Cambio sobre uno, hay q cambiarlo ;-)
		message->setWeapon(_id);
		_entity->emitMessage(message);
	}// decrementAmmo
	//----------------------------------------------------------

	void CShoot::resetAmmo()
	{
		//si yo soy el weapon
		_currentAmmo = 0;
	} // resetAmmo
	//----------------------------------------------------------

	void CShoot::drawRaycast(const Ray& raycast) {
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
	//----------------------------------------------------------


} // namespace Logic

