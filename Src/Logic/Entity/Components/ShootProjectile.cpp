/**
@file ShootProjectile.cpp

Contiene la implementación del componente que abstrae las particularidades
del disparo de proyectiles.
 
@see Logic::CShootProjectile
@see Logic::IComponent
*/

#include "ShootProjectile.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Messages/MessageAudio.h"

namespace Logic {
	//IMP_FACTORY(CShootProjectile);
	
	bool CShootProjectile::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!CShoot::spawn(entity, map, entityInfo)) return false;

		// Leer los parametros que toquen para los proyectiles
		std::stringstream aux;
		aux << "weapon" << _nameWeapon;	////!!!! Aqui debes de poner el nombre del arma que leera en el map.txt
		std::string weapon = aux.str();

		_projectileRadius = entityInfo->getFloatAttribute(weapon + "ProjectileRadius");

		if(entityInfo->hasAttribute("audioNoAmmo"))
			_noAmmo = entityInfo->getStringAttribute("audioNoAmmo");

		if(entityInfo->hasAttribute(weapon+"Audio"))
			_audioShoot = entityInfo->getStringAttribute(weapon+"Audio");

		return true;
	}

	//__________________________________________________________________

	// Implementa el patrón template, las clases derivadas solo deben
	// preocuparse de implementar el método "fireWeapon".
	void CShootProjectile::shoot() {
		if(_canShoot && _currentAmmo > 0 && _numberShots <= _currentAmmo){
			_canShoot = false;
			_cooldownTimer = 0;
				
			drawParticle("fire", "shootParticle");

			for(int i = 0; i < _numberShots; ++i)
				fireWeapon();

			decrementAmmo();

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
			maudio->setNotIfPlay(false);
			maudio->setIsPlayer(_entity->isPlayer());
			_entity->emitMessage(maudio);
		}
	}

} // namespace Logic

