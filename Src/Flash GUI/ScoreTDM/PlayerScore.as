package ScoreTDM 
{
	import flash.display.MovieClip;
	
	/**
	 * ...
	 * @author ...
	 */
	public class PlayerScore extends MovieClip 
	{
		
		private var _nick:TextField;
		private var _ping:TextField;
		private var _kills:TextField;
		private var _deaths:TextField;
		private var _spree:TextField;
		private var _icon:ClassIcon;
		
		/**
		 * Builds a new Score
		 * @param	nickname The nick of the player we are creating the score
		 * @param	playerClass The class the player has.
		 */
		public function PlayerScore( nickname:String, playerClass: String ) 
		{
			//catch de child textfields and instantiate it into my vars
			_nick 	= Nick as TextField;
			_ping 	= Ping as TextField;
			_kills 	= Kills as TextField;
			_deaths = Deaths as TextField;
			_spree 	= Spree as TextField;
			
			//get the icon control
			_icon = getChildByName("classIcon") as ClassIcon;
			
			//embedding  fonts
			
			/*var myfont:Font = new hammerhead();
			
			var format:TextFormat = new TextFormat(); 
			format.color = 0xFFFFFFFF; 
			format.size = 20; 
			format.font = myfont.fontName;
			
			_nick.embedFonts = true;
			_nick.defaultTextFormat = format//hammerhead
			
			_ping.embedFonts = true;
			_ping.defaultTextFormat = format//hammerhead
			
			_kills.embedFonts = true;
			_kills.defaultTextFormat = format//hammerhead
			
			_deaths.embedFonts = true;
			_deaths.defaultTextFormat = format//hammerhead
			
			_spree.embedFonts = true;
			_spree.defaultTextFormat = format//hammerhead
			
			//setting default values*/
			_nick.text 		= nickname;
			_ping.text 		= "0";
			_kills.text 	= "0";
			_deaths.text 	= "0";
			_spree.text 	= "0";
			
			classIcon.gotoAndPlay(playerClass);
		}
		
		/**
		 * Method who loads the icon of the player class
		 * @param	newClass hte class we are loading
		 */
		public function changeClass(newClass: String) {
			classIcon.gotoAndPlay(newClass);
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////  GETTERS/SETTERS  /////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	
		public function set nick(nick:String) {
			_nick.text = nick;
		}
		
		public function set kills(kills:int) {
			_kills.text = kills.toString();
		}
		
		public function set deaths(deaths:int) {
			_deaths.text = deaths.toString();
		}
		
		public function set spree(spree:int) {
			_spree.text = spree.toString();
		}
		
		public function set ping(ping:int) {
			_ping.text = ping.toString();
		}
		
		public function set icon(race:int) {
			_icon.gotoAndPlay(race);
		}
		
		public function get kills():int {
			return int(_kills.text);
		}
		
	}

}