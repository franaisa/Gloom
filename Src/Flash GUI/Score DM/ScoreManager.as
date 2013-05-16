package  
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface;
	
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class ScoreManager extends MovieClip 
	{
		//private members
		var scores:Array;
		
		public function ScoreManager() 
		{
			scores = new Array();
			
			//bind functions to C-side
			ExternalInterface.addCallback("addPlayer", addPlayer);
			ExternalInterface.addCallback("deletePlayer", deletePlayer);
			ExternalInterface.addCallback("addKill", addKill);
			ExternalInterface.addCallback("changeNick", changeNick);
			ExternalInterface.addCallback("addSpree", addSpree);
			ExternalInterface.addCallback("addDeath", addDeath);
			ExternalInterface.addCallback("changePing", changePing);
			
			addPlayer("rub0te");
			addPlayer("Gazpacho");
			
			
		}
		
		/**
		 * Adds a player to the scoreboard
		 * @param	nick The name of the player being added
		 */
		public function addPlayer(nick: String): void {
			var newscore: PlayerScore = new PlayerScore(nick);
			newscore.name = nick;
			addChild(newscore);
			
			newscore.x = 125;
			newscore.y = 183 + scores.length * 33;
			
			scores.push(newscore);
		}
		
		public function deletePlayer(nick: String): void {
			var deletescore: PlayerScore = getChildByName(nick) as PlayerScore;
			if (deletescore != null) {
				removeChild(deletescore);
			}
			//delete the element from de array
			scores.splice(scores.indexOf(deletescore), 1);
			
			//sort the array for correct viewing
			sortArray();
		}
		
		/**
		 * Function which sort the array with kills
		 */
		private function sortArray() {
			scores.sort(killSort);
			
			sortScores();
		}
		
		/**
		 * Function which
		 */
		private function sortScores() {
			
			var i: int;
			i = 0;
			for each (var score:PlayerScore in scores) {
				score.x = 125;
				score.y = 183 + i * 33;
				++i;
			}
			
		}
		
		function killSort(o1:PlayerScore, o2:PlayerScore):Number {
			if (o1.kills > o2.kills) {
				return 1;
			}else if (o1.kills < o2.kills) {
				return -1;
			}else {
				return 0;
			}
		}
		
		function addKill(nick:String, kills:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.kills = kills;
			sortScores();
		}
		
		function changeNick(oldnick:String, newnick:String):void {
			var actualScore:PlayerScore = getChildByName(oldnick) as PlayerScore;
			actualScore.nick = newnick;
		}
		
		function addSpree(nick:String, spree:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.spree = spree;
		}
		
		function addDeath(nick:String, death:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.deaths = death;
		}
		
		function changePing(nick:String, ping:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.ping = ping;
		}
		
	}

}