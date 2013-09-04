package Server
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class TextMode extends MovieClip 
	{
		private var mapBox: GameMode;
		private var _map:String;
		public function TextMode(m: GameMode, mapa:String) 
		{
			_map = mapa;
			mapBox = m;
			addEventListener(MouseEvent.CLICK, onMouseClick);
			gotoAndPlay("off");
		}
		
		public function unLight(): void {
			if (currentFrameLabel != "off") {
				gotoAndPlay("off");
			}
			
		}
		
		public function onMouseClick(e: MouseEvent): void {
			mapBox.unLightElements(_map);
			gotoAndPlay("on");
			
		}
		
	}

}