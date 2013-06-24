/***

@see Logic::CHudOverlay
@see Logic::IComponent

@author Antonio Jesus
@date Febrero, 2013
*/
#ifndef __Logic_Hud_H
#define __Logic_Hud_H

#include "Logic/Entity/Component.h"
#include "WeaponType.h"

namespace Hikari{
	class FlashControl;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el hud, mediante overlays
    @ingroup logicGroup

	@author Antonio Jesus Narvaez
	@date Febrero, 2013
*/
	class CHud : public IComponent
	{
		DEC_FACTORY(CHud);

	public:

		CHud();

		virtual ~CHud();

		// =======================================================================
		//                     METODOS HEREDADOS DE ICOMPONENT
		// =======================================================================

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente acepta mensajes que afectan al HUD.
		*/
		virtual bool accept(const std::shared_ptr<CMessage>& message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<CMessage>& message);
		

		virtual void onDeactivate();

		virtual void onActivate();

		void updateMatchTime(unsigned int minutes, unsigned int seconds);

	protected:

		virtual void onFixedTick(unsigned int msecs);

		void hudLife(int health);
		void hudShield(int shield);
		void hudWeapon(int ammo, int weapon);
		void hudAmmo(int ammo, int weapon);
		void hudSpawn(int spawn);
		void hudDispersion();
		void hudDeath();
		void hudRespawn();
		void weaponPicked(int weapon);
		void updatePrimarySkillCooldown(float cooldown);
		void updateSecondarySkillCooldown(float cooldown);
		void updatePrimarySpellCooldown(float cooldown);
		void updateSecondarySpellCooldown(float cooldown);
		void primarySkill();
		void secondarySkill();
		void primarySpell();
		void secondarySpell();

		/**
		Se usa para mostrar el localizador de impacto y setearle una posicion.

		@param radianAngle, angulos en radianes que se girara, sentido anti horario
		*/
		void hudDirectionImpact(float radianAngle);

	private:

		Hikari::FlashControl * _hud, *_respawn;

		std::string weapons[WeaponType::eSIZE];

		int _spawnTime,_acumSpawn;
	};

	REG_FACTORY(CHud);
}

#endif