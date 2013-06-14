package  
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class BackButton extends GloomButton 
	{
		
		public function BackButton() 
		{
			super();
		}
		
		override private function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("back");
		}
		
	}

}