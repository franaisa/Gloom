package Server
{
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class TextFile extends MovieClip 
	{
		private var mapBox: Maps;
		private var _map:String;
		public function TextFile(m: Maps, mapa:String) 
		{
			_map = mapa;
			mapBox = m;
			addEventListener(MouseEvent.CLICK, onMouseClick);
			gotoAndPlay("off");
		}
		
		public function unLight(): void {
			gotoAndPlay("off");
		}
		
		public function onMouseClick(e: MouseEvent): void {
			mapBox.unLightElements(_map);
			gotoAndPlay("on");
			
		}
		
	}

}