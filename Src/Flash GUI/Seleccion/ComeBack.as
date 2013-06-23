package Seleccion 
{
	import principal.GloomButton;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import principal.MenuManager;
	
	/**
	 * ...
	 * @author ...
	 */
	public class ComeBack extends GloomButton 
	{
		private var manager: SelectionManager;
		
		public function ComeBack() 
		{
			super();
			manager = parent as SelectionManager;
		}
		
		override protected function onMouseOver(event: MouseEvent): void {
			super.onMouseOver(event);
			manager.spellselector.gotoAndPlay("comeback");
		}
		
		override protected function onMouseOut(e:MouseEvent):void {
			super.onMouseOut(e);
			manager.spellselector.gotoAndPlay("none");
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			manager.addSpell(this.name);
		}
		
		override protected function onMouseUp(e:MouseEvent):void {
			super.onMouseUp(e);
			manager.removeSpell(this.name);
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