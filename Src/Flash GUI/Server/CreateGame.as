package  
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class CreateGame extends MovieClip 
	{
		private var manager: MenuManager;
		public function CreateGame() 
		{
			manager = parent as MenuManager;
			this.buttonMode = true;
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
			ExternalInterface.addCallback("enableCreate", enable);
			ExternalInterface.addCallback("disableCreate", disable);
		}
		
		public function disable(): void {
			gotoAndPlay("disabled");
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.removeEventListener(MouseEvent.CLICK, onMouseClick);
		}
		
		public function enable(): void {
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
			gotoAndPlay("start");
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.gotoAndPlay("rollover");
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.gotoAndPlay("rollout");
		}
		
		private function onMouseClick(e:MouseEvent):void {
			var mymap:Maps = manager.getChildByName("mapas") as Maps;
			ExternalInterface.call("createGame",mymap.actualMap);
			this.buttonMode = false;
		}
		
	}

}