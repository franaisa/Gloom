package MenuPrincipal
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class MPButton extends GloomButton 
	{
		
		public function MPButton() 
		{
			super();
		}
		override protected function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("multiplayer");
		}
		
	}
}