package principal
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author Rubén Mulero Guerrero
	 */
	public class BackButton extends GloomButton 
	{
		
		public function BackButton() 
		{
			super();
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("back");
		}
		
	}

}