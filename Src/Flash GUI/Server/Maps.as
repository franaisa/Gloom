package Server
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	import flash.text.TextField;	
	import flash.external.ExternalInterface;
	import principal.MenuManager;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class Maps extends MovieClip
	{
		
		private var maps: Array;
		private var textos: Array;
		private var manager:MenuManager;
		private var pos: int;
		private var _focusFile:String;
		
		public function Maps() 
		{
			maps = new Array();
			manager = parent as MenuManager;
			pos = 0;
			ExternalInterface.addCallback("pushFile", pushFile);
		}
		
		public function get actualMap():String {
			return _focusFile;
		}
		
		public function pushFile(file: String): void {
			var newfile: TextFile = new TextFile(this,file);
			var textbox: Texto = new Texto(file);
			newfile.x = 0;
			textbox.x = 0;
			
			textbox.y =pos * newfile.height;
			newfile.y =pos * newfile.height;
			content.addChild(textbox);
			content.addChild(newfile);
			pos++;
			maps.push(newfile);
			
			
		}
		
		public function unLightElements(txt: String): void {
			_focusFile = txt;
			for each (var f:TextFile in maps) {
				f.unLight();
			}
		}
		
	}

}