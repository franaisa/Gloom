package ScoreTDM 
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author ...
	 */
	public class TDMManager extends MovieClip 
	{
		
		public function TDMManager() 
		{
			
			ExternalInterface.addCallback("addPlayer", addPlayer);
			ExternalInterface.addCallback("deletePlayer", deletePlayer);
			ExternalInterface.addCallback("addKill", addKill);
			ExternalInterface.addCallback("changeNick", changeNick);
			ExternalInterface.addCallback("addSpree", addSpree);
			ExternalInterface.addCallback("addDeath", addDeath);
			ExternalInterface.addCallback("changePing", changePing);
			ExternalInterface.addCallback("changeClass", changeClass);
			ExternalInterface.addCallback("addLocalPlayer", addLocalPlayer);
			ExternalInterface.addCallback("changeTeam", changeTeam);
			ExternalInterface.addCallback("changeScores", changeScores);
			/*addPlayer("manolo", "Screamer", 1);
			addPlayer("pepito", "Screamer", 1);
			addPlayer("juan", "Screamer", 2);
			addKill("juan", 2, 3);
			addPlayer("juan", "Screamer", 1);
			head.scoreblue.text = "88";*/
		}
		
		public function addPlayer(nick: String, playerClass: String, team:int): void {
			if (team == 1) {
				blue.addPlayer(nick, playerClass);
				return;
			}
			
			red.addPlayer(nick, playerClass);
		}

		public function deletePlayer(nick: String, team:int): void {
			if (team == 1) {
				blue.deletePlayer(nick);
				return;
			}
			
			red.deletePlayer(nick);
		}
		
		public function addKill(nick: String, kills: int, team:int): void {
			if (team == 1) {
				blue.addKill(nick, kills);
				return;
			}
			
			red.addKill(nick, kills);
		}
		
		public function changeNick(oldnick:String, newnick:String, team:int): void {
			if (team == 1) {
				blue.changeNick(oldnick, newnick);
				return;
			}
			
			red.changeNick(oldnick, newnick);
		}
		
		public function addSpree(nick: String, spree: int, team:int): void {
			if (team == 1) {
				blue.addSpree(nick, spree);
				return;
			}
			
			red.addSpree(nick, spree);
		}
		
		public function addDeath(nick: String, death: int, team:int): void {
			if (team == 1) {
				blue.addDeath(nick, death);
				return;
			}
			
			red.addDeath(nick, death);
		}
		
		public function changePing(nick:String, ping:int, team:int): void {
			if (team == 1) {
				blue.changePing(nick, ping);
				return;
			}
			
			red.changePing(nick, ping);
		}
		
		public function changeClass(nick:String, newClass:String, team: int): void {
			if (team == 1) {
				blue.changeClass(nick, newClass);
				return;
			}
			
			red.changeClass(nick, newClass);
		}
		
		public function addLocalPlayer(nick: String, playerClass: String, team: int): void {
			if (team == 1) {
				blue.addLocalPlayer(nick, playerClass);
				return;
			}
			
			red.addLocalPlayer(nick, playerClass);
		}
		
		public function changeScores(score: int, team: int) {
			if (team == 1) {
				head.scoreblue.text = score.toString();
				return;
			}
			
			head.scorered.text = score.toString();
		}
		
		public function changeTeam(nick:String, newTeam:int) {
			
		}
		
	}
	
	
	
	
}