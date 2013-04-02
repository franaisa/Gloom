package 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class ScreamerButton extends MovieClip 
	{
		private var manager: MenuManager;
		
		
		public function ScreamerButton() 
		{
			manager = parent as MenuManager;
			this.buttonMode = true;
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.gotoAndPlay("rollover");
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("screamer");
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.gotoAndPlay("idle");
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("none");
		}
		
		private function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("screamer");
		}
		
	}

}