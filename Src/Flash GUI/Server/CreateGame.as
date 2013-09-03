package Server
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import principal.MenuManager;
	import fl.controls.CheckBox;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class CreateGame extends GloomButton 
	{
		private var manager:MenuManager;
		public function CreateGame() 
		{
			super();
			manager = parent as MenuManager;
			this.buttonMode = false;
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			
			
			if (manager.modo.actualMode == null || manager.mapas.actualMap == null) {
				return;
			}
			
			ExternalInterface.call("createGame", 
								manager.mapas.actualMap, 
								manager.modo.actualMode, 
								manager.settings.servername.text,
								manager.settings.serverpass.text,
								int(manager.settings.players.text),
								int(manager.settings.spectators.text),
								int(manager.gamerules.deathlimit.text),
								int(manager.gamerules.matchtime.text),
								int(manager.gamerules.fraglimit.text),
								manager.settings.autobalance.selected,
								manager.gamerules.ff.selected,
								manager.gamerules.warmup.selected);
			
		}
		
	}

}