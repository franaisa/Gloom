package Seleccion 
{
	import flash.events.MouseEvent;
	import principal.GloomButton;
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author ...
	 */
	public class Spectator extends GloomButton 
	{
		
		public function Spectator() 
		{
			super();
		}
		
		override protected function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("selected", "spectator", "", "");
		}
	}

}