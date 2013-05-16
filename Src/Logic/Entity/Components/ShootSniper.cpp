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

#include "Logic/Messages/MessageDamaged.h"



namespace Logic {

	IMP_FACTORY(CShootSniper);
	
	CShootSniper::~CShootSniper() {
		// Nada que hacer
	}//~CShootSniper
	//-------------------------------------------------------

	void CShootSniper::primaryShoot(){
	if(_canShoot && _currentAmmo > 0){
		std::cout << "Disparo primario sniper redefinido" << std::endl;
			_canShoot = false;
			_cooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

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
	std::cout << "Disparo secundario sniper redefinido" << std::endl;


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
		std::shared_ptr<CMessageDamaged> m = std::make_shared<CMessageDamaged>();
		for(int i=0;i<hits.size();++i){
			//Si tocamos el mundo no continuamos viendo hits
			if(hits[i].entity->getType().compare("World")==0)
				return;
			//Sino mientras que no seamos nosotros mismos
			if(hits[i].entity->getEntityID()!=_entity->getEntityID()){
				m->setDamage(_damage);
				m->setEnemy(_entity);
				hits[i].entity->emitMessage(m);
			}
		}

	}//primaryFireWeapon
	//-------------------------------------------------------

	void CShootSniper::secondaryFireWeapon(){
		


	}//secondaryFireWeapon
	//-------------------------------------------------------

} // namespace Logic

