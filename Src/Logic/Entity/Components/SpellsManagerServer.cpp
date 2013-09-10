/**
@file SpellManagerServer.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CSpellManagerServer
@see Logic::IComponent

@Author Antonio Jesus Narvaez
@date July, 2013
*/

#include "SpellsManagerServer.h"
#include "SpellType.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "AmplifyDamageServer.h"
#include "CoolDownServer.h"

#include "Logic/Messages/MessageAddSpell.h"


namespace Logic 
{
	IMP_FACTORY(CSpellsManagerServer);

	//---------------------------------------------------------
	
	CSpellsManagerServer::CSpellsManagerServer() {
		
	}

	//---------------------------------------------------------

	bool CSpellsManagerServer::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		_spells.resize(SpellType::eSIZE);
		// Rellenamos el vector con los punteros a los componentes correspondientes
		_spells[SpellType::eAMPLIFY_DAMAGE] = _entity->getComponent<Logic::CAmplifyDamageServer>("CAmplifyDamageServer");
		_spells[SpellType::eCOOLDOWN] = _entity->getComponent<Logic::CCoolDownServer>("CCoolDownServer");
		return true;

	} // spawn
	//---------------------------------------------------------

	bool CSpellsManagerServer::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ADD_SPELL;
	} // accept
	
	//---------------------------------------------------------

	void CSpellsManagerServer::process(const std::shared_ptr<CMessage>& message) {
		std::shared_ptr<CMessageAddSpell> spellMsg = std::static_pointer_cast<CMessageAddSpell>(message);
		if(_spells[spellMsg->getSpell()]->isSleeping())
			_spells[spellMsg->getSpell()]->wakeUp();
		else
			_spells[spellMsg->getSpell()]->addDuration();
	} // process
	//---------------------------------------------------------

} // namespace Logic

