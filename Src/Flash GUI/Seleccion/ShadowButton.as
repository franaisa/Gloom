package Seleccion
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import principal.MenuManager;
	/**
	 * ...
	 * @author Rubén Mulero Guerrero
	 */
	public class ShadowButton extends GloomButton 
	{
		private var manager: MenuManager;
		
		
		public function ShadowButton() 
		{
			super();
			manager = parent as MenuManager;
		}
		
		override protected function onMouseOver(event: MouseEvent): void {
			super.onMouseOver(event);
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("shadow");
		}
		
		override protected function onMouseOut(e:MouseEvent):void {
			super.onMouseOut(e);
			var selector: ImageSelector = manager.getChildByName("selector") as ImageSelector;
			selector.gotoAndPlay("none");
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("selected", 4);
		}
		
	}

}