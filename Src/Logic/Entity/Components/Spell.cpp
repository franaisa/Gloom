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
#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageControl.h"
#include "Logic/Messages/MessageAudio.h"

#include "Logic/Messages/MessagePrimarySpell.h"
#include "Logic/Messages/MessageSecondarySpell.h"

using namespace std;

namespace Logic {
	
	ISpell::ISpell(const string& spellName) : _spellName("spell" + spellName),
											_mustDeactivate(false) {

		// Nada que inicializar
	}

	//__________________________________________________________________

	ISpell::~ISpell() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpell::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_spellName + "ID") && "Debe tener id, mirar archivo spellType");

		// Leemos los atributos obligatorios de arma
		_spellID = (SpellType::Enum)entityInfo->getIntAttribute(_spellName + "ID");
		
		assert( entityInfo->hasAttribute("primarySpell") && "Debe de tener campo primarySpell");
		assert( entityInfo->hasAttribute("secondarySpell") && "Debe de tener campo secondarySpell");
		
		_mustDeactivate = (_spellID != entityInfo->getIntAttribute("primarySpell") && _spellID != entityInfo->getIntAttribute("secondarySpell"));

		return true;
	} // spawn+
	//__________________________________________________________________

	void ISpell::onActivate(){
		if(_mustDeactivate)
			deactivate();
		else
			spell();
	} // onActivate
	//__________________________________________________________________

	void ISpell::onStart(){
		if(_mustDeactivate)
			deactivate();
		else
			spell();
	} // onStart
	//__________________________________________________________________

	void ISpell::onDeactivate(){
		stopSpell();
	} // onDeactivate
	//__________________________________________________________________
} // namespace Logic

