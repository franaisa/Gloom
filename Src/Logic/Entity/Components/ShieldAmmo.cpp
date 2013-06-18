/**
@file ShieldAmmo.cpp

Contiene la implementacion del hechizo de shield

@see Logic::CShieldAmmo
@see Logic::ISpellAmmo

@author Jaime Chapinal Cervantes
@date Junio, 2013
*/

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/RaycastHit.h"

#include "ShieldAmmo.h"
#include "Shield.h"
#include "ShieldFeedback.h"

#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageDamaged.h"

using namespace std;

namespace Logic {

	IMP_FACTORY(CShieldAmmo);

	//__________________________________________________________________

	CShieldAmmo::CShieldAmmo() : ISpellAmmo("shield"),
								_spellIsActive(false){
		// Nada que hacer
	}

	//__________________________________________________________________

	CShieldAmmo::~CShieldAmmo() {
		// Nada que hacer
	}

	//__________________________________________________________________

	bool CShieldAmmo::accept(const shared_ptr<CMessage>& message) {
		return ISpellAmmo::accept(message) || 
			message->getMessageType() == Message::REDUCED_COOLDOWN;
	} // 
	//__________________________________________________________________

	void CShieldAmmo::process(const shared_ptr<CMessage>& message) {
		ISpellAmmo::process(message);
		switch( message->getMessageType() ) {
			case Message::REDUCED_COOLDOWN: {
				reduceCooldown(static_pointer_cast<CMessageReducedCooldown>(message)->getPercentCooldown());
			break;
			}
		}
	} // process
	//__________________________________________________________________
	bool CShieldAmmo::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !ISpellAmmo::spawn(entity, map, entityInfo) ) return false;

		// Nos aseguramos de tener todos los atributos que necesitamos
		//assert( entityInfo->hasAttribute("Cooldown") );
		assert( entityInfo->hasAttribute( "spellShieldRadius") );

		// Cooldown del disparo principal
		_shieldDamage = entityInfo->getIntAttribute("spellShieldDamage");
		_shieldRadius = entityInfo->getIntAttribute("spellShieldRadius");

		_friend[_friends] = _entity->getComponent<Logic::CShield>("CShield");
		if(_friend[_friends]) ++_friends;
		_friend[_friends] = _entity->getComponent<Logic::CShieldFeedback>("CShieldFeedback");
		if(_friend[_friends]) ++_friends;
		if(_friends == 0) assert("\nTiene que tenes alguno de los dos componentes");

		return true;
	}

	//__________________________________________________________________

	void CShieldAmmo::onActivate() {
		// Aqui enviaria el mensaje o lo que fuera para que pusiera en el hud
	}

	//__________________________________________________________________

	void CShieldAmmo::onAvailable() {
		/*ISpellAmmo::onAvailable();		
		_currentAmmo += _ammoPerPull;
		_currentAmmo = _currentAmmo > _maxAmmo ? _maxAmmo : _currentAmmo;*/
	}

	//__________________________________________________________________

	void CShieldAmmo::onTick(unsigned int msecs) {
		if(_spellIsActive){
			shieldDamage();
		}
	}

	//__________________________________________________________________

	bool CShieldAmmo::canUseSpell() {
		return true;
		//return _cooldownTimer == 0 && _currentAmmo > 0;
	}

	//__________________________________________________________________

	void CShieldAmmo::spell() {
		ISpellAmmo::spell();
		_spellIsActive = true;
	} // primaryFire
	//__________________________________________________________________

	void CShieldAmmo::stopSpell() {
		ISpellAmmo::stopSpell();
		_spellIsActive = false;
	} // stopPrimaryFire
	//__________________________________________________________________

	/*
	void CComeBackAmmo::reduceCooldown(unsigned int percentage) {
		// Si es 0 significa que hay que restaurar al que habia por defecto,
		// sino decrementamos conforme al porcentaje dado.
		
		
		_cooldown = percentage == 0 ? _defaultCooldown : (_defaultCooldown - (percentage * _cooldown * 0.01f));
		assert(_cooldown < _duration && "La duracion del cooldown reducido es inferior a la del hechizo, lo cual no tiene mucho sentido");
		
	}*/

	void CShieldAmmo::shieldDamage() {
		std::vector<CEntity*> entitiesHit;

		// Hacemos una query de overlap con la geometria de una esfera en la posicion 
		// en la que se encuentra la granada con el radio que se indique de explosion
		Physics::SphereGeometry explotionGeom = Physics::CGeometryFactory::getSingletonPtr()->createSphere(_shieldRadius);
		Physics::CServer::getSingletonPtr()->overlapMultiple(explotionGeom, _entity->getPosition(), entitiesHit);
		int nbHits = entitiesHit.size();
		if(nbHits==0){
			return;
		}

		// Mandamos el mensaje de daño a cada una de las entidades que hayamos golpeado
		// Además aplicamos un desplazamiento al jugador 
		for(int i = 0; i < nbHits; ++i) {
			// una vez tenemos las entidades en el radio de ceguera, hacemos el segundo filtro
			//que es coger solo a los players
			CEntity * aux = entitiesHit[i];

			if(entitiesHit[i] != NULL && 
				(entitiesHit[i]->getType() == "Hound" || 
					entitiesHit[i]->getType() == "Screamer" || 
					entitiesHit[i]->getType() == "Shadow" || 
					entitiesHit[i]->getType() == "Archangel") ) 
			{//begin if
				std::shared_ptr<CMessageDamaged> damage = std::make_shared<CMessageDamaged>();
				damage->setDamage(_shieldDamage);
				entitiesHit[i]->emitMessage(damage);
			}//end if
		}//end for
	}

}//namespace Logic