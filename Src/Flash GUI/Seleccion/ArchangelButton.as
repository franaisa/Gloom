package Seleccion
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import principal.MenuManager;
	/**
	 * ...
	 * @author ...
	 */
	public class ArchangelButton extends GloomButton 
	{
		private var manager: SelectionManager;
		
		public function ArchangelButton() 
		{
			super();
			manager = parent as SelectionManager;
			
		}
		
		override protected function onMouseOver(event: MouseEvent): void {
			super.onMouseOver(event);
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("archangel");
		}
		
		override protected function onMouseOut(e:MouseEvent):void {
			super.onMouseOut(e);
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("none");
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			manager.classSelected(this.name);
		}
		
		override protected function onMouseDown(e:MouseEvent):void {
			
			if (this.currentFrameLabel == "down") {
				
				this.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
				this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				return;
			}
			this.gotoAndPlay("down");
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
	}

}