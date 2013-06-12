/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "SpellsManager.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Maps/WorldState.h"

#include "SpellAmmo.h"

#include "ShotGunAmmo.h"
#include "SniperAmmo.h"
#include "MiniGunAmmo.h"
#include "SoulReaperAmmo.h"
#include "IronHellGoatAmmo.h"

#include "Logic/Messages/MessageReducedCooldown.h"
#include "Logic/Messages/MessageAddSpell.h"


namespace Logic 
{
	IMP_FACTORY(CSpellsManager);

	//---------------------------------------------------------
	
	CSpellsManager::CSpellsManager() {
		
	}

	//---------------------------------------------------------

	bool CSpellsManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) {
		if(!IComponent::spawn(entity,map,entityInfo)) return false;

		// Inicializamos el vector de armas.
		// De momento no tiene mucho sentido que comprobemos el número de armas que hay
		// por que luego usamos el getComponent a piñon.
		_spells.resize(SpellType::eSIZE);

		// Rellenamos el vector con los punteros a los componentes correspondientes

		/*
		_spells[SpellType::eSOUL_REAPER].second = _entity->getComponent<Logic::CSoulReaperAmmo>("CSoulReaperAmmo");
		_spells[SpellType::eSNIPER].second = _entity->getComponent<Logic::CSniperAmmo>("CSniperAmmo");
		_spells[SpellType::eSHOTGUN].second = _entity->getComponent<Logic::CShotGunAmmo>("CShotGunAmmo");
		_spells[SpellType::eMINIGUN].second = _entity->getComponent<Logic::CMiniGunAmmo>("CMiniGunAmmo");
		_spells[SpellType::eIRON_HELL_GOAT].second = _entity->getComponent<Logic::CIronHellGoatAmmo>("CIronHellGoatAmmo");
		*/

		
		/*
		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 0; i < _spellry.size(); ++i) {
			_spellry[i].first = false;
			_spellry[i].second->stayBusy();
			_spellry[i].second->inUse(false);
		}
		*/

		_primarySpell = (SpellType::Enum)entityInfo->getIntAttribute("primarySpell");
		_secondarySpell = (SpellType::Enum)entityInfo->getIntAttribute("secondarySpell");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CSpellsManager::onActivate() {
		
		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 0; i < _spells.size(); ++i) {
			_spells[i].first = false; // Por si acaso habian sido activadas anteriormente
			_spells[i].second->deactivate();
		}
	} // onActivate
	//---------------------------------------------------------

	void CSpellsManager::onDeactivate(){
		/*
		//reset de armas y balas
		for(unsigned int i = 0; i<_spellry.size();++i){
			_spellry[i].first = false;
			_spellry[i].second->resetAmmo();
			_spellry[i].second->inUse(false);
		}
		*/
		_spells[_primarySpell].second->deactivate();
		_spells[_secondarySpell].second->deactivate();
		
	}// onDeactivate
	//---------------------------------------------------------

	bool CSpellsManager::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ADD_SPELL
			|| msgType == Message::REDUCED_COOLDOWN;
	} // accept
	
	//---------------------------------------------------------

	void CSpellsManager::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::ADD_SPELL: {
				
				std::shared_ptr<CMessageAddSpell> addSpellMsg = std::static_pointer_cast<CMessageAddSpell>(message);
				unsigned int spellIndex = addSpellMsg->getSpell();
				_spells[spellIndex].first = true;
				_spells[spellIndex].second->activate();
				break;
			}
			case Message::REDUCED_COOLDOWN: {
				std::shared_ptr<CMessageReducedCooldown> reducedCdMsg = std::static_pointer_cast<CMessageReducedCooldown>(message);
				reduceCooldowns( reducedCdMsg->getPercentCooldown() );
				break;
			}
		}

	} // process
	
	//---------------------------------------------------------

	void CSpellsManager::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas las armas en base al porcentaje dado
		for(unsigned int i = 0; i < _spells.size(); ++i) {
			_spells[i].second->reduceCooldown(percentage);
		}
	}

	//---------------------------------------------------------

} // namespace Logic

