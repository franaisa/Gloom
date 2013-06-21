#include "Hud.h"

#include "Logic/Messages/Message.h"
#include "Logic/Messages/MessageHudLife.h"
#include "Logic/Messages/MessageHudShield.h"
#include "Logic/Messages/MessageHudAmmo.h"
#include "Logic/Messages/MessageHudWeapon.h"
#include "Logic/Messages/MessageHudSpawn.h"
#include "Logic/Messages/MessageChangeMaterialHudWeapon.h"
#include "Logic/Messages/MessageChangeWeaponGraphics.h"
#include "Logic/Server.h"
#include "Logic/Messages/MessageHudDispersion.h"
#include "Logic/Messages/MessageHudDebug.h"
#include "Logic/Messages/MessageHud.h"
#include "Logic/Messages/MessageHudDebugData.h"
#include "Logic/Messages/MessageImpact.h"

#include "Logic/Maps/GUIManager.h"

#include "FlashControl.h"


namespace Logic{

	IMP_FACTORY(CHud);

	CHud::CHud(){
		
		weapons[WeaponType::eSOUL_REAPER] = "soulreaper";
		weapons[WeaponType::eSNIPER] = "railgun";
		weapons[WeaponType::eSHOTGUN] = "shotgun";
		weapons[WeaponType::eMINIGUN] = "minigun";
		weapons[WeaponType::eIRON_HELL_GOAT] = "ironhellgoat";
		
		
	}

