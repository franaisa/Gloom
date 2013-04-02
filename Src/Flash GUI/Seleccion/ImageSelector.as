package 
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	
	/**
	 * ...
	 * @author ...
	 */
	public class ImageSelector extends MovieClip 
	{
		
		public function ImageSelector() 
		{
			gotoAndPlay("none");
		}
		
		public function putImage(image:String):void {
			gotoAndPlay(image);
		}
		
	}

}