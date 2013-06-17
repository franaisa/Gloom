package  Server
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Rectangle;
	import flash.text.TextField;	
	import flash.external.ExternalInterface;
	/**
	 * ...
	 * @author Rubén Mulero
	 */
	public class ConsoleMP extends MovieClip 
	{
		private var cLine: TextField;
		public function ConsoleMP() 
		{
			cLine = getChildByName("commandline") as TextField;
			ExternalInterface.addCallback("pushCommand", pushCommand);
		}
		
		public function pushCommand(command: String): void {
			cLine.appendText(command + "\n");
			
		}
		
	}

}