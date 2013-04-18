package  
{
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.events.Event;
	
	/**
	 * ...
	 * @author ...
	 */
	public class KilledBox extends MovieClip 
	{
		private var updatealpha: int;
		
		public function KilledBox(x: int, y: int) 
		{
			killer.embedFonts = true;
			killer.defaultTextFormat.font = "Calibri"// = format;
			
			killed.embedFonts = true;
			killed.defaultTextFormat.font = "Calibri";
			
			this.x = x;
			this.y = y;
			
			this.addEventListener(Event.ENTER_FRAME, update);
			updatealpha = 300;
		}
		
		public function addkillers(kill:String, victim:String) {
			killer.text = kill;
			killed.text = victim;
		}
		
		public function update(e: Event):void {
			if (alpha <= 0) {
				return;
			}
			
			if (updatealpha < 100) {
				alpha -= 0.05;
			}else {
				updatealpha--;
			}
	
		}
	}

}