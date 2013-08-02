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
			ExternalInterface.addCallback("secondarySkillAvaiable", secondarySkillAvaiable);
			ExternalInterface.addCallback("primarySkillAvaiable", primarySkillAvaiable);
			ExternalInterface.addCallback("updateCrosshair", updateCrosshair);
			ExternalInterface.addCallback("updateTeam", updateTeam);
			ExternalInterface.addCallback("updateBullets", updateBullets);
			ExternalInterface.addCallback("hit", onHit);
			ExternalInterface.addCallback("reset", reset);
			
			ironhellgoat.gotoAndPlay("none");
			shotgun.gotoAndPlay("none");
			minigun.gotoAndPlay("none");
			railgun.gotoAndPlay("none");
			
			soulreaper.selector.visible = false;
			ironhellgoat.selector.visible = false;
			shotgun.selector.visible = false;
			minigun.selector.visible = false;
			railgun.selector.visible = false;
		}
		
		public function reset() {
			ironhellgoat.gotoAndPlay("none");
			shotgun.gotoAndPlay("none");
			minigun.gotoAndPlay("none");
			railgun.gotoAndPlay("none");
			
			ironhellgoat.selector.visible = false;
			shotgun.selector.visible = false;
			minigun.selector.visible = false;
			railgun.selector.visible = false;
			
			updateWeapon("soulreaper");
			
			updateArmor(0);
		}
		
		public function updateWeapon(weapon:String) {
			if (equippedWeapon != null) {
				equippedWeapon.selector.visible = false;
				
			}
			/*if (this.bullets.currentFrameLabel != weapon) {
				this.bullets.gotoAndPlay(weapon);
			}*/
			
			equippedWeapon = getChildByName(weapon) as MovieClip;
			equippedWeapon.selector.visible = true;
			//equippedWeapon.name = weapon;
			if (weapon == "soulreaper") {
				this.bullets.asd.text = "";
				
			}
			crosshair.gotoAndPlay(weapon);
			
			trace(equippedWeapon.currentFrameLabel);
			
			if (equippedWeapon.currentFrameLabel != "avaiable") {
				equippedWeapon.gotoAndPlay("avaiable");
			}
		}
		
		public function updateBullets(bullets:int, weapon: String) {
			if (equippedWeapon != null && equippedWeapon.name == weapon) {
				this.bullets.asd.text = bullets.toString();
			}
			var weaponaux:MovieClip = getChildByName(weapon) as MovieClip;
			if (weaponaux.visible == false) {
				weaponPicked(weapon);
				weaponaux.visible = true;
			}
			
			if (bullets == 0) {
				
				equippedWeapon.gotoAndPlay("unavaiable");
				
			}
		}
		
		public function weaponPicked(weapon:String) {
			if (weapon == "ironhellgoat") {
				pickedmessage.picked("Iron Hell Goat");
			} else if (weapon == "shotgun") {
				pickedmessage.picked("ShotGun");
			} else if (weapon == "minigun") {
				pickedmessage.picked("MiniGun");
			} else if (weapon == "railgun") {
				pickedmessage.picked("RailGun");
			} 
		}
		
		public function updateTime(mins:int, secs:int) {
			
			time.mins.text = mins.toString();
			if (secs < 10) {
				time.secs.text = "0"+secs.toString();
			}else {
				time.secs.text = secs.toString();
			}
			
			
		}
		
		public function updateArmor(armadura:int) {
			lifebar.armorbar.scaleX = armadura / 100.0;
			
			if (lifebar.armorbar.scaleX > 1) {
				lifebar.armorbar.scaleX = 1;
			}
			lifebar.armornumber.text = armadura.toString();
		}
		
		public function updateLife(life:int) {
			
			/*if (life > 100) {
				lifebar.gotoAndPlay("idle");
			}*/
			lifebar.lifebar.scaleX = life / 100.0;
			
			if (lifebar.lifebar.scaleX > 1) {
				lifebar.lifebar.scaleX = 1;
			}
			
			/*if ( life <= 15 ) {
				lifebar.lifebar.gotoAndPlay("danger");
			}else {
				lifebar.lifebar.gotoAndPlay("idle");
				lifebar.lifebar.stop();
			}*/
			lifebar.lifenumber.text = life.toString();
		}
		
		public function primarySkill() {
			
			primaryskill.gotoAndPlay("cd");
			
		}
		
		public function secondarySkill() {
			
			secondaryskill.gotoAndPlay("cd");
			
		}
		
		public function primarySpell() {
			/*if (primaryspell.visible == false) {
				primaryspell.visible = true
				return;
			}
			primaryspell.gotoAndPlay("cooldown");
			*/
		}
		
		public function secondarySpell() {
			/*if (secondaryspell.visible == false) {
				secondaryspell.visible = true
				return;
			}
			secondaryspell.gotoAndPlay("cooldown");*/
			
		}
		
		public function primarySkillAvaiable() {
			
			//primaryskill.speed = 1.0 / cooldownTime;
			
			/*if (cooldownTime >= 1) {
				primaryskill.gotoAndPlay("avaiable");
			}else {
				if (this.bullets.currentFrameLabel != "cd") {
					secondaryskill.gotoAndPlay("cd");
				}
			}*/
			
			primaryskill.gotoAndPlay("avaiable");
			
		}
		
		public function updatePrimarySpellCooldown(cooldownTime:Number) {
			
			//primaryspell.speed = 1.0 / cooldownTime;
			
		}
		
		public function updateSecondarySpellCooldown(cooldownTime:Number) {
			
			//secondaryspell.speed = 2.0 / cooldownTime;
		}
		
		public function secondarySkillAvaiable() {
			
			//secondaryskill.speed = 2.0 / cooldownTime;
			/*if (cooldownTime >= 1) {
				secondaryskill.gotoAndPlay("avaiable");
			}else {
				if (this.bullets.currentFrameLabel != "cd") {
					secondaryskill.gotoAndPlay("cd");
				}
			}*/
			
			secondaryskill.gotoAndPlay("avaiable");
			
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