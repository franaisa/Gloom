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
	public class BeginGame extends GloomButton 
	{
		private var manager:MenuManager;
		public function BeginGame() 
		{
			super();
			manager = parent as MenuManager;
			this.buttonMode = true;
			ExternalInterface.addCallback("enableBegin", enableButton);
			ExternalInterface.addCallback("disableBegin", disableButton);
			disableButton();
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			var mymap:Maps = manager.getChildByName("mapas") as Maps;
			ExternalInterface.call("newgame",mymap.actualMap);
		}
		
	}

}