package principal
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	/**
	 * Button interface
	 * @author Rubén Mulero
	 */
	public class GloomButton extends MovieClip
	{
		
		public function GloomButton() 
		{
			this.buttonMode = true;
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.CLICK, onMouseClick);
			this.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			this.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
		}
		
		private function onMouseOver(event: MouseEvent): void {
			this.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.gotoAndPlay("over");
		}
		
		private function onMouseOut(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.gotoAndPlay("idle");
		}
		
		protected function onMouseClick(e:MouseEvent):void {
			
		}
		protected function onMouseDown(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			this.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			this.gotoAndPlay("down");
		}
		protected function onMouseUp(e:MouseEvent):void {
			this.removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			this.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			this.gotoAndPlay("over");
		}
		
		public function disableButton():void {
			removeEventListener.(MouseEvent.MOUSE_OVER, onMouseOver);
			removeEventListener(MouseEvent.CLICK, onMouseClick);
			removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
		}
		
		public function enableButton():void {
			addEventListener.(MouseEvent.MOUSE_OVER, onMouseOver);
			addEventListener(MouseEvent.CLICK, onMouseClick);
			addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
		}
			
		
	}

}