	CHud::~CHud(){
		Logic::CGUIManager::getSingletonPtr()->deleteGUI("hud");
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CHud::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		return true;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	void CHud::onDeactivate(){
		_hud->hide();
	}

	void CHud::onActivate(){
		CGUIManager * guiManager = Logic::CGUIManager::getSingletonPtr();
		guiManager->addGUI("hud", Hikari::Position(Hikari::Center));
		guiManager->load("hud", "Hud.swf");
		guiManager->setTransparent("hud",true);
		_hud = guiManager->getGUIControl("hud");
		_hud->show();
		_hud->callFunction("updateWeapon",Hikari::Args(weapons[0]));
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CHud::accept(const std::shared_ptr<CMessage>& message) {
		Logic::TMessageType msgType = message->getMessageType();

		return	msgType == Message::HUD_LIFE		|| 
				msgType == Message::HUD_SHIELD		|| 
				msgType == Message::HUD_AMMO		|| 
				msgType == Message::HUD_WEAPON		|| 
				msgType == Message::HUD_DISPERSION  ||
				msgType == Message::HUD_SPAWN		|| 
				msgType == Message::HUD_DEBUG		|| 
				msgType == Message::HUD_DEBUG_DATA	||
				msgType == Message::CHANGE_MATERIAL_HUD_WEAPON ||
				msgType == Message::IMPACT			||
				msgType == Message::HUD_DISPERSION	||
				msgType == Message::PRIMARY_SPELL	||
				msgType == Message::SECONDARY_SPELL	||
				msgType == Message::HUD				;

	}



	void CHud::process(const std::shared_ptr<CMessage>& message) {
		switch( message->getMessageType() ) {
			case Message::HUD_LIFE: {
				std::shared_ptr<CMessageHudLife> hudLifeMsg = std::static_pointer_cast<CMessageHudLife>(message);
				hudLife( hudLifeMsg->getLife() );
				break;
			}
			case Message::HUD_SHIELD: {
				std::shared_ptr<CMessageHudShield> hudShieldMsg = std::static_pointer_cast<CMessageHudShield>(message);
				hudShield( hudShieldMsg->getShield() );
				break;
			}
			case Message::HUD_AMMO: {
				std::shared_ptr<CMessageHudAmmo> hudAmmoMsg = std::static_pointer_cast<CMessageHudAmmo>(message);
				hudAmmo( hudAmmoMsg->getAmmo(), hudAmmoMsg->getWeapon() );
				break;
			}
			case Message::HUD_WEAPON: {
				std::shared_ptr<CMessageHudWeapon> hudWeaponMsg = std::static_pointer_cast<CMessageHudWeapon>(message);
				hudWeapon( hudWeaponMsg->getAmmo(), hudWeaponMsg->getWeapon() );
				break;
			}
			/*case Message::HUD_SPAWN: {
				std::shared_ptr<CMessageHudSpawn> hudSpawnMsg = std::static_pointer_cast<CMessageHudSpawn>(message);
				_spawnTime = hudSpawnMsg->getTime();
				_acumSpawn=3000;
				if(_spawnTime==0){
					hudRespawn();
				}else{
					hudDeath();
				}
				break;
			}*/

			case Message::HUD:{
				std::shared_ptr<CMessageHud> hudmsg = std::static_pointer_cast<CMessageHud>(message);
				switch(hudmsg->getType()){
					case CMessageHud::HudType::PRIMARY_SKILL:
						primarySkill();
						break;
					case CMessageHud::HudType::SECONDARY_SKILL:
						secondarySkill();
						break;
					break;
				}
			}

			case Message::IMPACT: {
				std::shared_ptr<CMessageImpact> impMes = std::static_pointer_cast<CMessageImpact>(message);
				hudDirectionImpact( impMes->getDirectionImpact() );
				break;
			}

			case Message::PRIMARY_SPELL:
				primarySpell();
				break;

			case Message::SECONDARY_SPELL:
				secondarySpell();
				break;
			/*case Message::HUD_DISPERSION: {
				std::shared_ptr<CMessageHudDispersion> hudDisp = std::static_pointer_cast<CMessageHudDispersion>(message);
				_dispersionTime = hudDisp->getTime();
				_resetMirilla = hudDisp->getReset();

				if (!_resetMirilla)
				{
					_dispersionWidth = hudDisp->getHeight();
					_dispersionHeight = hudDisp->getWidth();
				}
				break;
			}*/
		}

	} // process


	void CHud::hudLife(int health){
		_hud->callFunction("updateLife", Hikari::Args(health));
	}
	void CHud::hudWeapon(int ammo, int weapon){
		_hud->callFunction("updateWeapon",Hikari::Args(weapons[weapon]));
		_hud->callFunction("updateBullets", Hikari::Args(ammo));
	}
	void CHud::hudAmmo(int ammo, int weapon){
		_hud->callFunction("updateBullets", Hikari::Args(ammo));
	}
	void CHud::hudSpawn(int spawn){
		_hud->hide();
	}

	void CHud::hudDispersion(){

	}
	void CHud::hudDeath(){

	}

	void CHud::hudDirectionImpact(float radianAngle) {
	}

	void CHud::hudRespawn(){
		_hud->show();
	}

	void CHud::hudShield(int shield){
		_hud->callFunction("updateArmor", Hikari::Args(shield));
	}

	void CHud::weaponPicked(int weapon){
		std::string sweapon;
		switch(weapon){
			case WeaponType::eIRON_HELL_GOAT:
				sweapon = "Iron hell goat";
				break;
			case WeaponType::eMINIGUN:
				sweapon = "Minigun";
				break;
			case WeaponType::eSHOTGUN:
				sweapon = "Shotgun";
				break;
			case WeaponType::eSNIPER:
				sweapon = "Railgun";
				break;
		}
		_hud->callFunction("weaponPicked", Hikari::Args(sweapon));
	}

	void CHud::updatePrimarySkillCooldown(float cooldown){
		_hud->callFunction("updatePrimarySkillCooldown", Hikari::Args(cooldown));
	}
	void CHud::updateSecondarySkillCooldown(float cooldown){
		_hud->callFunction("updateSecondarySkillCooldown", Hikari::Args(cooldown));
	}
	void CHud::updatePrimarySpellCooldown(float cooldown){
		_hud->callFunction("updatePrimarySpellCooldown", Hikari::Args(cooldown));
	}
	void CHud::updateSecondarySpellCooldown(float cooldown){
		_hud->callFunction("updateSecondarySpellCooldown", Hikari::Args(cooldown));
	}

	void CHud::primarySkill(){
		_hud->callFunction("primarySkill", Hikari::Args());
	}
	void CHud::secondarySkill(){
		_hud->callFunction("secondarySkill", Hikari::Args());
	}
	void CHud::primarySpell(){
		_hud->callFunction("primarySpell", Hikari::Args());
	}
	void CHud::secondarySpell(){
		_hud->callFunction("secondarySpell", Hikari::Args());
	}
	
}