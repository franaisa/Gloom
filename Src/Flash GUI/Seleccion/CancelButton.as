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
	public class CancelButton extends GloomButton 
	{
		private var manager: MenuManager;
		
		
		public function CancelButton() 
		{
			super();
			manager = parent as MenuManager;
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			trace("cancel");
			ExternalInterface.call("selected", "cancel", "", "");
		}
		
	}

}