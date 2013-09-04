package Client 
{
	import flash.display.MovieClip;
	import principal.GloomButton;
	import flash.external.ExternalInterface;
	import flash.events.MouseEvent;
	import principal.MenuManager
	/**
	 * ...
	 * @author ...
	 */
	public class LaunchGame extends GloomButton 
	{
		private var manager:MenuManager;
		public function LaunchGame() 
		{
			super();
			manager = parent as MenuManager;
			this.buttonMode = false;
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("createGame", manager.mapas.actualMap);
		}
		
	}

}