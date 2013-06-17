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
		private var manager: MenuManager;
		
		public function ArchangelButton() 
		{
			super();
			manager = parent as MenuManager;
			
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
			ExternalInterface.call("selected", 3);
		}
		
	}

}