package Server
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import principal.MenuManager;
	
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
			this.buttonMode = true;
			ExternalInterface.addCallback("enableBegin", enableButton);
			ExternalInterface.addCallback("disableBegin", disableButton);
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			var mymap:Maps = manager.getChildByName("mapas") as Maps;
			ExternalInterface.call("createGame",mymap.actualMap);
			disableButton();
		}
		
	}

}