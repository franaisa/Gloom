/**
@file SpellManagerClient.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CSpellManagerClient
@see Logic::IComponent

@Author Antonio Jesus Narvaez
@date July, 2013
*/

#include "SpellsManagerClient.h"
#include "SpellType.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "AmplifyDamageClient.h"
#include "CoolDownClient.h"

#include "Logic/Messages/MessageAddSpell.h"


namespace Logic 
{
	IMP_FACTORY(CSpellsManagerClient);

	//---------------------------------------------------------
	
	CSpellsManagerClient::CSpellsManagerClient() {
		
	}

	//---------------------------------------------------------

	bool CSpellsManagerClient::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		_spells.resize(SpellType::eSIZE);
		// Rellenamos el vector con los punteros a los componentes correspondientes
		_spells[SpellType::eAMPLIFY_DAMAGE] = _entity->getComponent<Logic::CAmplifyDamageClient>("CAmplifyDamageClient");
		_spells[SpellType::eCOOLDOWN] = _entity->getComponent<Logic::CCoolDownClient>("CCoolDownClient");
		return true;

	} // spawn
	//---------------------------------------------------------

	bool CSpellsManagerClient::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ADD_SPELL;
	} // accept
	
	//---------------------------------------------------------

	void CSpellsManagerClient::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageAddSpell> spellMsg = std::static_pointer_cast<CMessageAddSpell>(message);
		if(_spells[spellMsg->getSpell()]->isSleeping())
			_spells[spellMsg->getSpell()]->wakeUp();
		else
			_spells[spellMsg->getSpell()]->addDuration();
	} // process
	//---------------------------------------------------------

} // namespace Logic

