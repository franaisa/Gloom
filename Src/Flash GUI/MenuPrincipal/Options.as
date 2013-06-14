package MenuPrincipal
{
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	import flash.events.MouseEvent;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class Options extends GloomButton
	{
		
		public function Options() 
		{
			super();
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("newgame");
			
		}
		
	}

}