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

#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageDamaged.h"

#include "Graphics/Camera.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreManualObject.h>

namespace Logic {
	IMP_FACTORY(CShootRaycast);
	
	void CShootRaycast::shoot() {
		if(_canShoot && _currentAmmo > 0 && _numberShoots <= _currentAmmo){
			_canShoot = false;
			_coldDownTime = 0;
				
			for(int i = 0; i < _numberShoots; ++i) {
				std::pair<CEntity*, Ray> entityHit = fireWeapon();
				if(entityHit.first != NULL) {
					printf("impacto con %s\n", entityHit.first->getName().c_str());
					triggerHitMessages(entityHit);
				}

				decrementAmmo();
			}
		}
		else if(_currentAmmo == 0) {
			// Ejecutar sonidos y animaciones de falta de balas
			//triggerRunOutOfAmmoMessages();
		}
	}// fireWeapon
	//----------------------------------------------------------

	// La implementacion por defecto de fireWeapon se basa en el uso del raycasting
	// Devuelve la entidad con la que ha colisionado el raycasting, si es que hay alguna
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
		CEntity *entity = Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance,3);

		return std::pair<CEntity*, Ray>(Physics::CServer::getSingletonPtr()->raycastClosestInverse(ray, _distance, 3), ray);
	}// fireWeapon
	//----------------------------------------------------------

	void CShootRaycast::triggerHitMessages(std::pair<CEntity*, Ray> entityHit) {
		Logic::CMessageDamaged* m = new Logic::CMessageDamaged();
		m->setDamage(_damage);
		m->setEnemy(_entity);
		entityHit.first->emitMessage(m);
	}// triggerHitMessages
	//----------------------------------------------------------

} // namespace Logic

