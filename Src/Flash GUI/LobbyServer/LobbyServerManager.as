package LobbyServer
{
	import fl.controls.List;
	import flash.display.MovieClip;
	import flash.events.Event;
	//import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author Francisco Aisa García
	 */
	public class LobbyServerManager extends MovieClip
	{
		//private var gameModes 		: Array;
		//private var mapList			: Array;
		//private var rules				: Array;
		
		private var _gameModesBox	: List;
		
		public function LobbyServerManager() {
			_gameModesBox = getChildByName("gameModesBox") as List;
			_gameModesBox.addEventListener(Event.CHANGE, modeSelected);
			
			_gameModesBox.addItem( { label : "DeathMatch" } );
			_gameModesBox.addItem( { label : "Team Deathmatch" } );
			_gameModesBox.addItem( { label : "Capture The Flag" } );
			
			//ExternalInterface.addCallback("addGameMode", addGameMode);
		}
		
		public function addGameMode(gameMode : String) : void {
			_gameModesBox.addItem( { label : gameMode } );
		}
		
		public function modeSelected(e : Event) : void {
			var gameModeSelected : String = e.target.selectedItem.label;
			trace("Game Mode = " + gameModeSelected);
		}
	}
	
}