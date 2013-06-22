package hud 
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface;
	/**
	 * Clase que maneja el hud, y que recibe órdenes procedentes de C++
	 * para ir actualizandolo cuando suceden eventos
	 * 
	 * El flash que maneja esta clase contiene, mediante la definicion 
	 * en el .fla, los siguientes atributos
	 * <ul>
	 * <li>soulreaper: simbolo que maneja el soul reaper </li>
	 * <li>ironhellgoat: simbolo que maneja el iron hell goat </li>
	 * <li>minigun: simbolo que maneja la minigun</li>
	 * <li>railgun: simbolo que maneja la railgun</li>
	 * <li>shotgun: simbolo que maneja la shotgun</li>
	 * <li>lifebar: simbolo que maneja la vida del personaje</li>
	 * <li>pickedmessage: simbolo que maneja los mensajes de arma
	 * 		cogida, a su vez lleva un atributo weapon que es el
	 * 		TextField que lleva el mensaje del arma cogida</li>
	 * <li>primaryskill: simbolo que maneja la skill primaria</li>
	 * <li>secondaryskill: simbolo que maneja la skill secundaria</li>
	 * <li>primaryspell: simbolo que maneja el hechizo primario</li>
	 * <li>secondaryspell: simbolo que maneja el hechizo secundario</li>
	 * <li>crosshair: simbolo que maneja el crosshair</li>
	 * <li>time: simbolo que maneja el tiempo. A su vez lleva dentro
	 * 		dos atributos: mins (maneja los minutos) y secs (maneja
	 * 		los segundos)</li>
	 * <li>bullets: simbolo que maneja las balas del arma equipada</li>
	 * <li>armor: simbolo que maneja la armadura del personaje</li>
	 * </ul>
	 * @author Rubén Mulero Guerrero
	 */
	public class HudManager extends MovieClip 
	{
		private var equippedWeapon:MovieClip;
		
		public function HudManager() 
		{
			equippedWeapon = null;
			
			ExternalInterface.addCallback("updateWeapon", updateWeapon);
			ExternalInterface.addCallback("weaponPicked", weaponPicked);
			ExternalInterface.addCallback("updateTime", updateTime);
			ExternalInterface.addCallback("updateArmor", updateArmor);
			ExternalInterface.addCallback("updateLife", updateLife);
			ExternalInterface.addCallback("primarySkill", primarySkill);
			ExternalInterface.addCallback("secondarySkill", secondarySkill);
			ExternalInterface.addCallback("primarySpell", primarySpell);
			ExternalInterface.addCallback("secondarySpell", secondarySpell);
			ExternalInterface.addCallback("updatePrimarySkillCooldown", updatePrimarySkillCooldown);
			ExternalInterface.addCallback("updatePrimarySpellCooldown", updatePrimarySpellCooldown);
			ExternalInterface.addCallback("updateSecondarySpellCooldown", updateSecondarySpellCooldown);
			ExternalInterface.addCallback("updateSecondarySkillCooldown", updateSecondarySkillCooldown);
			ExternalInterface.addCallback("updateCrosshair", updateCrosshair);
			ExternalInterface.addCallback("updateTeam", updateTeam);
			ExternalInterface.addCallback("updateBullets", updateBullets);
			ExternalInterface.addCallback("hit", onHit);
			
			primaryspell.visible = false;
			secondaryspell.visible = false;
		}
		
		public function updateWeapon(weapon:String) {
			if (equippedWeapon != null) {
				equippedWeapon.gotoAndPlay("unselected");
				
			}
			if (this.bullets.currentFrameLabel != weapon) {
				this.bullets.gotoAndPlay(weapon);
			}
			
			equippedWeapon = getChildByName(weapon) as MovieClip
			equippedWeapon.gotoAndPlay("selected");
			
			if (weapon == "soulreaper") {
				this.bullets.asd.text = "";
			}
		}
		
		public function updateBullets(bullets:int) {
			this.bullets.asd.text = bullets.toString();
			
		}
		
		public function weaponPicked(weapon:String) {
			
			pickedmessage.picked(weapon);
			
		}
		
		public function updateTime(mins:int, secs:int) {
			
			time.mins = mins;
			time.secs = secs;
			
		}
		
		public function updateArmor(armadura:int) {
			trace(this.armor.armorbar.rotation);
			this.armor.armorbar.rotation = ( 90 / 100 ) * armadura;
			trace(this.armor.armorbar.rotation);
		}
		
		public function updateLife(life:int) {
			
			/*if (life > 100) {
				lifebar.gotoAndPlay("idle");
			}*/
			lifebar.scaleX = (2 + (life / 100.0) ) / 3;
			lifebar.scaleY = (2 + (life / 100.0) ) / 3;
			if ( life <= 15 ) {
				lifebar.gotoAndPlay("danger");
			}else {
				lifebar.gotoAndPlay("idle");
				lifebar.stop();
			}
			
		}
		
		public function primarySkill() {
			
			primaryskill.coolDown()
			
		}
		
		public function secondarySkill() {
			
			secondaryskill.coolDown()
			
		}
		
		public function primarySpell() {
			
			primaryskill.coolDown()
			
		}
		
		public function secondarySpell() {
			
			secondaryskill.coolDown()
			
		}
		
		public function updatePrimarySkillCooldown(cooldownTime:Number) {
			
			primaryskill.speed = 1.0 / cooldownTime;
			
			
		}
		
		public function updatePrimarySpellCooldown(cooldownTime:Number) {
			
			primaryspell.speed = 1.0 / cooldownTime;
			
		}
		
		public function updateSecondarySpellCooldown(cooldownTime:Number) {
			
			secondaryspell.speed = 2.0 / cooldownTime;
		}
		
		public function updateSecondarySkillCooldown(cooldownTime:Number) {
			
			secondaryskill.speed = 2.0 / cooldownTime;
			
		}
		
		public function updateCrosshair(crosshair:String) {
			
			this.crosshair.gotoAndPlay(crosshair);
			
		}
		
		public function updateTeam(team:String) {
			
			time.gotoAndPlay(team);
			
		}
		public function onHit():void {
			hit.gotoAndPlay("hit");
		}
		
	}

}