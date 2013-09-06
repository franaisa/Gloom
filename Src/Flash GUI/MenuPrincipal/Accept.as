package MenuPrincipal 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import principal.GloomButton;
	
	/**
	 * ...
	 * @author ...
	 */
	public class Accept extends GloomButton 
	{
		
		public function Accept() 
		{
			super();
		}
		
		protected override function onMouseClick(e:MouseEvent):void {
			var padre:MovieClip = parent as MovieClip;
			padre.visible = false;
		}
		
	}

}