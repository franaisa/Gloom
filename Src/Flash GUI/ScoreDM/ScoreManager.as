package ScoreDM 
{
	import flash.display.MovieClip;
	import flash.external.ExternalInterface;
	import ScoreDM.PlayerScore;
	
	/**
	 * Class who manages the scoreboard, adding, deleting and sorting rows when we make changes
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
			ExternalInterface.addCallback("changeClass", changeClass);
			ExternalInterface.addCallback("addLocalPlayer", addLocalPlayer);
			
		}
		
		/**
		 * Adds a player to the scoreboard
		 * @param	nick The name of the player being added
		 */
		public function addPlayer(nick: String, playerClass: String): void {
			var newscore: PlayerScore = new PlayerScore(nick, playerClass);
			newscore.name = nick;
			addChild(newscore);
			
			newscore.x = 125;
			newscore.y = 183 + scores.length * 33;
			
			scores.push(newscore);
			
			sortArray();
		}
		
		/**
		 * Adds a player to the scoreboard, but this player loads a score a bit 
		 * different because is the local player
		 * @param	nick The name of the player being added
		 */
		public function addLocalPlayer(nick: String, playerClass: String): void {
			var newscore: PlayerScore = new PlayerScore(nick, playerClass);
			newscore.name = nick;
			addChild(newscore);
			trace(newscore.kills);
			newscore.x = 125;
			newscore.y = 183 + scores.length * 33;
			
			newscore.gotoAndPlay("local");
			
			scores.push(newscore);
			sortArray();
		}
		
		/**
		 * Delete a player from de GUI and relocate the elements of the scoreboard
		 * @param	nick The score we are deleting
		 */
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
		 * Function which sort the scores by kills
		 */
		private function sortScores() {
			
			var i: int;
			i = 0;
			for each (var score:PlayerScore in scores) {
				var actualScore:PlayerScore = getChildByName(score.name) as PlayerScore;
				
				actualScore.x = 125;
				actualScore.y = 183 + i * 33;
				++i;
			}
			
		}
		
		/**
		 * function passed to the array that sort by kills
		 * @param	o1 The first element compared
		 * @param	o2 The secon element compared
		 * @return 	1 if o1 is greather than o2, -1 if o2 is greather tan o1
		 * 			and 0 if they are equal.
		 */
		function killSort(o1:PlayerScore, o2:PlayerScore):Number {
			
			if (o1.kills > o2.kills) {
				return -1;
			}else if (o1.kills < o2.kills) {
				return 1;
			}else {
				return 0;
			}
		}
		
		/**
		 * Method that adds a kill to a concrete score
		 * @param	nick The player we are adding a kill
		 * @param	kills The kills the player has
		 */
		function addKill(nick:String, kills:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.kills = kills;
			
			var idx: int = scores.indexOf(actualScore);
				
			trace(scores[idx].kills);
			
			sortArray();
			
		}
		
		/**Method who changes the nick of a concrete player
		 * 
		 * @param	oldnick The nick the player had.
		 * @param	newnick The new nick the player has.
		 */
		function changeNick(oldnick:String, newnick:String):void {
			var actualScore:PlayerScore = getChildByName(oldnick) as PlayerScore;
			actualScore.nick = newnick;
		}
		
		/**
		 * Method who adds a new killing spree to the player
		 * @param	nick The player we are changing de spree
		 * @param	spree The spree the player has.
		 */
		function addSpree(nick:String, spree:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.spree = spree;
		}
		
		/**
		 * Method who adds a death to a player
		 * @param	nick The player we are adding deaths
		 * @param	death the deaths the player has.
		 */
		function addDeath(nick:String, death:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.deaths = death;
		}
		
		/**
		 * Method who changes de ping of a player
		 * @param	nick The player we are modifying
		 * @param	ping The ping the player has.
		 */
		function changePing(nick:String, ping:int):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.ping = ping;
		}
		
		/**
		 * Method who changes de class to a concrete player
		 * @param	nick The player who changed his class
		 * @param	newClass The new class the player has
		 */
		function changeClass(nick:String, newClass:String):void {
			var actualScore:PlayerScore = getChildByName(nick) as PlayerScore;
			actualScore.changeClass(newClass);
		}
		
	}

}