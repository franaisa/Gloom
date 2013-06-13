/**
@file SpellFeedback.cpp

@see Logic::ISpellFeedback
@see Logic::IComponent

@author Antonio Jesus Narváez Corrales
@author Francisco Aisa García
@date Mayo, 2013
*/

#include "SpellFeedback.h"

#include "Graphics.h"

// Mapa
#include "Map/MapEntity.h"

// Mensajes
#include "Logic/Messages/MessageAudio.h"
#include "Logic/Messages/MessagePrimarySpell.h"
#include "Logic/Messages/MessageSecondarySpell.h"

using namespace std;

namespace Logic {
	
	ISpellFeedback::ISpellFeedback(const string& spellName) : _spellName("spell" + spellName),
															_spellID(-1),
															_isPrimarySpell(false),
															_isPassive(false) {
	}

	//__________________________________________________________________

	ISpellFeedback::~ISpellFeedback() {
		// Nada que borrar
	}

	//__________________________________________________________________
		
	bool ISpellFeedback::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
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

	bool ISpellFeedback::accept(const shared_ptr<CMessage>& message) {
		TMessageType msgType = message->getMessageType();

		return msgType == Message::PRIMARY_SPELL ||
			   msgType == Message::SECONDARY_SPELL;
	}

	//__________________________________________________________________

	void ISpellFeedback::process(const shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::PRIMARY_SPELL: {
				shared_ptr<CMessagePrimarySpell> primarySpellMsg = static_pointer_cast<CMessagePrimarySpell>(message);
				if(!_isPassive && _isPrimarySpell){
					if( primarySpellMsg->getSpell() )
						spell();
					else
						stopSpell();
				}
				break;
			}
			case Message::SECONDARY_SPELL: {
				shared_ptr<CMessageSecondarySpell> secondarySpellMsg = static_pointer_cast<CMessageSecondarySpell>(message);
				if(!_isPassive && !_isPrimarySpell){
					if( secondarySpellMsg->getSpell() )
						spell();
					else
						stopSpell();
				}
				break;
			}
		}
	}

	//__________________________________________________________________

	void ISpellFeedback::emitSound(const string &ruta, const string &sound, bool notIfPlay){
		shared_ptr<CMessageAudio> audioMsg = make_shared<CMessageAudio>();
		
		audioMsg->setRuta(ruta);
		audioMsg->setId(sound);
		audioMsg->setPosition( _entity->getPosition() );
		audioMsg->setNotIfPlay(notIfPlay);
		audioMsg->setIsPlayer( _entity->isPlayer() );
			
		_entity->emitMessage(audioMsg);
	} // emitSound
	//__________________________________________________________________

} // namespace Logic

