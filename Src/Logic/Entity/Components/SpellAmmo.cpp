/**
@file Ammo.cpp

@see Logic::ISpellAmmo
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SpellAmmo.h"
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
	
	ISpellAmmo::ISpellAmmo(const string& spellName) : _spellName("spell" + spellName) {

		// Nada que inicializar
	}

	//__________________________________________________________________

	ISpellAmmo::~ISpellAmmo() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpellAmmo::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if( !IComponent::spawn(entity,map,entityInfo) ) return false;

		// Comprobamos que los atributos obligatorios existen
		assert( entityInfo->hasAttribute(_spellName + "ID") && "Debe tener id, mirar archivo spellType");
		assert( entityInfo->hasAttribute(_spellName + "IsPassive") && "Es importante que tenga este campo");
		assert( entityInfo->hasAttribute(_spellName + "IsPrimarySpell") && "Este parametro debe de haberse seteado al crear la entidad");

		// Leemos los atributos obligatorios de arma
		_spellID = (SpellType::Enum)entityInfo->getIntAttribute(_spellName + "ID");
		_isPassive = entityInfo->getBoolAttribute(_spellName + "IsPassive");
		_isPrimarySpell = entityInfo->getBoolAttribute(_spellName + "IsPrimarySpell");		


		return true;
	}

	//__________________________________________________________________

	bool ISpellAmmo::accept(const shared_ptr<CMessage>& message) {
		// Solo nos interesan los mensajes de disparo.
		// Es importante que hagamos esto porque si no, el putToSleep
		// puede convertirse en nocivo.
		if(message->getMessageType() == Message::CONTROL) {
			shared_ptr<CMessageControl> controlMsg = static_pointer_cast<CMessageControl>(message);
			
			ControlType type = controlMsg->getType();
			
			return type == Control::USE_PRIMARY_SPELL ||
				   type == Control::USE_SECONDARY_SPELL;				   
		}

		return false;
	}

	//__________________________________________________________________

	void ISpellAmmo::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::CONTROL: {
				ControlType type = std::static_pointer_cast<CMessageControl>(message)->getType();

				if(type == Control::USE_PRIMARY_SPELL) {
					if(!_isPassive && _isPrimarySpell && canUseSpell())
						spell();
				}
				else if(type == Control::USE_SECONDARY_SPELL) {
					if(!_isPassive && !_isPrimarySpell && canUseSpell())
						spell();
				}

				break;
			}
		}
	}

	//__________________________________________________________________

	void ISpellAmmo::onAvailable(){
		if(_isPassive){
			if(_isPrimarySpell){
				shared_ptr<CMessagePrimarySpell> message = make_shared<CMessagePrimarySpell>();
				message->setSpell(true);
				_entity->emitMessage(message);
			}else{
				shared_ptr<CMessageSecondarySpell> message = make_shared<CMessageSecondarySpell>();
				message->setSpell(true);
				_entity->emitMessage(message);			
			}
		}

		if(_friend)
			_friend->stayAvailable();
	} // onAvailable
	//__________________________________________________________________

	void ISpellAmmo::onBusy() {
		if(_friend)
			_friend->stayBusy();
	} // onBusy
	//__________________________________________________________________

	void ISpellAmmo::onActivate() {
		if(_friend)
			_friend->activate();
	} // onBusy
	//__________________________________________________________________

	void ISpellAmmo::onDeactivate() {
		if(_friend)
			_friend->deactivate();
	} // onBusy
	//__________________________________________________________________


	void ISpellAmmo::spell() {
		
		// Si a llegao aqui, esq no es pasiva
		// Mandar el mensaje spell(true)
		if(_isPrimarySpell){
			auto m = make_shared<CMessagePrimarySpell>(true);
			_entity->emitMessage(m);
		}else{
			auto m = make_shared<CMessageSecondarySpell>(true);
			_entity->emitMessage(m);
		}
	} // spell
	//__________________________________________________________________

	void ISpellAmmo::stopSpell() {
		
		// Si a llegao aqui, esq no es pasiva
		// Mandar el mensaje spell(false)
		if(_isPrimarySpell){
			auto m = make_shared<CMessagePrimarySpell>(false);
			_entity->emitMessage(m);
		}else{
			auto m = make_shared<CMessageSecondarySpell>(false);
			_entity->emitMessage(m);
		}
	} // stopSpell
	//__________________________________________________________________

} // namespace Logic
