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
	public class GameMode extends MovieClip
	{
		
		private var modes: Array;
		private var textos: Array;
		private var manager:MenuManager;
		private var pos: int;
		private var _focusMode:String;
		
		public function GameMode() 
		{
			modes = new Array();
			manager = parent as MenuManager;
			pos = 0;
			
			pushFile("DeathMatch");
			pushFile("Duel");
			pushFile("Team DeathMatch");
			
			
		}
		
		public function get actualMode():String {
			return _focusMode;
		}
		
		public function pushFile(file: String): void {
			var newfile: TextMode = new TextMode(this, file);
			var textbox: Texto = new Texto(file);
			newfile.x = 0;
			textbox.x = 0;
			
			textbox.y =pos * newfile.height;
			newfile.y =pos * newfile.height;
			content.addChild(textbox);
			content.addChild(newfile);
			pos++;
			modes.push(newfile);
			
			
		}
		
		public function unLightElements(txt: String): void {
			_focusMode = txt;
			for each (var f:TextMode in modes) {
				f.unLight();
			}
		}
		
	}

}