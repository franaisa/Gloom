/**
@file Shoot.cpp

Contiene la implementación del componente que gestiona las armas y que administra el disparo.
 
@see Logic::CShoot
@see Logic::IComponent

*/

#include "SpellsManagerClient.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/ArrayGraphics.h"
#include "Logic/Maps/WorldState.h"

#include "SpellAmmo.h"

#include "AmplifyDamageAmmo.h"
#include "ComeBackAmmo.h"
#include "ScopeAmmo.h"
#include "GravityAmmo.h"
#include "HardDeathAmmo.h"
#include "ShieldAmmo.h"
#include "CoolDownAmmo.h"
#include "ResurrectionAmmo.h"


#include "Logic/Messages/MessageReducedCooldown.h"
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

		// Inicializamos el vector de armas.
		// De momento no tiene mucho sentido que comprobemos el número de armas que hay
		// por que luego usamos el getComponent a piñon.
		_spells.resize(SpellType::eSIZE);

		// Rellenamos el vector con los punteros a los componentes correspondientes

		_spells[SpellType::eAMPLIFY_DAMAGE].second = _entity->getComponent<Logic::CAmplifyDamageAmmo>("CAmplifyDamageAmmo");
		_spells[SpellType::eCOME_BACK].second = _entity->getComponent<Logic::CComeBackAmmo>("CComeBackAmmo");
		_spells[SpellType::eSCOPE].second = _entity->getComponent<Logic::CScopeAmmo>("CScopeAmmo");
		_spells[SpellType::eGRAVITY].second = _entity->getComponent<Logic::CGravityAmmo>("CGravityAmmo");
		_spells[SpellType::eHARD_DEATH].second = _entity->getComponent<Logic::CHardDeathAmmo>("CHardDeathAmmo");
		_spells[SpellType::eSHIELD].second = _entity->getComponent<Logic::CShieldAmmo>("CShieldAmmo");
		_spells[SpellType::eCOOLDOWN].second = _entity->getComponent<Logic::CCoolDownAmmo>("CCoolDownAmmo");
		_spells[SpellType::eRESURECTION].second = _entity->getComponent<Logic::CResurrectionAmmo>("CResurrectionAmmo");
		
		
		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 0; i < _spells.size(); ++i) {
			_spells[i].first = false;
			_spells[i].second->stayBusy();
		}

		_primarySpell = (SpellType::Enum)entityInfo->getIntAttribute("primarySpell");
		_secondarySpell = (SpellType::Enum)entityInfo->getIntAttribute("secondarySpell");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CSpellsManagerClient::onActivate() {
		
		// El resto de las armas están desactivadas, ya que no las tenemos
		for(unsigned int i = 0; i < _spells.size(); ++i) {
			_spells[i].first = false; // Por si acaso habian sido activadas anteriormente
			_spells[i].second->deactivate();
		}
	} // onActivate
	//---------------------------------------------------------

	void CSpellsManagerClient::onDeactivate(){
		
		// Solo deberias de estar activos estos dos.
		_spells[_primarySpell].second->deactivate();
		_spells[_secondarySpell].second->deactivate();
		
	}// onDeactivate
	//---------------------------------------------------------

	bool CSpellsManagerClient::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return msgType == Message::ADD_SPELL
			|| msgType == Message::REDUCED_COOLDOWN;
	} // accept
	
	//---------------------------------------------------------

	void CSpellsManagerClient::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::ADD_SPELL: {
				
				std::shared_ptr<CMessageAddSpell> addSpellMsg = std::static_pointer_cast<CMessageAddSpell>(message);
				unsigned int spellIndex = addSpellMsg->getSpell();
				
				if(spellIndex == 1){
					_spells[_primarySpell].first = true;
					_spells[_primarySpell].second->activate();
				}else{
					if(spellIndex == 2){
						_spells[_secondarySpell].first = true;
						_spells[_secondarySpell].second->activate();		
					}else{
						printf("\nCuidado, has puesto un hechizo no valido, o es 1 (primario) o es 2 (secundario)");
					}
				}
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

	void CSpellsManagerClient::reduceCooldowns(int percentage) {
		// Reducimos el cooldown de todas los hechizos posibles
			_spells[_primarySpell].second->reduceCooldown(percentage);
			_spells[_secondarySpell].second->reduceCooldown(percentage);
	}
	//---------------------------------------------------------

} // namespace Logic

