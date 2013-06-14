package MenuPrincipal
{
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import flash.events.MouseEvent;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class NGButton extends GloomButton
	{
		
		public function NGButton() 
		{
			super();
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("newgame");
		}
		
	}

}