package principal 
{
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author ...
	 */
	public class ResumeButton extends GloomButton 
	{
		
		public function ResumeButton() 
		{
			super();
		}
		protected override function onMouseClick(e:MouseEvent):void {
			ExternalInterface.call("resume");
		}
	}

}