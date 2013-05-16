package  
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.Font;
	
	/**
	 * Class who manages the player score in the game
	 * This class represents the stadistic information of the player in the game,
	 * such as kills, deaths, best killing spree ... and represents it in flash
	 * @author Rubén Mulero Guerrero
	 * 
	 */
	public class PlayerScore extends MovieClip 
	{
		/**
		 * Private members
		 */
		private var _nick:TextField;
		private var _ping:TextField;
		private var _kills:TextField;
		private var _deaths:TextField;
		private var _spree:TextField;
		private var _icon:ClassIcon;
		
		/**
		 * constructor
		 */
		public function PlayerScore( nickname:String ) 
		{
			
			//catch de child textfields and instantiate it into my vars
			_nick 	= getChildByName("Nick") as TextField;
			_ping 	= getChildByName("Ping") as TextField;
			_kills 	= getChildByName("Kills") as TextField;
			_deaths = getChildByName("Deaths") as TextField;
			_spree 	= getChildByName("Spree") as TextField;
			
			//get the icon control
			_icon = getChildByName("classIcon") as ClassIcon;
			
			//embedding  fonts
			
			var myfont:Font = new hammerhead();
			
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
			
			//setting default values
			_nick.text 		= nickname;
			_ping.text 		= "0";
			_kills.text 	= "0";
			_deaths.text 	= "0";
			_spree.text 	= "0";
		}
		
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////  GETTERS/SETTERS  /////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	
		public function set nick(nick:String) {
			_nick.text = nick;
		}
		
		public function set kills(kills:int) {
			_kills.text = kills as String;
		}
		
		public function set deaths(deaths:int) {
			_deaths.text = deaths as String;
		}
		
		public function set spree(spree:int) {
			_spree.text = spree as String;
		}
		
		public function set ping(ping:int) {
			_ping.text = ping as String;
		}
		
		public function set icon(race:int) {
			_icon.gotoAndPlay(race);
		}
		
		public function get kills():int {
			return _kills.text as int;
		}
	}

}