/**
@file Spell.cpp

Contiene la implementación de la interfaz común
a todas las armas.

@see Logic::ISpell
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "Spell.h"
#include "SpellType.h"

// Mapa
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

using namespace std;

namespace Logic {
	
	ISpell::ISpell(const string& spellName) : _spellName("Spell" + spellName), _currentTime(0){

		// Nada que inicializar
	}

	//__________________________________________________________________

	ISpell::~ISpell() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpell::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		Map::CEntity *tempEntity = CEntityFactory::getSingletonPtr()->getInfo(_spellName);
		// Comprobamos que los atributos obligatorios existen
		assert( tempEntity->hasAttribute("ID") && "Debe tener id, mirar archivo spellType");

		// Leemos los atributos obligatorios de arma
		_spellID = (SpellType::Enum)tempEntity->getIntAttribute("ID");
		
		_duration = tempEntity->getFloatAttribute("Duration")*1000;

		return true;
	} // spawn+
	//__________________________________________________________________

	void ISpell::addDuration(){
		_currentTime += _duration;
	} // addDuration
	//__________________________________________________________________

	void ISpell::onWake(){
		_currentTime = _duration;
		spell();
	} // onWake
	//__________________________________________________________________

	void ISpell::onTick(unsigned int msecs){
		_currentTime -= msecs;

		if(_currentTime < 0){
			stopSpell();
			this->putToSleep(true);
		}
	} // onTick
	//__________________________________________________________________
} // namespace Logic